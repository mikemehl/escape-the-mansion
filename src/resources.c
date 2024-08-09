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

tmx_map *load_tiled() {
  static tmx_map *test_room = NULL;
  if (test_room == NULL) {
    tmx_img_load_func = load_tiled_img;
    tmx_img_free_func = free_tiled_img;
    test_room         = tmx_load("./assets/test-room.tmx");
    if (test_room == NULL) {
      tmx_perror("Error loading test room: ");
      exit(1);
    }
  }
  return test_room;
}

void RenderRoom() {
  tmx_map const *const room  = load_tiled();
  tmx_layer const     *layer = room->ly_head;
  while (layer) {
    if (layer->type != L_LAYER) {
      layer = layer->next;
      continue;
    }
    uint32_t const *gids = layer->content.gids;
    for (uint32_t x = 0; x < room->width; x++) {
      for (uint32_t y = 0; y < room->height; y++) {
        uint32_t gid = (gids[y * room->width + x]) & TMX_FLIP_BITS_REMOVAL;
        if (gid == 0) {
          continue;
        }
        printf("[RenderRoom] gid = %u\n", gid);
        tmx_tile const *const    tile      = room->tiles[gid];
        tmx_tileset const *const tileset   = room->tiles[gid]->tileset;
        Texture2D               *img       = tileset->image->resource_image;
        Rectangle                tile_rect = {.x      = tile->ul_x,
                                              .y      = tile->ul_y,
                                              .width  = tile->width,
                                              .height = tile->height};
        Vector2                  pos       = {.x = x, .y = y};
        DrawTextureRec(*img, tile_rect, pos, WHITE);
      }
    }
    layer = layer->next;
  }
}

void ResourcesImport(ecs_world_t *world) {
  ECS_MODULE(world, Resources);
  ECS_COMPONENT_DEFINE(world, Sprite);
  load_tiled();
}
