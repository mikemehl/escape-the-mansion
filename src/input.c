#include "input.h"

ECS_COMPONENT_DECLARE(InputActions);
ECS_SYSTEM_DECLARE(SystemGatherInput);

static void SystemGatherInput(ecs_iter_t *it) {
  InputActions *input = ecs_field(it, InputActions, 0);
  assert(input);
  if (IsKeyDown(KEY_Q)) {
    CloseWindow();
    return;
  }
  input->up    = IsKeyDown(KEY_W);
  input->down  = IsKeyDown(KEY_S);
  input->right = IsKeyDown(KEY_D);
  input->left  = IsKeyDown(KEY_A);
}

void InputImport(ecs_world_t *world) {
  ECS_MODULE(world, Input);
  ECS_COMPONENT_DEFINE(world, InputActions);
  ecs_singleton_set(
      world, InputActions,
      {.up = false, .down = false, .left = false, .right = false});
  ECS_SYSTEM_DEFINE(world, SystemGatherInput, EcsOnUpdate,
                    input.InputActions($));
}
