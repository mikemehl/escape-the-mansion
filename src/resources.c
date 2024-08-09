#include "resources.h"
#include "tmx.h"
#include <raylib.h>
#include <stdlib.h>

ECS_COMPONENT_DECLARE(Sprite);

Sprite load_walk_sprite() {
  Texture2D walk = LoadTexture("./assets/characters/HumanTownsfolkWalk.png");
  Sprite    walk_sprite = {
         .texture = walk, .area = {.x = 12, .y = 10, .width = 8, .height = 10}
  };
  return walk_sprite;
}

static void load_tiled() {
  tmx_map *test_room = tmx_load("./assets/test-room.tmx");
  if (test_room == NULL) {
    const char *err = tmx_strerr();
    printf("%s\n", err);
    exit(1);
  }
}

void ResourcesImport(ecs_world_t *world) {
  ECS_MODULE(world, Resources);
  ECS_COMPONENT_DEFINE(world, Sprite);
  load_tiled();
}
