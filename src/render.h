#ifndef __RENDER__H__
#define __RENDER__H__
#include "flecs.h"
#include <raylib.h>

typedef struct RectSprite {
  Rectangle dimensions;
  Color color;
} RectSprite;

extern ECS_COMPONENT_DECLARE(RectSprite);

void RenderImport(ecs_world_t *world);

#endif // !__RENDER__H__
