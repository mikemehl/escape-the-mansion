#include "flecs.h"
#include <raylib.h>
#include <stdio.h>

typedef Vector2 Position;
typedef Vector2 Velocity;
typedef struct RectSprite {
  Vector2 dimensions;
  Color color;
} RectSprite;
typedef Vector2 CollisionBox;
typedef struct Input {
  bool up;
  bool left;
  bool down;
  bool right;
} Input;

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(RectSprite);
ECS_COMPONENT_DECLARE(CollisionBox);
ECS_COMPONENT_DECLARE(Input);
ECS_TAG_DECLARE(Player);
ECS_SYSTEM_DECLARE(system_gather_input);
ECS_SYSTEM_DECLARE(system_rectsprite_draw);
ECS_SYSTEM_DECLARE(system_player_update);

static void window_init();
static ecs_world_t *world_init();
static void world_close(ecs_world_t *);
static void player_init(ecs_world_t *);
static void system_rectsprite_draw(ecs_iter_t *);
static void system_player_update(ecs_iter_t *);
static void system_gather_input(ecs_iter_t *);

int main(void) {
  ecs_world_t *world = world_init();
  window_init();
  player_init(world);
  while (!WindowShouldClose() && IsWindowReady()) {
    BeginDrawing();
    ClearBackground(RED);
    ecs_run(world, ecs_id(system_rectsprite_draw), 0, NULL);
    EndDrawing();
    ecs_run(world, ecs_id(system_player_update), 0, NULL);
    ecs_run(world, ecs_id(system_gather_input), 0, NULL);
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
  ECS_COMPONENT_DEFINE(world, Position);
  ECS_COMPONENT_DEFINE(world, Velocity);
  ECS_COMPONENT_DEFINE(world, RectSprite);
  ECS_COMPONENT_DEFINE(world, CollisionBox);
  ECS_COMPONENT_DEFINE(world, Input);
  ecs_singleton_set(
      world, Input,
      {.up = false, .down = false, .left = false, .right = false});
  ECS_TAG_DEFINE(world, Player);
  return world;
}
void world_close(ecs_world_t *world) { ecs_fini(world); }

void player_init(ecs_world_t *world) {
  assert(world);
  ecs_entity_t player = ecs_new(world);
  ecs_add(world, player, Player);
  ecs_add(world, player, Position);
  ecs_add(world, player, RectSprite);

  Position *pos = ecs_get_mut(world, player, Position);
  assert(pos);
  pos->x = 640 / 2;
  pos->y = 480 / 2;

  RectSprite *rect = ecs_get_mut(world, player, RectSprite);
  assert(rect);
  rect->color = GREEN;
  rect->dimensions.x = 50;
  rect->dimensions.y = 50;

  ECS_SYSTEM_DEFINE(world, system_gather_input, EcsOnUpdate, Input($));
  ECS_SYSTEM_DEFINE(world, system_rectsprite_draw, 0, Position, RectSprite);
  ECS_SYSTEM_DEFINE(world, system_player_update, 0, Position, Player);
}

static void system_rectsprite_draw(ecs_iter_t *it) {
  Position *p = ecs_field(it, Position, 0);
  RectSprite *r = ecs_field(it, RectSprite, 1);
  assert(p);
  assert(r);

  for (int i = 0; i < it->count; i++) {
    DrawRectangle(p[i].x, p[i].y, r[i].dimensions.x, r[i].dimensions.y,
                  r[i].color);
  }
}

static void system_player_update(ecs_iter_t *it) {
  Position *pos = ecs_field(it, Position, 0);
  assert(pos);
  const Input *input = ecs_singleton_get(it->world, Input);
  assert(input);
  if (input->up) {
    pos->y -= 1;
  } else if (input->down) {
    pos->y += 1;
  }

  if (input->right) {
    pos->x += 1;
  } else if (input->left) {
    pos->x -= 1;
  }
}

static void system_gather_input(ecs_iter_t *it) {
  Input *input = ecs_field(it, Input, 0);
  assert(input);
  if (IsKeyDown(KEY_Q)) {
    CloseWindow();
    return;
  }
  input->up = IsKeyDown(KEY_W);
  input->down = IsKeyDown(KEY_S);
  input->right = IsKeyDown(KEY_D);
  input->left = IsKeyDown(KEY_A);
}
