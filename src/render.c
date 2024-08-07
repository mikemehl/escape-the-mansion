#include "render.h"

ECS_COMPONENT_DECLARE(RectSprite);

void RenderImport(ecs_world_t *world) {
  ECS_MODULE(world, Render);
  ECS_COMPONENT_DEFINE(world, RectSprite);
}
