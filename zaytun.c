#ifndef ZAYTUN_C
#define ZAYTUN_C

#include <stddef.h>
#include <stdint.h>

#define SWAP(type, a, b)                                                       \
  {                                                                            \
    type t = a;                                                                \
    a = b;                                                                     \
    b = t;                                                                     \
  }

// 0xAABBGGRR
typedef enum {
  COMP_RED = 0,
  COMP_GREEN,
  COMP_BLUE,
  COMP_ALPHA,
  COMP_COUNT,
} Comp_Index;

// taken as u16 so that it expands automatically
uint8_t mix_comps(uint16_t c1, uint16_t c2, uint16_t a) {
  // lerp: a, b, t = a + (b-a)*t where 0 <= t <= 1
  // so we need t in range of 0..1 but we have 0..255
  // c1 + (c2-c1) * a/255
  // c1 + c2*a/255 - c1*a/255
  // (c1 + c2 - c1)a/255
  return c1 + (c2 - c1) * a / 255;
}

void unpack_rgb32(uint32_t c, uint8_t comp[COMP_COUNT]) {
  for (int i = 0; i < COMP_COUNT; i++) {
    comp[i] = c & 0xFF;
    c >>= 8;
  }
}

uint32_t pack_rgb32(uint8_t comp[COMP_COUNT]) {
  uint32_t res = 0;
  for (size_t i = 0; i < COMP_COUNT; i++) {
    res |= comp[i] << (8 * i);
  }

  return res;
}

uint32_t mix_colors(uint32_t c1, uint32_t c2) {
  uint8_t comp1[COMP_COUNT];
  unpack_rgb32(c1, comp1);

  uint8_t comp2[COMP_COUNT];
  unpack_rgb32(c2, comp2);

  for (size_t i = 0; i < COMP_ALPHA; ++i) {
    comp1[i] = mix_comps(comp1[i], comp2[i], comp2[COMP_ALPHA]);
  }

  // if (comp1[COMP_ALPHA] == 0) {
  //   comp1[COMP_ALPHA] = comp2[COMP_ALPHA];
  // }

  return pack_rgb32(comp1);
}

void fill(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
          uint32_t color) {
  for (int i = 0; i < (int)(pixels_width * pixels_height); ++i) {
    // pixels[i] = mix_colors(pixels[i], color);
    pixels[i] = color;
  }
}

// provide -w -h to draw from the oposide side
void fill_rect(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
               int x0, int y0, int w, int h, uint32_t color) {
  if (w < 0) {
    w *= -1;
    x0 -= w;
  }

  if (h < 0) {
    h *= -1;
    y0 -= h;
  }

  for (int dy = 0; dy < h; dy++) {
    int y = y0 + dy;
    if (0 <= y && y < (int)pixels_height) {
      for (int dx = 0; dx < w; dx++) {
        int x = x0 + dx;
        if (0 <= x && x < (int)pixels_width) {
          pixels[y * pixels_width + x] =
              mix_colors(pixels[y * pixels_width + x], color);
        }
      }
    }
  }
}

void fill_circule(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
                  int cx, int cy, int r, uint32_t color) {
  int x1 = cx - r;
  int y1 = cy - r;
  int x2 = cx + r;
  int y2 = cy + r;

  for (int y = y1; y <= y2; y++) {
    if (0 <= y && y < (int)pixels_height) {
      for (int x = x1; x <= x2; x++) {
        if (0 <= x && x < (int)pixels_width) {
          int dx = x - cx;
          int dy = y - cy;
          if (dx * dx + dy * dy <= r * r) {
            pixels[y * pixels_width + x] =
                mix_colors(pixels[y * pixels_width + x], color);
          }
        }
      }
    }
  }
}

void draw_line(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
               int x1, int y1, int x2, int y2, uint32_t color) {
  int dx = x1 - x2;
  int dy = y1 - y2;
  if (dx != 0) {
    int c = y1 - (dy * x1) / dx;

    if (x1 > x2)
      SWAP(int, x1, x2);

    for (int x = x1; x <= x2; x++) {
      if (0 <= x && x < (int)pixels_width) {
        int sy1 = (dy * x) / dx + c;       // current y
        int sy2 = (dy * (x + 1)) / dx + c; // next y
        if (sy1 > sy2)
          SWAP(int, sy1, sy2);
        for (int y = sy1; y <= sy2; ++y) {
          if (0 <= y && y < (int)pixels_height) {
            pixels[y * pixels_width + x] =
                mix_colors(pixels[y * pixels_width + x], color);
          }
        }
      }
    }

  } else {
    // perpentigular to --------x--------
    int x = x1;
    if (0 <= x && x < (int)pixels_width) {
      if (y1 > y2)
        SWAP(int, y1, y2);
      for (int y = y1; y <= y2; y++) {
        if (0 <= y && y < (int)pixels_height) {
          pixels[y * pixels_width + x] =
              mix_colors(pixels[y * pixels_width + x], color);
        }
      }
    }
  }
}

void triangle_sort_by_y(int *x1, int *y1, int *x2, int *y2, int *x3, int *y3) {
  //
  if (*y1 > *y2) {
    SWAP(int, *x1, *x2);
    SWAP(int, *y1, *y2);
  }

  if (*y2 > *y3) {
    SWAP(int, *x2, *x3);
    SWAP(int, *y2, *y3);
  }

  if (*y1 > *y2) {
    SWAP(int, *x1, *x2);
    SWAP(int, *y1, *y2);
  }
}

void fill_triangle(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
                   int x1, int y1, int x2, int y2, int x3, int y3,
                   uint32_t color) {
  triangle_sort_by_y(&x1, &y1, &x2, &y2, &x3, &y3);

  int dx12 = x2 - x1;
  int dy12 = y2 - y1;
  int dx13 = x3 - x1;
  int dy13 = y3 - y1;

  for (int y = y1; y <= y2; ++y) {
    if (0 <= y && (size_t)y < pixels_height) {
      int s1 = dy12 != 0 ? (y - y1) * dx12 / dy12 + x1 : x1;
      int s2 = dy13 != 0 ? (y - y1) * dx13 / dy13 + x1 : x1;
      if (s1 > s2)
        SWAP(int, s1, s2);
      for (int x = s1; x <= s2; x++) {
        if (0 <= x && (size_t)x < pixels_width) {
          pixels[y * pixels_width + x] =
              mix_colors(pixels[y * pixels_width + x], color);
        }
      }
    }
  }

  int dx32 = x2 - x3;
  int dy32 = y2 - y3;
  int dx31 = x1 - x3;
  int dy31 = y1 - y3;

  for (int y = y2; y <= y3; ++y) {
    if (0 <= y && (size_t)y < pixels_height) {
      int s1 = dy32 != 0 ? (y - y3) * dx32 / dy32 + x3 : x3;
      int s2 = dy31 != 0 ? (y - y3) * dx31 / dy31 + x3 : x3;
      if (s1 > s2)
        SWAP(int, s1, s2);
      for (int x = s1; x <= s2; x++) {
        if (0 <= x && (size_t)x < pixels_width) {
          pixels[y * pixels_width + x] =
              mix_colors(pixels[y * pixels_width + x], color);
        }
      }
    }
  }
}

#endif // ZAYTUN_INPLIMENTATRION
