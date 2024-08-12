#include "resources.h"
#include "tmx.h"
#include <raylib.h>
#include <stdlib.h>

ECS_COMPONENT_DECLARE(Sprite);
ECS_COMPONENT_DECLARE(Tiled);
ECS_COMPONENT_DECLARE(AnimatedSprite);

Sprite LoadWalkSprite() {
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

void GetPlayerAnimation(ecs_world_t *world) {
  const Tiled *tiled = ecs_singleton_get(world, Tiled);
  assert(tiled);
  tmx_tileset_list *tslist = tiled->ts_head;
  while (tslist) {
    if (strcmp("character", tslist->tileset->name) != 0) {
      tslist = tslist->next;
      continue;
    }
    tmx_tile *tiles = tslist->tileset->tiles;
    for (int i = 0; i < tslist->tileset->tilecount; i++) {
      if (!tiles[i].animation) {
        continue;
      }
      tmx_anim_frame *anim          = tslist->tileset->tiles[i].animation;
      uint32_t        num_frames    = tslist->tileset->tiles[i].animation_len;
      Sprite         *frames        = malloc(num_frames * sizeof(Sprite));
      AnimatedSprite  anim_resource = {.frames     = frames,
                                       .num_frames = num_frames};
      for (int j = 0; j < num_frames; j++) {
        uint32_t              gid        = anim[j].tile_id + tslist->firstgid;
        tmx_tile const *const frame_tile = tiled->tiles[gid];
        assert(frame_tile);
        frames[j].texture =
            *((Texture2D *)tslist->tileset->image->resource_image);
        frames[j].area.x      = frame_tile->ul_x;
        frames[j].area.y      = frame_tile->ul_y;
        frames[j].area.width  = frame_tile->width;
        frames[j].area.height = frame_tile->height;
      }

      // TODO: Load the frames somewhere.
      free(frames);
    }
    tslist = tslist->next;
  }
}

Vector2 GetPlayerStartPoint(ecs_world_t *world) {
  const Tiled *tiled = ecs_singleton_get(world, Tiled);
  assert(tiled);
  tmx_layer *layer = tiled->ly_head;
  assert(layer);
  while (layer) {
    if (layer->type != L_OBJGR) {
      layer = layer->next;
      continue;
    }
    tmx_object *obj = layer->content.objgr->head;
    while (obj) {
      if (obj->obj_type != OT_POINT) {
        obj = obj->next;
        continue;
      }
      Vector2 start = {.x = obj->x, .y = obj->y};
      return start;
    }
    layer = layer->next;
  }
  Vector2 start = {.x = 0, .y = 0};
  return start;
}

void ResourcesImport(ecs_world_t *world) {
  ECS_MODULE(world, Resources);
  ECS_COMPONENT_DEFINE(world, Sprite);
  ECS_COMPONENT_DEFINE(world, Tiled);
  ECS_COMPONENT_DEFINE(world, AnimatedSprite);
  LoadTiled(world);
  GetPlayerAnimation(world);
}
