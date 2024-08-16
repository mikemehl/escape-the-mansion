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

static void AddWalls(ecs_world_t *world) {
  const Tiled *tiled = ecs_singleton_get(world, Tiled);
  tmx_layer *layer = tiled->ly_head;
  while (layer && layer->id != TILED_WALL_LAYER) {
    layer = layer->next;
  }
  assert(layer);
  tmx_object_group *wallgroup = layer->content.objgr;
  tmx_object *wall = wallgroup->head;
  while (wall) {
    if (wall->width > 0 && wall->height > 0) {
      ecs_entity_t wall_id = ecs_new(world);
      ecs_set(world, wall_id, Position, {.x = wall->x, .y = wall->y});
      ecs_set(world, wall_id, CollisionBox,
              {.x = wall->x,
               .y = wall->y,
               .width = wall->width,
               .height = wall->height});
    }
    wall = wall->next;
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
