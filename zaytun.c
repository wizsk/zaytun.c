#ifndef ZAYTUN_C
#define ZAYTUN_C

#include <stddef.h>
#include <stdint.h>

#define SWAP(t, a, b)                                                          \
  {                                                                            \
    t tmp = a;                                                                 \
    a = b;                                                                     \
    b = tmp;                                                                   \
  }

void fill(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
          uint32_t color) {
  for (int i = 0; i < (int)(pixels_width * pixels_height); ++i) {
    pixels[i] = color;
  }
}

void fill_rect(uint32_t *pixels, size_t pixels_width, size_t pixels_height,
               int x0, int y0, uint32_t w, uint32_t h, uint32_t color) {
  for (int dy = 0; dy < (int)h; dy++) {
    int y = y0 + dy;
    if (0 <= y && y < (int)pixels_height) {
      for (int dx = 0; dx < (int)w; dx++) {
        int x = x0 + dx;
        if (0 <= x && x < (int)pixels_width) {
          pixels[y * pixels_width + x] = color;
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
            pixels[y * pixels_width + x] = color;
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
            pixels[y * pixels_width + x] = color;
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
          pixels[y * pixels_width + x] = color;
        }
      }
    }
  }
}

// void fill_triangle(uint32_t *pixels, size_t pixels_width, size_t
// pixels_height,
//                    int x1, int y1, int x2, int y2, int x3, int y3,
//                    uint32_t color) {
//   //
// }
#endif // ZAYTUN_INPLIMENTATRION
