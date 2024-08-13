#include "flecs.h"
#include <assert.h>
#include <raylib.h>
#include <stdio.h>
#include <time.h>

#include "input.h"
#include "physics.h"
#include "player.h"
#include "render.h"
#include "resources.h"
#include "tmx.h"

#define TILED_WALL_LAYER 5

static void window_init();

static ecs_world_t *world_init();

static void world_close(ecs_world_t *);
static void wall_init(ecs_world_t *);

static void AddWalls(ecs_world_t *world) {
  const Tiled *tiled = ecs_singleton_get(world, Tiled);
  tmx_layer   *layer = tiled->ly_head;
  while (layer && layer->id != TILED_WALL_LAYER) {
    layer = layer->next;
  }
  assert(layer);
  tmx_object_group *wallgroup = layer->content.objgr;
  tmx_object       *wall      = wallgroup->head;
  while (wall) {
    if (wall->width > 0 && wall->height > 0) {
      ecs_entity_t wall_id = ecs_new(world);
      ecs_add(world, wall_id, Position);
      ecs_add(world, wall_id, CollisionBox);

      Position *pos = ecs_get_mut(world, wall_id, Position);
      assert(pos);
      pos->x = wall->x;
      pos->y = wall->y;

      CollisionBox *collision = ecs_get_mut(world, wall_id, CollisionBox);
      assert(collision);
      collision->x      = wall->x;
      collision->y      = wall->y;
      collision->width  = wall->width;
      collision->height = wall->height;
    }
    wall = wall->next;
  }
}

int main(void) {
  window_init();
  ecs_world_t *world = world_init();
  AddPlayer(world);
  AddWalls(world);
  while (!WindowShouldClose() && IsWindowReady()) {
    BeginDrawing();
    ClearBackground(BLACK);
    ecs_run(world, ecs_id(SystemCameraDrawBegin), 0, NULL);
    ecs_run(world, ecs_id(SystemDrawRoom), 0, NULL);
    ecs_run(world, ecs_id(SystemDrawRectSprite), 0, NULL);
    ecs_run(world, ecs_id(SystemDrawSprite), 0, NULL);
    ecs_run(world, ecs_id(SystemDrawAnimatedSprite), 0, NULL);
    ecs_run(world, ecs_id(SystemCameraDrawEnd), 0, NULL);
    EndDrawing();
    ecs_run(world, ecs_id(SystemGatherInput), 0, NULL);
    ecs_run(world, ecs_id(SystemPlayerUpdate), 0, NULL);
    ecs_run(world, ecs_id(SystemApplyVelocity), 0, NULL);
    ecs_run(world, ecs_id(SystemResolveCollisions), 0, NULL);
    ecs_run(world, ecs_id(SystemCameraUpdate), 0, NULL);
  }
  world_close(world);
  return 0;
}

void window_init() {
  InitWindow(640, 480, "escape-the-mansion");
  SetTargetFPS(60);
}

ecs_world_t *world_init() {
  ecs_world_t *world = ecs_init();
  ECS_IMPORT(world, Resources);
  ECS_IMPORT(world, Physics);
  ECS_IMPORT(world, Render);
  ECS_IMPORT(world, Input);
  ECS_IMPORT(world, Player);
  return world;
}

void world_close(ecs_world_t *world) {
  ecs_fini(world);
  FreeResources();
}
