#ifndef __PHYSICS__H__
#define __PHYSICS__H__
#include "flecs.h"
#include <raylib.h>

typedef Vector2   Position;
typedef Vector2   Velocity;
typedef Rectangle CollisionBox;

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(CollisionBox);
extern ECS_SYSTEM_DECLARE(SystemApplyVelocity);
extern ECS_SYSTEM_DECLARE(SystemCollisionDetect);

void PhysicsImport(ecs_world_t *world);
#endif
