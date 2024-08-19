#include "player.h"
#include "flecs/addons/flecs_c.h"
#include "input.h"
#include "physics.h"
#include "raylib.h"
#include "raymath.h"
#include "render.h"
#include "resources.h"

ECS_TAG_DECLARE(PlayerTag);
ECS_SYSTEM_DECLARE(SystemPlayerMove);
ECS_SYSTEM_DECLARE(SystemPlayerSpriteUpdate);
ECS_SYSTEM_DECLARE(SystemPlayerUpdateShader);
ECS_QUERY_DECLARE(PlayerCollisionQuery);

static void SystemPlayerMove(ecs_iter_t *it) {
  Velocity *vel = ecs_field(it, Velocity, 0);
  Facing *facing = ecs_field(it, Facing, 2);
  const InputActions *input = ecs_singleton_get(it->world, InputActions);
  assert(vel);
  assert(facing);
  assert(input);

  vel->x = input->right ? 1 : (input->left ? -1 : 0);
  vel->y = input->down ? 1 : (input->up ? -1 : 0);
  facing->x = input->right ? 1 : (input->left ? -1 : facing->x);
  facing->y = input->down ? 1 : (input->up ? -1 : facing->y);
  *vel = Vector2Normalize(*vel);
}

static void SystemPlayerSpriteUpdate(ecs_iter_t *it) {
  Facing *facing = ecs_field(it, Facing, 0);
  AnimatedSprite *sprite = ecs_field(it, AnimatedSprite, 1);
  Velocity *vel = ecs_field(it, Velocity, 2);
  const ResourceTable *resources = ecs_singleton_get(it->world, ResourceTable);
  assert(facing);
  assert(sprite);
  assert(vel);
  assert(resources);

  if (vel->x == 0 && vel->y == 0) {
    sprite->paused = true;
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
    sprite->frames = new_sprite.frames;
    sprite->num_frames = new_sprite.num_frames;
    if (sprite->curr_frame > sprite->num_frames) {
      sprite->curr_frame = 0;
    }
    sprite->durations = new_sprite.durations;
    if (sprite->frame_time > sprite->durations[sprite->curr_frame]) {
      sprite->frame_time = 0;
    }
  }
}

static void SystemPlayerUpdateShader(ecs_iter_t *it) {
  Position const *const pos = ecs_field(it, Position, 0);
  Spotlight const *const spotlight = ecs_singleton_get(it->world, Spotlight);
  assert(pos);
  assert(spotlight);
  int spotLoc = GetShaderLocation(*spotlight, "spotLoc");
  SetShaderValue(*spotlight, spotLoc, (float[2]){pos->x, pos->y},
                 SHADER_UNIFORM_VEC2);
}

void AddPlayer(ecs_world_t *world) {
  assert(world);
  const ResourceTable *resource_table = ecs_singleton_get(world, ResourceTable);
  assert(resource_table);
  ecs_entity_t player = ecs_new(world);
  ecs_add(world, player, PlayerTag);
  Position pos = resource_table->player_start;
  ecs_set(world, player, Position, {.x = pos.x, .y = pos.y});
  ecs_set(world, player, Velocity, {0});
  ecs_set(world, player, Facing, {.x = 1, .y = -1});
  ecs_set(world, player, RectSprite,
          {.color = GREEN,
           .dimensions = {.x = pos.x, .y = pos.y, .width = 8, .height = 10}});
  ecs_set(world, player, CollisionBox,
          {.x = pos.x, .y = pos.y, .width = 8, .height = 9});
  ecs_set(world, player, CameraFollow,
          {.zoom = 4.0f,
           .target = pos,
           .rotation = 0,
           .offset = {.x = 640 / 2, .y = 480 / 2}});
  ecs_set_ptr(
      world, player, AnimatedSprite,
      (const void *)&resource_table->animated_sprites[ANIM_PLAYER_WALK_FR]);
}

void PlayerImport(ecs_world_t *world) {
  ECS_MODULE(world, Player);
  ECS_TAG_DEFINE(world, PlayerTag);
  ECS_QUERY_DEFINE(world, PlayerCollisionQuery, physics.Position,
                   physics.CollisionBox, !player.PlayerTag);
  ECS_SYSTEM_DEFINE(world, SystemPlayerMove, EcsPreUpdate, physics.Velocity,
                    player.PlayerTag, physics.Facing);
  ECS_SYSTEM_DEFINE(world, SystemPlayerSpriteUpdate, EcsPreUpdate,
                    physics.Facing, resources.AnimatedSprite, physics.Velocity,
                    player.PlayerTag);
  ECS_SYSTEM_DEFINE(world, SystemPlayerUpdateShader, EcsPreUpdate,
                    physics.Position, player.PlayerTag);
}
