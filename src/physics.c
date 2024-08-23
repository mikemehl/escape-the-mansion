#include "physics.h"
#include "flecs/addons/flecs_c.h"
#include "flecs/private/api_defines.h"
#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(Facing);
ECS_COMPONENT_DECLARE(CollisionBox);
ECS_SYSTEM_DECLARE(SystemApplyVelocity);
ECS_SYSTEM_DECLARE(SystemCollisionDetect);
ECS_SYSTEM_DECLARE(SystemUpdateCollisionBoxPositions);
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
    pos[i].x = new_pos.x;
    pos[i].y = new_pos.y;
  }
}

static void SystemCollisionDetect(ecs_iter_t *it) {
  Position *pos = ecs_field(it, Position, 0);
  Velocity *vel = ecs_field(it, Velocity, 1);
  CollisionBox *box = ecs_field(it, CollisionBox, 2);
  assert(pos);
  assert(vel);
  assert(box);

  for (int i = 0; i < it->count; i++) {
    ecs_query_desc_t other_query_desc = {
        .terms =
            {
                {.id = ecs_id(CollisionBox)},
                {.id = ecs_id(Position)},
            },
    };
    ecs_query_t *other_query = ecs_query_init(it->world, &other_query_desc);
    ecs_iter_t other_it = ecs_query_iter(it->world, other_query);
    while (ecs_query_next(&other_it)) {
      CollisionBox *other_box = ecs_field(&other_it, CollisionBox, 0);
      Position *other_pos = ecs_field(&other_it, Position, 1);
      assert(other_box);
      assert(other_pos);

      for (int j = 0; j < other_it.count; j++) {
        if (other_it.entities[j] == it->entities[i]) {
          continue;
        }

        CollisionBox test_box_i =
            (CollisionBox){.x = pos[i].x + vel[i].x + box[i].x,
                           .y = pos[i].y + vel[i].y + box[i].y,
                           .width = box[i].width,
                           .height = box[i].height};
        CollisionBox test_box_j =
            (CollisionBox){.x = other_pos[j].x + other_box[j].x,
                           .y = other_pos[j].y + other_box[j].y,
                           .width = other_box[j].width,
                           .height = other_box[j].height};

        if (CheckCollisionRecs(test_box_i, test_box_j)) {
          vel[i].x *= -1;
          test_box_i.x = pos[i].x + vel[i].x + box[i].x;
          if (CheckCollisionRecs(test_box_i, test_box_j)) {
            vel[i].y *= -1;
            test_box_i.y = pos[i].y + vel[i].y + box[i].y;
          }
        }
      }
    }
    ecs_query_fini(other_query);
  }
}

static void SystemCollisionResolve(ecs_iter_t *it) {
  for (int i = 0; i < it->count; i++) {
    ecs_entity_t pair = it->entities[i];
    ecs_entity_t first = ecs_pair_first(it->world, pair);
    ecs_entity_t second = ecs_pair_second(it->world, pair);
    assert(first > 0);
    assert(second > 0);
    const CollisionBox *first_collider =
        ecs_get(it->world, first, CollisionBox);
    const CollisionBox *second_collider =
        ecs_get(it->world, second, CollisionBox);
    assert(first_collider);
    assert(second_collider);
    Position *first_pos = ecs_get_mut(it->world, first, Position);
    const Position *second_pos = ecs_get(it->world, second, Position);
    assert(first_pos);
    assert(second_pos);

    Position a_pos = *first_pos;
    Position b_pos = *second_pos;
    const Velocity *a_vel = ecs_get(it->world, first, Velocity);
    if (a_vel != NULL) {
      a_pos = Vector2Add(a_pos, *a_vel);
    }

    // TODO: Get collision rectangle and adjust position to not collide.
  }
}

void PhysicsImport(ecs_world_t *world) {
  ECS_MODULE(world, Physics);

  // phases
  ecs_entity_t PhysicsCollision = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t PhysicsApply = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t PhysicsCleanup = ecs_new_w_id(world, EcsPhase);

  ecs_add_pair(world, PhysicsCollision, EcsDependsOn, EcsOnUpdate);
  ecs_add_pair(world, PhysicsApply, EcsDependsOn, PhysicsCollision);
  ecs_add_pair(world, PhysicsCleanup, EcsDependsOn, PhysicsApply);

  ECS_COMPONENT_DEFINE(world, Position);
  ECS_COMPONENT_DEFINE(world, Velocity);
  ECS_COMPONENT_DEFINE(world, Facing);
  ECS_COMPONENT_DEFINE(world, CollisionBox);
  ECS_SYSTEM_DEFINE(world, SystemApplyVelocity, EcsPreStore, Position,
                    Velocity);
  ECS_SYSTEM_DEFINE(world, SystemCollisionDetect, EcsPostUpdate, Position,
                    Velocity, CollisionBox);
  ECS_QUERY_DEFINE(world, CollisionQuery, CollisionBox);
}
