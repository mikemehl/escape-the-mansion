#include "player.h"
#include "flecs/addons/flecs_c.h"
#include "input.h"
#include "physics.h"
#include "raymath.h"
#include "render.h"
#include "resources.h"

ECS_TAG_DECLARE(PlayerTag);
ECS_SYSTEM_DECLARE(SystemPlayerMove);
ECS_SYSTEM_DECLARE(SystemPlayerSpriteUpdate);
ECS_QUERY_DECLARE(PlayerCollisionQuery);

static void SystemPlayerMove(ecs_iter_t *it) {
  Velocity           *vel    = ecs_field(it, Velocity, 0);
  Facing             *facing = ecs_field(it, Facing, 2);
  const InputActions *input  = ecs_singleton_get(it->world, InputActions);
  assert(vel);
  assert(facing);
  assert(input);

  vel->x    = input->right ? 1 : (input->left ? -1 : 0);
  vel->y    = input->down ? 1 : (input->up ? -1 : 0);
  facing->x = input->right ? 1 : (input->left ? -1 : facing->x);
  facing->y = input->down ? 1 : (input->up ? -1 : facing->y);
  *vel      = Vector2Normalize(*vel);
}

static void SystemPlayerSpriteUpdate(ecs_iter_t *it) {
  Facing              *facing    = ecs_field(it, Facing, 0);
  AnimatedSprite      *sprite    = ecs_field(it, AnimatedSprite, 1);
  Velocity            *vel       = ecs_field(it, Velocity, 2);
  const ResourceTable *resources = ecs_singleton_get(it->world, ResourceTable);
  assert(facing);
  assert(sprite);
  assert(vel);
  assert(resources);

  if (vel->x == 0 && vel->y == 0) {
    sprite->paused     = true;
    sprite->curr_frame = 0;
    sprite->frame_time = 0;
  } else {
    sprite->paused = false;
    AnimatedSpriteIndex idx =
        facing->x > 0 && facing->y > 0   ? ANIM_PLAYER_WALK_FR
        : facing->x < 0 && facing->y > 0 ? ANIM_PLAYER_WALK_FL
        : facing->x > 0 && facing->y < 0 ? ANIM_PLAYER_WALK_BR
                                         : ANIM_PLAYER_WALK_BL;
    AnimatedSprite new_sprite = resources->animated_sprites[idx];
    sprite->frames            = new_sprite.frames;
    sprite->num_frames        = new_sprite.num_frames;
    if (sprite->curr_frame > sprite->num_frames) {
      sprite->curr_frame = 0;
    }
    sprite->durations = new_sprite.durations;
    if (sprite->frame_time > sprite->durations[sprite->curr_frame]) {
      sprite->frame_time = 0;
    }
  }
}

void AddPlayer(ecs_world_t *world) {
  assert(world);
  ecs_entity_t player = ecs_new(world);
  ecs_add(world, player, PlayerTag);
  ecs_add(world, player, Position);
  ecs_add(world, player, Velocity);
  ecs_add(world, player, Facing);
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

  Facing *facing = ecs_get_mut(world, player, Facing);
  assert(facing);
  facing->x = 1;
  facing->y = -1;

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
  ECS_SYSTEM_DEFINE(world, SystemPlayerMove, 0, physics.Velocity,
                    player.PlayerTag, physics.Facing);
  ECS_SYSTEM_DEFINE(world, SystemPlayerSpriteUpdate, 0, physics.Facing,
                    resources.AnimatedSprite, physics.Velocity,
                    player.PlayerTag);
}
