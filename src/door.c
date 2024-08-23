#include "door.h"
#include "flecs/addons/flecs_c.h"
#include "flecs/private/addons.h"

ECS_COMPONENT_DECLARE(DoorTile);

void DoorImport(ecs_world_t *world) {
  ECS_MODULE(world, Door);
  ECS_COMPONENT_DEFINE(world, DoorTile);
}
