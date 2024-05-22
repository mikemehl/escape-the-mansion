#include "title.h"
#include <tonc.h>
void title_draw() {
  tte_init_chr4c_default(0, BG_CBB(0) | BG_SBB(31));
  tte_set_pos(92, 68);
  tte_write("Hello World!");
}
