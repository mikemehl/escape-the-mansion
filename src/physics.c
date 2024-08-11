#include "physics.h"
#include "resources.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(CollisionBox);

void PhysicsImport(ecs_world_t *world) {
  ECS_MODULE(world, Physics);
  ECS_COMPONENT_DEFINE(world, Position);
  ECS_COMPONENT_DEFINE(world, Velocity);
  ECS_COMPONENT_DEFINE(world, CollisionBox);
}
