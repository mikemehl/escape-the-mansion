#include "input.h"

ECS_COMPONENT_DECLARE(InputActions);

void InputImport(ecs_world_t *world) {
  ECS_MODULE(world, Input);
  ECS_COMPONENT_DEFINE(world, InputActions);
}
