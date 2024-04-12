#include "../zaytun.c"
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../3rd_party/stb_image_write.h"

#include <stdio.h>
#include <sys/stat.h>

#define SCALER 2
#define WIDTH (SCALER * 800)
#define HEIGHT (SCALER * 600)

#define IMG_DIR "demos"
// #define IMG_DIR "test"

#define BACLGROUD_COLOR 0xFF202020
#define FOREGROUD_COLOR 0xFF0000FF

// access a pixed [HEIGHT*x + WIDTH]
uint32_t pixels[HEIGHT * WIDTH];

int save_to_file(uint32_t *pixels, size_t w, size_t h, const char *file_name) {
  return stbi_write_png(file_name, w, h, 4, pixels, w * sizeof(uint32_t));
}

// for the cheker board
#define COLS (8 * 2)
#define ROWS (6 * 2)
#define CELL_WIDTH (WIDTH / COLS)
#define CELL_HEIGHT (HEIGHT / ROWS)
void checkerboard_rec() {
  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x) {
      uint32_t color;
      if ((x + y) % 2 == 0) {
        color = FOREGROUD_COLOR;
      } else {
        color = BACLGROUD_COLOR;
      }

      fill_rect(pixels, WIDTH, HEIGHT, x * CELL_WIDTH, y * CELL_HEIGHT,
                CELL_WIDTH, CELL_HEIGHT, color);
    }
  }
  const char *ouput = IMG_DIR "/chekerboard_rec.png";
  if (save_to_file(pixels, WIDTH, HEIGHT, ouput) < 0) {
    printf("err: while saveing file %s\n", ouput);
  }
}

// soto -> boro
float lerpf(float a, float b, float t) { return a + (b - a) * t; }

void checkerboard_cir() {
  fill(pixels, WIDTH, HEIGHT, BACLGROUD_COLOR);
  for (int y = 0; y < ROWS; ++y) {
    for (int x = 0; x < COLS; ++x) {
      float u = (float)x / COLS;
      float v = (float)y / ROWS;
      float t = (u + v) / 2;

      int r = CELL_WIDTH;
      if (CELL_HEIGHT < r)
        r = CELL_HEIGHT;

      fill_circle(pixels, WIDTH, HEIGHT, x * CELL_WIDTH + CELL_WIDTH / 2,
                   y * CELL_HEIGHT + CELL_HEIGHT / 2,
                   (int)lerpf((float)r / 8, (float)r / 2, t), FOREGROUD_COLOR);
    }
  }
  const char *ouput = IMG_DIR "/chekerboard_cir.png";
  if (save_to_file(pixels, WIDTH, HEIGHT, ouput) < 0) {
    printf("err: while saveing file %s\n", ouput);
  }
}

void lines_man() {
  fill(pixels, WIDTH, HEIGHT, BACLGROUD_COLOR);

  uint32_t c = FOREGROUD_COLOR;
  draw_line(pixels, WIDTH, HEIGHT, WIDTH / 2, HEIGHT, WIDTH / 2, 0, c);
  draw_line(pixels, WIDTH, HEIGHT, 0, HEIGHT / 2, WIDTH, HEIGHT / 2, c);

  c = 0xFF00FA00;
  draw_line(pixels, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, c);
  draw_line(pixels, WIDTH, HEIGHT, 0, HEIGHT, WIDTH, 0, c);

  c = 0xFFF40000;
  draw_line(pixels, WIDTH, HEIGHT, 0, 0, WIDTH / 4, HEIGHT, c);
  draw_line(pixels, WIDTH, HEIGHT, WIDTH / 4, 0, 0, HEIGHT, c);
  draw_line(pixels, WIDTH, HEIGHT, WIDTH, 0, WIDTH / 4 * 3, HEIGHT, c);
  draw_line(pixels, WIDTH, HEIGHT, WIDTH / 4 * 3, 0, WIDTH, HEIGHT, c);

  const char *ouput = IMG_DIR "/lines.png";
  if (save_to_file(pixels, WIDTH, HEIGHT, ouput) < 0) {
    printf("err: while saveing file %s\n", ouput);
  }
}

void triangle() {
  fill(pixels, WIDTH, HEIGHT, BACLGROUD_COLOR);

  uint32_t c = FOREGROUD_COLOR;
  fill_triangle(pixels, WIDTH, HEIGHT, WIDTH / 2, HEIGHT / 8, WIDTH / 7,
                HEIGHT / 2, WIDTH * 7 / 8, HEIGHT * 7 / 8, c);

  const char *ouput = IMG_DIR "/trianlge.png";
  if (save_to_file(pixels, WIDTH, HEIGHT, ouput) < 0) {
    printf("err: while saveing file %s\n", ouput);
  }
}

void tranparency() {
  fill(pixels, WIDTH, HEIGHT, BACLGROUD_COLOR);

  fill_rect(pixels, WIDTH, HEIGHT, 0, 0, WIDTH * 3 / 4, HEIGHT * 3 / 4,
            0xFF0000FF);

  fill_rect(pixels, WIDTH, HEIGHT, WIDTH, HEIGHT, -WIDTH * 3 / 4,
            -HEIGHT * 3 / 4, 0x6600FF00);

  fill_circle(pixels, WIDTH, HEIGHT, WIDTH / 2, HEIGHT / 2, WIDTH * 2 / 6,
               0x77FF0000);

  fill_triangle(pixels, WIDTH, HEIGHT, WIDTH / 2, 0, 0, HEIGHT, WIDTH, HEIGHT,
                0x77A0FFA0);
  const char *ouput = IMG_DIR "/tranperency.png";
  if (save_to_file(pixels, WIDTH, HEIGHT, ouput) < 0) {
    printf("err: while saveing file %s\n", ouput);
  }
}

int main() {
  mkdir(IMG_DIR, 0777);
  lines_man();
  checkerboard_rec();
  checkerboard_cir();
  triangle();
  tranparency();
}
