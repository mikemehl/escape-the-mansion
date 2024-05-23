#include "title.h"
#include "tonc_tte.h"
#include <tonc.h>

#define CELL_H (tte_get_font()->cellH)
#define CELL_W (tte_get_font()->cellW)
#define TITLE_Y ((SCREEN_HEIGHT / 2) - (tte_get_font()->cellH / 2))
#define TITLE_X ((SCREEN_WIDTH / 2) - ((tte_get_font()->cellW * 16) / 2))

static bool init = false;
static int title_frame_ctr = 0;

static void title_draw_press_start() {
  if (title_frame_ctr == 0) {
    tte_set_pos(TITLE_X + CELL_W * 3, TITLE_Y + CELL_H);
    tte_write("PRESS START");
  } else if (title_frame_ctr == 30) {
    tte_set_pos(TITLE_X - CELL_W * 10, TITLE_Y + CELL_H);
    tte_erase_line();
  }
  title_frame_ctr = (title_frame_ctr + 1) % 60;
}

void title_draw() {
  if (!init) {
    tte_init_chr4c_default(0, BG_CBB(0) | BG_SBB(31));
    tte_set_pos(TITLE_X, TITLE_Y);
    init = true;
    tte_write("ESCAPE THE MANSION");
  }
  title_draw_press_start();
}

void title_update() {}
