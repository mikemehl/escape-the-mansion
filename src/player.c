#include "player.h"
#include "input.h"
#include "physics.h"
#include "raymath.h"
#include "render.h"
#include "resources.h"

ECS_TAG_DECLARE(PlayerTag);
ECS_SYSTEM_DECLARE(SystemPlayerUpdate);
ECS_QUERY_DECLARE(PlayerCollisionQuery);

static void SystemPlayerUpdate(ecs_iter_t *it) {
  Velocity *vel = ecs_field(it, Velocity, 0);
  assert(vel);
  const InputActions *input = ecs_singleton_get(it->world, InputActions);
  assert(input);

  vel->x = 0;
  vel->y = 0;

  if (input->up) {
    vel->y -= 1;
  } else if (input->down) {
    vel->y += 1;
  }
  if (input->right) {
    vel->x += 1;
  } else if (input->left) {
    vel->x -= 1;
  }

  *vel = Vector2Normalize(*vel);
}

void AddPlayer(ecs_world_t *world) {
  assert(world);
  ecs_entity_t player = ecs_new(world);
  ecs_add(world, player, PlayerTag);
  ecs_add(world, player, Position);
  ecs_add(world, player, Velocity);
  ecs_add(world, player, RectSprite);
  ecs_add(world, player, CollisionBox);
  ecs_add(world, player, CameraFollow);
  ecs_add(world, player, AnimatedSprite);

  Position *pos = ecs_get_mut(world, player, Position);
  assert(pos);
  *pos = GetPlayerStartPoint(world);

  Velocity *vel = ecs_get_mut(world, player, Velocity);
  assert(vel);
  vel->x = 0;
  vel->y = 0;

  RectSprite *rect = ecs_get_mut(world, player, RectSprite);
  assert(rect);
  rect->color             = GREEN;
  rect->dimensions.x      = pos->x;
  rect->dimensions.y      = pos->y;
  rect->dimensions.width  = 8;
  rect->dimensions.height = 10;

  CollisionBox *collision = ecs_get_mut(world, player, CollisionBox);
  assert(collision);
  collision->x      = pos->x;
  collision->y      = pos->y;
  collision->width  = 8;
  collision->height = 10;

  CameraFollow *camera = ecs_get_mut(world, player, CameraFollow);
  assert(camera);
  camera->zoom     = 4.0f;
  camera->target   = *pos;
  camera->rotation = 0.0;
  camera->offset.x = 640 / 2;
  camera->offset.y = 480 / 2;

  AnimatedSprite *anim_sprite = ecs_get_mut(world, player, AnimatedSprite);
  assert(anim_sprite);
  const ResourceTable *resource_table = ecs_singleton_get(world, ResourceTable);
  *anim_sprite = resource_table->animated_sprites[ANIM_PLAYER_WALK_FR];
}

void PlayerImport(ecs_world_t *world) {
  ECS_MODULE(world, Player);
  ECS_TAG_DEFINE(world, PlayerTag);
  ECS_QUERY_DEFINE(world, PlayerCollisionQuery, physics.Position,
                   physics.CollisionBox, !player.PlayerTag);
  ECS_SYSTEM_DEFINE(world, SystemPlayerUpdate, 0, physics.Velocity,
                    player.PlayerTag);
}
