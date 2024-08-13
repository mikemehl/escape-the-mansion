#include "physics.h"
#include "flecs.h"
#include "flecs/addons/flecs_c.h"
#include <raylib.h>

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(CollisionBox);
ECS_SYSTEM_DECLARE(SystemApplyVelocity);
ECS_SYSTEM_DECLARE(SystemResolveCollisions);
ECS_QUERY_DECLARE(CollisionQuery);

static void SystemApplyVelocity(ecs_iter_t *it) {
  Position *pos = ecs_field(it, Position, 0);
  Velocity *vel = ecs_field(it, Velocity, 1);
  assert(pos);
  assert(vel);

  for (int i = 0; i < it->count; i++) {
    Position new_pos;
    new_pos.x = pos[i].x + vel[i].x;
    new_pos.y = pos[i].y + vel[i].y;
    pos[i].x  = new_pos.x;
    pos[i].y  = new_pos.y;
  }
}

static void SystemResolveCollisions(ecs_iter_t *it) {
  Position     *pos = ecs_field(it, Position, 0);
  Velocity     *vel = ecs_field(it, Velocity, 1);
  CollisionBox *box = ecs_field(it, CollisionBox, 2);
  assert(pos);
  assert(vel);
  assert(box);

  for (int i = 0; i < it->count; i++) {
    ecs_query_desc_t other_query_desc = {
        .terms =
            {
                    {.id = ecs_id(CollisionBox)},
                    },
    };
    ecs_query_t *other_query = ecs_query_init(it->world, &other_query_desc);
    ecs_iter_t   other_it    = ecs_query_iter(it->world, other_query);
    while (ecs_query_next(&other_it)) {
      CollisionBox *other_box = ecs_field(&other_it, CollisionBox, 0);
      assert(other_box);
      for (int j = 0; j < other_it.count; j++) {
        if (other_it.entities[j] == it->entities[i]) {
          continue;
        }
        if (CheckCollisionRecs(box[i], other_box[j])) {
          printf("%f %f %f %f vs. %f %f %f %f\n", box[i].x, box[i].y,
                 box[i].width, box[i].height, other_box[i].x, other_box[i].y,
                 other_box[i].width, other_box[i].height);
          pos[i].x -= vel[i].x;
          pos[i].y -= vel[i].y;
        }
      }
    }
    box[i].x = pos[i].x;
    box[i].y = pos[i].y;
    ecs_query_fini(other_query);
  }
}

void PhysicsImport(ecs_world_t *world) {
  ECS_MODULE(world, Physics);
  ECS_COMPONENT_DEFINE(world, Position);
  ECS_COMPONENT_DEFINE(world, Velocity);
  ECS_COMPONENT_DEFINE(world, CollisionBox);
  ECS_SYSTEM_DEFINE(world, SystemApplyVelocity, 0, Position, Velocity);
  ECS_SYSTEM_DEFINE(world, SystemResolveCollisions, 0, Position, Velocity,
                    CollisionBox);
  ECS_QUERY_DEFINE(world, CollisionQuery, CollisionBox);
}
