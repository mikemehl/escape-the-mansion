#include "image.h"
#include <raylib.h>

ECS_COMPONENT_DECLARE(Sprite);

Sprite load_walk_sprite() {
  Texture2D walk = LoadTexture("./assets/characters/HumanTownsfolkWalk.png");
  Sprite walk_sprite = {.texture = walk,
                        .area = {.x = 12, .y = 10, .width = 8, .height = 10}};
  return walk_sprite;
}

void ImageImport(ecs_world_t *world) {
  ECS_MODULE(world, Image);
  ECS_COMPONENT_DEFINE(world, Sprite);
}
