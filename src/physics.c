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
ECS_COMPONENT_DECLARE(CollidingWith);
ECS_SYSTEM_DECLARE(SystemApplyVelocity);
ECS_SYSTEM_DECLARE(SystemCollisionDetect);
ECS_SYSTEM_DECLARE(SystemCollisionResolve);
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
          CollidingWith collision = GetCollisionRec(test_box_i, test_box_j);
          ecs_add_pair(it->world, it->entities[i], ecs_id(CollidingWith),
                       other_it.entities[j]);
        }
      }
    }
    ecs_query_fini(other_query);
  }
}

static void SystemCollisionResolve(ecs_iter_t *it) {
  CollidingWith *collisions = ecs_field(it, CollidingWith, 0);
  for (int i = 0; i < it->count; i++) {
    ecs_entity_t pair = ecs_field_id(it, i);
    ecs_entity_t target = ecs_pair_second(it->world, pair);
    if (!ecs_has(it->world, it->entities[i], Velocity)) {
      ecs_remove(it->world, it->entities[i], CollidingWith);
      continue;
    }

    const Position *pos = ecs_get(it->world, it->entities[i], Position);
    Velocity *vel = ecs_get_mut(it->world, it->entities[i], Velocity);
    const CollisionBox *box = ecs_get(it->world, it->entities[i], CollisionBox);
    assert(pos);
    assert(vel);
    assert(box);

    Vector2 box_center = (Vector2){.x = pos->x + box->x + box->width / 2,
                                   .y = pos->y + box->y + box->height / 2};
    Vector2 collision_center =
        (Vector2){.x = collisions[i].x + collisions[i].width / 2,
                  .y = collisions[i].y + collisions[i].height / 2};
    float center_distance = Vector2Distance(box_center, collision_center);
    Vector2 collision_direction = Vector2Subtract(collision_center, box_center);
    collision_direction = Vector2Normalize(collision_direction);
    Vector2 collision_force =
        Vector2Scale(collision_direction, center_distance);

    float velocity_magnitude = Vector2Length(*vel);
    *vel = Vector2Add(*vel, collision_force);
    *vel = Vector2Normalize(*vel);
    *vel = Vector2Scale(*vel, -1 * velocity_magnitude);
    /* ecs_remove_id(it->world, it->entities[i], pair); */
    ecs_remove_pair(it->world, it->entities[i], ecs_id(CollidingWith), target);
  }
}

void PhysicsImport(ecs_world_t *world) {
  ECS_MODULE(world, Physics);

  // phases
  ecs_entity_t PhysicsCollisionDetect = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t PhysicsCollisionResolve = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t PhysicsApply = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t PhysicsCleanup = ecs_new_w_id(world, EcsPhase);

  ecs_add_pair(world, PhysicsCollisionDetect, EcsDependsOn, EcsOnUpdate);
  ecs_add_pair(world, PhysicsCollisionResolve, EcsDependsOn,
               PhysicsCollisionDetect);
  ecs_add_pair(world, PhysicsApply, EcsDependsOn, PhysicsCollisionResolve);
  ecs_add_pair(world, PhysicsCleanup, EcsDependsOn, PhysicsApply);

  ECS_COMPONENT_DEFINE(world, Position);
  ECS_COMPONENT_DEFINE(world, Velocity);
  ECS_COMPONENT_DEFINE(world, Facing);
  ECS_COMPONENT_DEFINE(world, CollisionBox);
  ECS_COMPONENT_DEFINE(world, CollidingWith);
  ECS_SYSTEM_DEFINE(world, SystemApplyVelocity, PhysicsApply,
                    Position, [in] Velocity);
  ECS_SYSTEM_DEFINE(world, SystemCollisionDetect, PhysicsCollisionDetect,
                    Position, Velocity, CollisionBox);
  ECS_SYSTEM_DEFINE(world, SystemCollisionResolve, PhysicsCollisionResolve,
                    (CollidingWith, *), [out] Velocity);
  ECS_QUERY_DEFINE(world, CollisionQuery, CollisionBox);
}
