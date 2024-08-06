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

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(RectSprite);
extern ECS_COMPONENT_DECLARE(CollisionBox);

static void window_init();
static void window_close();
static ecs_world_t *world_init();
static void world_close(ecs_world_t *);

int main(void) {
  ecs_world_t *world = world_init();
  window_init();
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RED);
    EndDrawing();
    if (GetKeyPressed() == KEY_Q) {
      break;
    }
  }
  window_close();
  world_close(world);
  return 0;
}

void window_init() { InitWindow(640, 480, "escape-the-mansion"); }
void window_close() { CloseWindow(); }
ecs_world_t *world_init() {
  ecs_world_t *world = ecs_init();
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, Velocity);
  ECS_COMPONENT(world, RectSprite);
  ECS_COMPONENT(world, CollisionBox);
  return world;
}
void world_close(ecs_world_t *world) { ecs_fini(world); }
