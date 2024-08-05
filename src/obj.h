#ifndef __OBJ_H__
#define __OBJ_H__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define NUM_OBJS 100

typedef struct Rect {
  int width;
  int height;
} Rect;

typedef struct Obj {
  uint32_t flags;
  float position[NUM_OBJS][2];
  float velocity[NUM_OBJS][2];
  Rect rectangle[NUM_OBJS];
  Rect collision_box[NUM_OBJS];
} Obj;

typedef enum ObjFlags {
  ACTIVE = 1,
  PLAYER = (1 << 1),
  POS = (1 << 2),
  VEL = (1 << 3),
  RECT = (1 << 4),
  COLLISON_BOX = (1 << 5),
} ObjFlags;

Obj *obj_new();
void obj_return(Obj *obj);

#ifdef __OBJ__IMPL__

static Obj objs[NUM_OBJS];
Obj *obj_new() {
  for (int i = 0; i < NUM_OBJS; i++) {
    if ((objs[i].flags & ACTIVE) == 0) {
      objs[i].flags |= ACTIVE;
      return &objs[i];
    }
  }
  return NULL;
}

void obj_return(Obj *obj) {
  if (obj >= &objs[0] && obj < &objs[NUM_OBJS]) {
    obj->flags &= ~ACTIVE;
  }
}

#endif

#endif // !__OBJ_H__
