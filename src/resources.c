#include "resources.h"
#include <raylib.h>

#define CUTE_TILED_IMPLEMENTATION
#include "cute_tiled.h"

ECS_COMPONENT_DECLARE(Sprite);

Sprite load_walk_sprite() {
  Texture2D walk = LoadTexture("./assets/characters/HumanTownsfolkWalk.png");
  Sprite    walk_sprite = {
         .texture = walk, .area = {.x = 12, .y = 10, .width = 8, .height = 10}
  };
  return walk_sprite;
}

void ResourcesImport(ecs_world_t *world) {
  ECS_MODULE(world, Resources);
  ECS_COMPONENT_DEFINE(world, Sprite);
}

static void load_tiled() {
  cute_tiled_map_t *test_room =
      cute_tiled_load_map_from_file("../assets/test-room.json", NULL);
  cute_tiled_map_t *character =
      cute_tiled_load_map_from_file("../assets/character.json", NULL);
}
