#include "resources.h"
#include "arena.h"
#include "door.h"
#include "flecs/addons/flecs_c.h"
#include "tmx.h"
#include <raylib.h>
#include <stdlib.h>

ECS_COMPONENT_DECLARE(Sprite);
ECS_COMPONENT_DECLARE(Tiled);
ECS_COMPONENT_DECLARE(AnimatedSprite);
ECS_COMPONENT_DECLARE(ResourceTable);

static Arena ResourcesArena = {0};

static void *load_tiled_img(const char *path) {
  Texture2D *img = NULL;
  img = tmx_alloc_func(img, sizeof(Texture2D));
  if (img == NULL) {
    return img;
  }

  *img = LoadTexture(path);

  return img;
}

static void free_tiled_img(void *img) { tmx_free_func(img); }

static void LoadTiled(ecs_world_t *world) {
  tmx_img_load_func = load_tiled_img;
  tmx_img_free_func = free_tiled_img;
  tmx_map *test_room = tmx_load("./assets/test-room.tmx");
  if (test_room == NULL) {
    tmx_perror("Error loading test room: ");
    exit(1);
  }
  ecs_singleton_set_ptr(world, Tiled, test_room);
}

static void LoadRooms(ecs_world_t *const world) {
  ResourceTable *resource_table =
      ecs_get_mut(world, ecs_id(ResourceTable), ResourceTable);
  const Tiled *tiled = ecs_singleton_get(world, Tiled);
  assert(resource_table);
  assert(tiled);
  resource_table->width_tiles = tiled->width;
  resource_table->height_tiles = tiled->height;
  resource_table->floor_tiles = arena_alloc(
      &ResourcesArena, sizeof(Rectangle) * resource_table->width_tiles *
                           resource_table->height_tiles);
  resource_table->wall_tiles = arena_alloc(
      &ResourcesArena, sizeof(Rectangle) * resource_table->width_tiles *
                           resource_table->height_tiles);
  resource_table->door_tiles = arena_alloc(
      &ResourcesArena, sizeof(Rectangle) * resource_table->width_tiles *
                           resource_table->height_tiles);
  tmx_tileset const *const tileset =
      tmx_find_tileset_by_name(tiled, "haunted-house")->tileset;
  resource_table->haunted_house_tileset =
      *((Texture2D *)tileset->image->resource_image);
  tmx_layer const *const floor_layer = tmx_find_layer_by_id(tiled, 1);
  tmx_layer const *const walls_layer = tmx_find_layer_by_id(tiled, 2);
  tmx_layer const *const doors_layer = tmx_find_layer_by_id(tiled, 6);
  for (int x = 0; x < resource_table->width_tiles; x++) {
    for (int y = 0; y < resource_table->height_tiles; y++) {
      uint32_t const idx = x + y * resource_table->width_tiles;
      uint32_t floor_gid =
          floor_layer->content.gids[idx] & TMX_FLIP_BITS_REMOVAL;
      uint32_t wall_gid =
          walls_layer->content.gids[idx] & TMX_FLIP_BITS_REMOVAL;
      uint32_t door_gid =
          doors_layer->content.gids[idx] & TMX_FLIP_BITS_REMOVAL;
      if (floor_gid == 0) {
        resource_table->floor_tiles[idx] = (Rectangle){0};
      } else {
        tmx_tile const *const floor_tile = tiled->tiles[floor_gid];
        resource_table->floor_tiles[idx] = (Rectangle){
            .x = floor_tile->ul_x,
            .y = floor_tile->ul_y,
            .width = floor_tile->width,
            .height = floor_tile->height,
        };
      }
      if (wall_gid == 0) {
        resource_table->wall_tiles[idx] = (Rectangle){0};
      } else {
        tmx_tile const *const wall_tile = tiled->tiles[wall_gid];
        resource_table->wall_tiles[idx] = (Rectangle){
            .x = wall_tile->ul_x,
            .y = wall_tile->ul_y,
            .width = wall_tile->width,
            .height = wall_tile->height,
        };
      }
      if (door_gid == 0) {
        resource_table->door_tiles[idx] = (Rectangle){0};
      } else {
        tmx_tile const *const door_tile = tiled->tiles[door_gid];
        resource_table->door_tiles[idx] = (Rectangle){
            .x = door_tile->ul_x,
            .y = door_tile->ul_y,
            .width = door_tile->width,
            .height = door_tile->height,
        };
      }
    }
  }
}

static void LoadAnimations(ecs_world_t *const world) {
  static int call_count = 0;
  if (call_count == 0) {
    call_count++;
  } else {
    exit(666);
  }
  uint32_t num_sprites = 0;
  const Tiled *tiled = ecs_singleton_get(world, Tiled);
  ResourceTable *resource_table =
      ecs_get_mut(world, ecs_id(ResourceTable), ResourceTable);
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
      num_sprites = num_sprites + 1;
      tmx_anim_frame *anim = tslist->tileset->tiles[i].animation;
      uint32_t num_frames = tslist->tileset->tiles[i].animation_len;
      Rectangle *frames =
          arena_alloc(&ResourcesArena, num_frames * sizeof(Rectangle));
      uint32_t *durations =
          arena_alloc(&ResourcesArena, num_frames * sizeof(uint32_t));
      resource_table->animated_sprites[num_sprites - 1].texture =
          *((Texture2D *)tslist->tileset->image->resource_image);
      resource_table->animated_sprites[num_sprites - 1].frames = frames;
      resource_table->animated_sprites[num_sprites - 1].durations = durations;
      resource_table->animated_sprites[num_sprites - 1].num_frames = num_frames;
      resource_table->animated_sprites[num_sprites - 1].curr_frame = 0;
      resource_table->animated_sprites[num_sprites - 1].frame_time = 0;
      resource_table->animated_sprites[num_sprites - 1].paused = false;
      for (int j = 0; j < num_frames; j++) {
        uint32_t gid = anim[j].tile_id + tslist->firstgid;
        tmx_tile const *const frame_tile = tiled->tiles[gid];
        assert(frame_tile);
        frames[j].x = frame_tile->ul_x + tslist->tileset->x_offset;
        frames[j].y = frame_tile->ul_y + tslist->tileset->y_offset;
        frames[j].width = frame_tile->width - tslist->tileset->x_offset;
        frames[j].height = frame_tile->height - tslist->tileset->y_offset;
        durations[j] = anim[j].duration;
      }
    }
    tslist = tslist->next;
  }
}

static void LoadPlayerStartPoint(ecs_world_t *world) {
  const Tiled *tiled = ecs_singleton_get(world, Tiled);
  assert(tiled);
  ResourceTable *resource_table =
      ecs_get_mut(world, ecs_id(ResourceTable), ResourceTable);
  assert(resource_table);
  tmx_object *player_start = tmx_find_object_by_id(tiled, 1);
  assert(player_start);
  resource_table->player_start =
      (Vector2){.x = player_start->x, .y = player_start->y};
}

static void LoadDoors(ecs_world_t *world) {
  const Tiled *tiled = ecs_singleton_get(world, Tiled);
  assert(tiled);
  ResourceTable *resource_table =
      ecs_get_mut(world, ecs_id(ResourceTable), ResourceTable);
  assert(resource_table);

  tmx_layer const *const doors_layer = tmx_find_layer_by_id(tiled, 7);
  assert(doors_layer);
  tmx_object const *door_objs = doors_layer->content.objgr->head;
  assert(door_objs);

  const size_t alloc_size = sizeof(struct {
    Vector2 _;
    DoorTile __;
  });

  resource_table->doors = arena_alloc(&ResourcesArena, alloc_size);
  int door_count = 0;

  while (door_objs) {
    resource_table->doors[door_count].pos =
        (Vector2){.x = door_objs->x, .y = door_objs->y};
    int const to_id =
        tmx_get_property(door_objs->properties, "to")->value.object_id;
    tmx_object const *const to_obj = tmx_find_object_by_id(tiled, to_id);
    assert(to_obj);
    resource_table->doors[door_count].obj =
        (DoorTile){.to = (Vector2){.x = to_obj->x, .y = to_obj->y},
                   .locked = false,
                   .cooldown = 0};
    door_count++;
    resource_table->doors =
        arena_realloc(&ResourcesArena, resource_table->doors,
                      (door_count - 1) * alloc_size, door_count * alloc_size);

    door_objs = door_objs->next;
  }
  resource_table->num_doors = door_count;
  TraceLog(LOG_INFO, "NUMBER OF DOOR OBJECTS: %d", door_count);
}

void FreeResources() { arena_free(&ResourcesArena); }

void ResourcesImport(ecs_world_t *world) {
  ECS_MODULE(world, Resources);
  ECS_COMPONENT_DEFINE(world, Sprite);
  ECS_COMPONENT_DEFINE(world, Tiled);
  ECS_COMPONENT_DEFINE(world, AnimatedSprite);
  ECS_COMPONENT_DEFINE(world, ResourceTable);
  ecs_singleton_add(world, ResourceTable);
  LoadTiled(world);
  LoadAnimations(world);
  LoadRooms(world);
  LoadDoors(world);
  LoadPlayerStartPoint(world);
}
