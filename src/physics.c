#include "physics.h"
#include "resources.h"
#include <raylib.h>

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(CollisionBox);
ECS_SYSTEM_DECLARE(SystemApplyVelocity);
ECS_SYSTEM_DECLARE(SystemResolveCollisions);

static void SystemApplyVelocity(ecs_iter_t *it) {
  Position *pos = ecs_field(it, Position, 0);
  Velocity *vel = ecs_field(it, Velocity, 0);
  assert(pos);
  assert(vel);

  for (int i = 0; i < it->count; i++) {
    pos[i].x = pos[i].x + vel[i].x;
    pos[i].y = pos[i].y + vel[i].y;
  }
}

static void SystemResolveCollisions(ecs_iter_t *it) {
  Position     *pos = ecs_field(it, Position, 0);
  CollisionBox *box = ecs_field(it, CollisionBox, 1);
  assert(pos);
  assert(box);

  for (int i = 0; i < it->count; i++) {
    for (int j = 0; j < it->count; j++) {
      if (i == j) {
        continue;
      }
      if (CheckCollisionRecs(box[i], box[j])) {
        Rectangle collision_rec = GetCollisionRec(box[i], box[j]);
        pos[i].x                = pos[i].x - collision_rec.width;
        pos[i].y                = pos[i].y - collision_rec.height;
      }
    }
  }
}

void PhysicsImport(ecs_world_t *world) {
  ECS_MODULE(world, Physics);
  ECS_COMPONENT_DEFINE(world, Position);
  ECS_COMPONENT_DEFINE(world, Velocity);
  ECS_COMPONENT_DEFINE(world, CollisionBox);
  ECS_SYSTEM_DEFINE(world, SystemApplyVelocity, 0, Position, Velocity);
  ECS_SYSTEM_DEFINE(world, SystemResolveCollisions, 0, Position, CollisionBox);
}
