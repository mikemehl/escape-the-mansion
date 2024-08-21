#include "input.h"
#include "raylib.h"
#include <flecs.h>

ECS_COMPONENT_DECLARE(InputActions);
ECS_SYSTEM_DECLARE(SystemGatherInput);
ECS_SYSTEM_DECLARE(SystemCheckQuitGame);

static void SystemGatherInput(ecs_iter_t *it) {
  InputActions *input = ecs_field(it, InputActions, 0);
  assert(input);
  input->up = IsKeyDown(KEY_W);
  input->down = IsKeyDown(KEY_S);
  input->right = IsKeyDown(KEY_D);
  input->left = IsKeyDown(KEY_A);
  input->action = IsKeyDown(KEY_SPACE);
}

void InputImport(ecs_world_t *world) {
  ECS_MODULE(world, Input);
  ECS_COMPONENT_DEFINE(world, InputActions);
  ecs_singleton_set(world, InputActions,
                    {.up = false,
                     .down = false,
                     .left = false,
                     .right = false,
                     .action = false});
  ECS_SYSTEM_DEFINE(world, SystemGatherInput, EcsPreUpdate,
                    input.InputActions($));
}
