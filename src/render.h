#ifndef __RENDER__H__
#define __RENDER__H__
#include <flecs.h>
#include <raylib.h>

typedef struct RectSprite {
  Rectangle dimensions;
  Color color;
} RectSprite;

typedef Camera2D CameraFollow;

extern ECS_COMPONENT_DECLARE(RectSprite);
extern ECS_COMPONENT_DECLARE(CameraFollow);
extern ECS_SYSTEM_DECLARE(SystemDrawSprite);
extern ECS_SYSTEM_DECLARE(SystemDrawRectSprite);
extern ECS_SYSTEM_DECLARE(SystemDrawAnimatedSprite);
extern ECS_SYSTEM_DECLARE(SystemDrawBegin);
extern ECS_SYSTEM_DECLARE(SystemDrawEnd);
extern ECS_SYSTEM_DECLARE(SystemCameraDrawBegin);
extern ECS_SYSTEM_DECLARE(SystemCameraDrawEnd);
extern ECS_SYSTEM_DECLARE(SystemCameraUpdate);
extern ECS_SYSTEM_DECLARE(SystemDrawRoom);

void RenderImport(ecs_world_t *world);
void RenderRoom();

#endif // !__RENDER__H__
