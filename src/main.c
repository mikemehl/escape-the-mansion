#include <assert.h>
#include <flecs.h>
#include <raylib.h>

#include "input.h"
#include "physics.h"
#include "player.h"
#include "render.h"
#include "resources.h"
#include "tmx.h"

#define TILED_WALL_LAYER 5

static void RaylibSetup();

static ecs_world_t *EcsSetup();

static void EcsTeardown(ecs_world_t *);
static void wall_init(ecs_world_t *);

/* static void AddWalls(ecs_world_t *world) { */
/*   const Tiled *tiled = ecs_singleton_get(world, Tiled); */
/*   tmx_layer *layer = tiled->ly_head; */
/*   while (layer && layer->id != TILED_WALL_LAYER) { */
/*     layer = layer->next; */
/*   } */
/*   assert(layer); */
/*   tmx_object_group *wallgroup = layer->content.objgr; */
/*   tmx_object *wall = wallgroup->head; */
/*   while (wall) { */
/*     if (wall->width > 0 && wall->height > 0) { */
/*       ecs_entity_t wall_id = ecs_new(world); */
/*       ecs_set(world, wall_id, Position, {.x = wall->x, .y = wall->y}); */
/*       ecs_set(world, wall_id, CollisionBox, */
/*               {.x = wall->x, */
/*                .y = wall->y, */
/*                .width = wall->width, */
/*                .height = wall->height}); */
/*     } */
/*     wall = wall->next; */
/*   } */
/* } */

static void AddWalls(ecs_world_t *world) {
  Tiled *tiled = ecs_get_mut(world, ecs_id(Tiled), Tiled);
  tmx_layer *wall_layer = tmx_find_layer_by_id(tiled, 2);
  assert(wall_layer);
  assert(wall_layer->type == L_LAYER);
  for (int x = 0; x < tiled->width; x++) {
    for (int y = 0; y < tiled->height; y++) {
      uint32_t gid = wall_layer->content.gids[x + y * tiled->width] &
                     TMX_FLIP_BITS_REMOVAL;
      if (gid == 0) {
        continue;
      }

      tmx_tile *tile = tiled->tiles[gid];
      assert(tile);

      ecs_entity_t wall = ecs_new(world);
      ecs_set(world, wall, Position,
              {.x = x * tiled->tile_width, .y = y * tiled->tile_height});
      ecs_set(world, wall, Sprite,
              {.texture = *((Texture *)tile->tileset->image->resource_image),
               .area = {.x = tile->ul_x,
                        .y = tile->ul_y,
                        .width = tile->width,
                        .height = tile->height}});
      if (tile->collision) {
        ecs_set(world, wall, CollisionBox,
                {.x = x * tiled->tile_width + tile->collision->x,
                 .y = y * tiled->tile_height + tile->collision->y,
                 .width = tile->collision->width,
                 .height = tile->collision->height});
      }
    }
  }
}

int main(void) {
  RaylibSetup();
  ecs_world_t *world = EcsSetup();
  AddPlayer(world);
  AddWalls(world);
  while (!WindowShouldClose()) {
    ecs_progress(world, GetFrameTime());
  }
  EcsTeardown(world);
  return 0;
}

void RaylibSetup() {
  InitWindow(640, 480, "escape-the-mansion");
  SetTargetFPS(60);
  SetExitKey(KEY_Q);
}

ecs_world_t *EcsSetup() {
  ecs_world_t *world = ecs_init();
  ECS_IMPORT(world, Resources);
  ECS_IMPORT(world, Physics);
  ECS_IMPORT(world, Render);
  ECS_IMPORT(world, Input);
  ECS_IMPORT(world, Player);
  ECS_IMPORT(world, FlecsStats);
  ecs_singleton_set(world, EcsRest, {0});
  ecs_set_target_fps(world, 60);
  return world;
}

void EcsTeardown(ecs_world_t *world) {
  ecs_fini(world);
  FreeResources();
}
