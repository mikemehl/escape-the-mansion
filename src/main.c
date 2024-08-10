#include "flecs.h"
#include <assert.h>
#include <raylib.h>
#include <stdio.h>
#include <time.h>

#include "input.h"
#include "physics.h"
#include "render.h"
#include "resources.h"

ECS_TAG_DECLARE(Player);
ECS_SYSTEM_DECLARE(system_player_update);
ECS_QUERY_DECLARE(PlayerCollisionQuery);

ECS_SYSTEM_DECLARE(system_camera_draw_begin);
ECS_SYSTEM_DECLARE(system_camera_draw_end);
ECS_SYSTEM_DECLARE(system_camera_update);

static void window_init();

static ecs_world_t *world_init();

static void world_close(ecs_world_t *);
static void player_init(ecs_world_t *);
static void wall_init(ecs_world_t *);

static void system_player_update(ecs_iter_t *);
static void system_camera_update(ecs_iter_t *it);
static void system_camera_draw_begin(ecs_iter_t *it);
static void system_camera_draw_end(ecs_iter_t *it);

int main(void) {
  window_init();
  ecs_world_t *world = world_init();
  player_init(world);
  wall_init(world);
  while (!WindowShouldClose() && IsWindowReady()) {
    BeginDrawing();
    ClearBackground(GRAY);
    ecs_run(world, ecs_id(system_camera_draw_begin), 0, NULL);
    ecs_run(world, ecs_id(SystemDrawRectSprite), 0, NULL);
    ecs_run(world, ecs_id(SystemDrawSprite), 0, NULL);
    ecs_run(world, ecs_id(system_camera_draw_end), 0, NULL);
    EndDrawing();
    ecs_run(world, ecs_id(system_player_update), 0, NULL);
    ecs_run(world, ecs_id(system_camera_update), 0, NULL);
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
  ECS_TAG_DEFINE(world, Player);
  ECS_QUERY_DEFINE(world, PlayerCollisionQuery, physics.Position,
                   physics.CollisionBox, !Player);
  ECS_SYSTEM_DEFINE(world, system_player_update, 0, physics.Position, Player,
                    render.RectSprite, physics.CollisionBox);
  ECS_SYSTEM_DEFINE(world, system_camera_draw_begin, 0, render.CameraFollow);
  ECS_SYSTEM_DEFINE(world, system_camera_draw_end, 0, render.CameraFollow);
  ECS_SYSTEM_DEFINE(world, system_camera_update, 0, render.CameraFollow,
                    physics.Position);
  return world;
}

void world_close(ecs_world_t *world) {
  ecs_fini(world);
}

void player_init(ecs_world_t *world) {
  assert(world);
  ecs_entity_t player = ecs_new(world);
  ecs_add(world, player, Player);
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
  *sprite = load_walk_sprite();

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

void system_player_update(ecs_iter_t *it) {
  Position *pos = ecs_field(it, Position, 0);
  assert(pos);
  RectSprite *rs = ecs_field(it, RectSprite, 2);
  assert(rs);
  CollisionBox *cb = ecs_field(it, CollisionBox, 3);
  assert(cb);
  Position            new_pos = *pos;
  const InputActions *input   = ecs_singleton_get(it->world, InputActions);
  assert(input);
  if (input->up) {
    new_pos.y -= 1;
  } else if (input->down) {
    new_pos.y += 1;
  }

  if (input->right) {
    new_pos.x += 1;
  } else if (input->left) {
    new_pos.x -= 1;
  }

  bool collided               = false;
  cb->x                       = new_pos.x;
  cb->y                       = new_pos.y;
  ecs_query_t *collider_query = PlayerCollisionQuery;
  ecs_iter_t   collider_it    = ecs_query_iter(it->world, collider_query);

  while (ecs_query_next(&collider_it)) {
    CollisionBox *collider = ecs_field(&collider_it, CollisionBox, 1);
    if (CheckCollisionRecs(*cb, *collider)) {
      ecs_iter_fini(&collider_it);
      collided = true;
      break;
    }
  }

  if (!collided) {
    *pos = new_pos;
  }

  rs->dimensions.x = pos->x;
  rs->dimensions.y = pos->y;
  cb->x            = pos->x;
  cb->y            = pos->y;
}

void system_camera_draw_begin(ecs_iter_t *it) {
  CameraFollow *camera = ecs_field(it, CameraFollow, 0);
  assert(camera);
  assert(it->count == 1);
  BeginMode2D(*camera);
}

void system_camera_draw_end(ecs_iter_t *it) {
  CameraFollow *camera = ecs_field(it, CameraFollow, 0);
  assert(camera);
  assert(it->count == 1);
  EndMode2D();
}

void system_camera_update(ecs_iter_t *it) {
  CameraFollow *camera = ecs_field(it, CameraFollow, 0);
  Position     *pos    = ecs_field(it, Position, 1);

  for (int i = 0; i < it->count; i++) {
    camera[i].target = pos[i];
  }
}
