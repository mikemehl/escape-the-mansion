#include "render.h"
#include "physics.h"
#include "resources.h"
#include <raylib.h>

ECS_COMPONENT_DECLARE(RectSprite);
ECS_COMPONENT_DECLARE(CameraFollow);
ECS_SYSTEM_DECLARE(SystemDrawSprite);
ECS_SYSTEM_DECLARE(SystemDrawRectSprite);
ECS_SYSTEM_DECLARE(SystemCameraDrawBegin);
ECS_SYSTEM_DECLARE(SystemCameraDrawEnd);
ECS_SYSTEM_DECLARE(SystemCameraUpdate);

static void SystemDrawSprite(ecs_iter_t *it) {
  Position *p      = ecs_field(it, Position, 0);
  Sprite   *sprite = ecs_field(it, Sprite, 1);

  for (int i = 0; i < it->count; i++) {
    DrawTextureRec(sprite->texture, sprite->area, *p, WHITE);
  }
}

static void SystemDrawRectSprite(ecs_iter_t *it) {
  Position   *p = ecs_field(it, Position, 0);
  RectSprite *r = ecs_field(it, RectSprite, 1);
  assert(p);
  assert(r);

  for (int i = 0; i < it->count; i++) {
    DrawRectangle(p[i].x, p[i].y, r[i].dimensions.width, r[i].dimensions.height,
                  r[i].color);
  }
}

static void SystemCameraDrawBegin(ecs_iter_t *it) {
  CameraFollow *camera = ecs_field(it, CameraFollow, 0);
  assert(camera);
  assert(it->count == 1);
  BeginMode2D(*camera);
}

static void SystemCameraDrawEnd(ecs_iter_t *it) {
  CameraFollow *camera = ecs_field(it, CameraFollow, 0);
  assert(camera);
  assert(it->count == 1);
  EndMode2D();
}

static void SystemCameraUpdate(ecs_iter_t *it) {
  CameraFollow *camera = ecs_field(it, CameraFollow, 0);
  Position     *pos    = ecs_field(it, Position, 1);

  for (int i = 0; i < it->count; i++) {
    camera[i].target = pos[i];
  }
}

void RenderImport(ecs_world_t *world) {
  ECS_MODULE(world, Render);
  ECS_COMPONENT_DEFINE(world, RectSprite);
  ECS_COMPONENT_DEFINE(world, CameraFollow);
  ECS_SYSTEM_DEFINE(world, SystemDrawSprite, EcsOnUpdate, physics.Position,
                    resources.Sprite);
  ECS_SYSTEM_DEFINE(world, SystemDrawRectSprite, 0, physics.Position,
                    render.RectSprite);
  ECS_SYSTEM_DEFINE(world, SystemCameraDrawBegin, 0, render.CameraFollow);
  ECS_SYSTEM_DEFINE(world, SystemCameraDrawEnd, 0, render.CameraFollow);
  ECS_SYSTEM_DEFINE(world, SystemCameraUpdate, 0, render.CameraFollow,
                    physics.Position);
}
