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

static void window_init();

static ecs_world_t *world_init();

static void world_close(ecs_world_t *);
static void player_init(ecs_world_t *);
static void wall_init(ecs_world_t *);

int main(void) {
  window_init();
  ecs_world_t *world = world_init();
  player_init(world);
  wall_init(world);
  while (!WindowShouldClose() && IsWindowReady()) {
    BeginDrawing();
    ClearBackground(GRAY);
    ecs_run(world, ecs_id(SystemCameraDrawBegin), 0, NULL);
    ecs_run(world, ecs_id(SystemDrawRoom), 0, NULL);
    ecs_run(world, ecs_id(SystemDrawRectSprite), 0, NULL);
    ecs_run(world, ecs_id(SystemDrawSprite), 0, NULL);
    ecs_run(world, ecs_id(SystemCameraDrawEnd), 0, NULL);
    EndDrawing();
    ecs_run(world, ecs_id(SystemPlayerUpdate), 0, NULL);
    ecs_run(world, ecs_id(SystemCameraUpdate), 0, NULL);
    ecs_run(world, ecs_id(SystemGatherInput), 0, NULL);
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
  ECS_IMPORT(world, Physics);
  ECS_IMPORT(world, Resources);
  ECS_IMPORT(world, Render);
  ECS_IMPORT(world, Input);
  ECS_IMPORT(world, Player);
  return world;
}

void world_close(ecs_world_t *world) {
  ecs_fini(world);
}

void player_init(ecs_world_t *world) {
  assert(world);
  ecs_entity_t player = ecs_new(world);
  ecs_add(world, player, PlayerTag);
  ecs_add(world, player, Position);
  ecs_add(world, player, RectSprite);
  ecs_add(world, player, CollisionBox);
  ecs_add(world, player, Sprite);
  ecs_add(world, player, CameraFollow);

  Position *pos = ecs_get_mut(world, player, Position);
  assert(pos);
  *pos = GetPlayerStartPoint(world);

  RectSprite *rect = ecs_get_mut(world, player, RectSprite);
  assert(rect);
  rect->color             = GREEN;
  rect->dimensions.x      = pos->x;
  rect->dimensions.y      = pos->y;
  rect->dimensions.width  = 8;
  rect->dimensions.height = 10;

  CollisionBox *collision = ecs_get_mut(world, player, CollisionBox);
  assert(collision);
  *collision = (rect->dimensions);

  Sprite *sprite = ecs_get_mut(world, player, Sprite);
  assert(sprite);
  *sprite = LoadWalkSprite();

  CameraFollow *camera = ecs_get_mut(world, player, CameraFollow);
  assert(camera);
  camera->zoom     = 4.0f;
  camera->target   = *pos;
  camera->rotation = 0.0;
  camera->offset.x = 640 / 2;
  camera->offset.y = 480 / 2;
}

void wall_init(ecs_world_t *world) {
  assert(world);
  ecs_entity_t wall = ecs_new(world);
  ecs_add(world, wall, Position);
  ecs_add(world, wall, RectSprite);
  ecs_add(world, wall, CollisionBox);

  Position *pos = ecs_get_mut(world, wall, Position);
  assert(pos);
  pos->x = 192;
  pos->y = 96 - 64;

  RectSprite *rect = ecs_get_mut(world, wall, RectSprite);
  assert(rect);
  rect->color             = BLUE;
  rect->dimensions.x      = pos->x;
  rect->dimensions.y      = pos->y;
  rect->dimensions.width  = 150;
  rect->dimensions.height = 4;

  CollisionBox *collision = ecs_get_mut(world, wall, CollisionBox);
  assert(collision);
  *collision = rect->dimensions;
}
