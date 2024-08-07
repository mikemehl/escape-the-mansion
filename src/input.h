#ifndef __INPUT__H__
#define __INPUT__H__
#include "flecs.h"
#include <raylib.h>

typedef struct InputActions {
  bool up;
  bool left;
  bool down;
  bool right;
} InputActions;

extern ECS_COMPONENT_DECLARE(InputActions);

void InputImport(ecs_world_t *world);

#endif
