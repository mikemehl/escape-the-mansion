#include "render.h"
#include "image.h"
#include "physics.h"
#include <raylib.h>

ECS_COMPONENT_DECLARE(RectSprite);
ECS_SYSTEM_DECLARE(system_draw_sprite);

void system_draw_sprite(ecs_iter_t *it) {
  Position *p = ecs_field(it, Position, 0);
  Sprite *sprite = ecs_field(it, Sprite, 1);

  for (int i = 0; i < it->count; i++) {
    DrawTextureRec(sprite->texture, sprite->area, *p, WHITE);
  }
}

void RenderImport(ecs_world_t *world) {
  ECS_MODULE(world, Render);
  ECS_COMPONENT_DEFINE(world, RectSprite);
  ECS_SYSTEM_DEFINE(world, system_draw_sprite, EcsOnUpdate, physics.Position,
                    image.Sprite);
}
