#include "render.h"
#include "physics.h"
#include "resources.h"
#include <raylib.h>

ECS_COMPONENT_DECLARE(RectSprite);
ECS_COMPONENT_DECLARE(CameraFollow);
ECS_SYSTEM_DECLARE(SystemDrawSprite);
ECS_SYSTEM_DECLARE(SystemDrawRectSprite);

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

void RenderImport(ecs_world_t *world) {
  ECS_MODULE(world, Render);
  ECS_COMPONENT_DEFINE(world, RectSprite);
  ECS_COMPONENT_DEFINE(world, CameraFollow);
  ECS_SYSTEM_DEFINE(world, SystemDrawSprite, EcsOnUpdate, physics.Position,
                    resources.Sprite);
  ECS_SYSTEM_DEFINE(world, SystemDrawRectSprite, 0, physics.Position,
                    render.RectSprite);
}
