#include "resources.h"
#include "tmx.h"
#include <raylib.h>
#include <stdlib.h>

ECS_COMPONENT_DECLARE(Sprite);
ECS_COMPONENT_DECLARE(Tiled);

Sprite load_walk_sprite() {
  Texture2D walk = LoadTexture("./assets/characters/HumanTownsfolkWalk.png");
  Sprite    walk_sprite = {
         .texture = walk, .area = {.x = 12, .y = 10, .width = 8, .height = 10}
  };
  return walk_sprite;
}

static void *load_tiled_img(const char *path) {
  Texture2D *img = NULL;
  img            = tmx_alloc_func(img, sizeof(Texture2D));
  if (img == NULL) {
    return img;
  }

  *img = LoadTexture(path);

  return img;
}

static void free_tiled_img(void *img) {
  tmx_free_func(img);
}

static void LoadTiled(ecs_world_t *world) {
  tmx_img_load_func  = load_tiled_img;
  tmx_img_free_func  = free_tiled_img;
  tmx_map *test_room = tmx_load("./assets/test-room.tmx");
  if (test_room == NULL) {
    tmx_perror("Error loading test room: ");
    exit(1);
  }
  ecs_singleton_set_ptr(world, Tiled, test_room);
}

void ResourcesImport(ecs_world_t *world) {
  ECS_MODULE(world, Resources);
  ECS_COMPONENT_DEFINE(world, Sprite);
  ECS_COMPONENT_DEFINE(world, Tiled);
  LoadTiled(world);
}
