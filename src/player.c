#include "player.h"
#include "input.h"
#include "physics.h"
#include "render.h"

ECS_TAG_DECLARE(PlayerTag);
ECS_SYSTEM_DECLARE(SystemPlayerUpdate);
ECS_QUERY_DECLARE(PlayerCollisionQuery);

static void SystemPlayerUpdate(ecs_iter_t *it) {
  Position *pos = ecs_field(it, Position, 0);
  assert(pos);
  CollisionBox *cb = ecs_field(it, CollisionBox, 2);
  assert(cb);
  Position            new_pos = *pos;
  const InputActions *input   = ecs_singleton_get(it->world, InputActions);
  assert(input);
  if (input->up) {
    new_pos.y -= 1;
  } else if (input->down) {
    new_pos.y += 1;
  }

  if (input->right) {
    new_pos.x += 1;
  } else if (input->left) {
    new_pos.x -= 1;
  }

  bool collided               = false;
  cb->x                       = new_pos.x;
  cb->y                       = new_pos.y;
  ecs_query_t *collider_query = PlayerCollisionQuery;
  ecs_iter_t   collider_it    = ecs_query_iter(it->world, collider_query);

  while (ecs_query_next(&collider_it) && !collided) {
    CollisionBox *collider = ecs_field(&collider_it, CollisionBox, 1);
    assert(collider);
    for (int i = 0; i < collider_it.count; i++) {
      if (CheckCollisionRecs(*cb, collider[i])) {
        ecs_iter_fini(&collider_it);
        collided = true;
        break;
      }
    }
    if (collided) {
      break;
    }
  }

  if (!collided) {
    *pos = new_pos;
  }

  cb->x = pos->x;
  cb->y = pos->y;
}

void PlayerImport(ecs_world_t *world) {
  ECS_MODULE(world, Player);
  ECS_TAG_DEFINE(world, PlayerTag);
  ECS_QUERY_DEFINE(world, PlayerCollisionQuery, physics.Position,
                   physics.CollisionBox, !player.PlayerTag);
  ECS_SYSTEM_DEFINE(world, SystemPlayerUpdate, 0, physics.Position,
                    player.PlayerTag, physics.CollisionBox);
}
