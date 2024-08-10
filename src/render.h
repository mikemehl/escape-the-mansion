#ifndef __RENDER__H__
#define __RENDER__H__
#include "flecs.h"
#include <raylib.h>

typedef struct RectSprite {
  Rectangle dimensions;
  Color     color;
} RectSprite;

typedef Camera2D CameraFollow;

extern ECS_COMPONENT_DECLARE(RectSprite);
extern ECS_COMPONENT_DECLARE(CameraFollow);
extern ECS_SYSTEM_DECLARE(SystemDrawSprite);
extern ECS_SYSTEM_DECLARE(SystemDrawRectSprite);

void RenderImport(ecs_world_t *world);
void RenderRoom();

#endif // !__RENDER__H__
