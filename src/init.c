#include "init.h"
#include <tonc.h>

void init_system() {
  irq_init(NULL);
  irq_enable(II_VBLANK);
  tte_init_chr4c_default(0, BG_CBB(0) | BG_SBB(31));
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
}
