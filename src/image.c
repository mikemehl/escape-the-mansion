#include "image.h"
#include <raylib.h>

ECS_COMPONENT_DECLARE(Sprite);

Sprite load_walk_sprite() {
  Texture2D walk = LoadTexture("./assets/characters/HumanTownsfolkWalk.png");
  Sprite walk_sprite = {.texture = walk,
                        .area = {.x = 0, .y = 0, .width = 32, .height = 32}};
  return walk_sprite;
}

void ImageImport(ecs_world_t *world) {
  ECS_MODULE(world, Image);
  ECS_COMPONENT_DEFINE(world, Sprite);
}
