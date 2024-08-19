#include "render.h"
#include "flecs/addons/flecs_c.h"
#include "physics.h"
#include "resources.h"
#include "tmx.h"
#include <flecs.h>
#include <raylib.h>

typedef Shader Spotlight;

ECS_COMPONENT_DECLARE(RectSprite);
ECS_COMPONENT_DECLARE(CameraFollow);
ECS_COMPONENT_DECLARE(Spotlight);
ECS_SYSTEM_DECLARE(SystemDrawSprite);
ECS_SYSTEM_DECLARE(SystemDrawRectSprite);
ECS_SYSTEM_DECLARE(SystemDrawBegin);
ECS_SYSTEM_DECLARE(SystemDrawEnd);
ECS_SYSTEM_DECLARE(SystemCameraDrawBegin);
ECS_SYSTEM_DECLARE(SystemCameraDrawEnd);
ECS_SYSTEM_DECLARE(SystemCameraUpdate);
ECS_SYSTEM_DECLARE(SystemDrawRoom);
ECS_SYSTEM_DECLARE(SystemDrawAnimatedSprite);

static void SystemDrawSprite(ecs_iter_t *it) {
  Position *p = ecs_field(it, Position, 0);
  Sprite *sprite = ecs_field(it, Sprite, 1);

  for (int i = 0; i < it->count; i++) {
    DrawTextureRec(sprite->texture, sprite->area, *p, WHITE);
  }
}

static void SystemDrawRectSprite(ecs_iter_t *it) {
  Position *p = ecs_field(it, Position, 0);
  RectSprite *r = ecs_field(it, RectSprite, 1);
  assert(p);
  assert(r);

  for (int i = 0; i < it->count; i++) {
    DrawRectangle(p[i].x, p[i].y, r[i].dimensions.width, r[i].dimensions.height,
                  r[i].color);
  }
}

static void SystemCameraDrawBegin(ecs_iter_t *it) {
  CameraFollow *camera = ecs_field(it, CameraFollow, 0);
  assert(camera);
  assert(it->count == 1);
  BeginMode2D(*camera);
}

static void SystemCameraDrawEnd(ecs_iter_t *it) {
  CameraFollow *camera = ecs_field(it, CameraFollow, 0);
  assert(camera);
  assert(it->count == 1);
  EndMode2D();
}

static void SystemCameraUpdate(ecs_iter_t *it) {
  CameraFollow *camera = ecs_field(it, CameraFollow, 0);
  Position *pos = ecs_field(it, Position, 1);

  for (int i = 0; i < it->count; i++) {
    camera[i].target = pos[i];
  }
}

static void SystemDrawRoom(ecs_iter_t *it) {
  const ResourceTable *resource_table = ecs_field(it, ResourceTable, 0);
  assert(resource_table);
  for (int x = 0; x < resource_table->width_tiles; x++) {
    for (int y = 0; y < resource_table->height_tiles; y++) {
      Rectangle floor_source_rec =
          resource_table->floor_tiles[x + y * resource_table->width_tiles];
      Rectangle wall_source_rec =
          resource_table->wall_tiles[x + y * resource_table->width_tiles];
      if (floor_source_rec.width > 0) {
        DrawTexturePro(resource_table->haunted_house_tileset, floor_source_rec,
                       (Rectangle){.x = x * floor_source_rec.width,
                                   .y = y * floor_source_rec.height,
                                   .width = floor_source_rec.width,
                                   .height = floor_source_rec.height},
                       (Vector2){.x = 0, .y = 0}, 0.0, WHITE);
      }
      if (wall_source_rec.width > 0) {
        DrawTexturePro(resource_table->haunted_house_tileset, wall_source_rec,
                       (Rectangle){.x = x * wall_source_rec.width,
                                   .y = y * wall_source_rec.height,
                                   .width = wall_source_rec.width,
                                   .height = wall_source_rec.height},
                       (Vector2){.x = 0, .y = 0}, 0.0, WHITE);
      }
    }
  }
}

static void SystemDrawAnimatedSprite(ecs_iter_t *it) {
  AnimatedSprite *const sprite = ecs_field(it, AnimatedSprite, 0);
  Position const *const pos = ecs_field(it, Position, 1);
  assert(sprite);
  assert(pos);
  for (int i = 0; i < it->count; i++) {
    Rectangle curr_frame = sprite[i].frames[sprite->curr_frame];
    DrawTexturePro(sprite[i].texture, curr_frame,
                   (Rectangle){.x = pos[i].x,
                               .y = pos[i].y,
                               .width = curr_frame.width,
                               .height = curr_frame.height},
                   (Vector2){.x = 0, .y = 0}, 0, WHITE);
    if (sprite[i].paused) {
      continue;
    }
    sprite[i].frame_time += 10;
    if (sprite[i].frame_time >= sprite[i].durations[sprite[i].curr_frame]) {
      sprite[i].frame_time = 0;
      sprite[i].curr_frame = (sprite[i].curr_frame + 1) % sprite[i].num_frames;
    }
  }
}

static void SystemDrawBegin(ecs_iter_t *it) {
  const Spotlight *spotlight = ecs_singleton_get(it->world, Spotlight);
  BeginDrawing();
  /* BeginShaderMode(*spotlight); */
  ClearBackground(BLACK);
}

static void SystemDrawEnd(ecs_iter_t *it) {
  /* EndShaderMode(); */
  EndDrawing();
}

static void InitSpotlight(ecs_world_t *world) {
  Spotlight spotlight = LoadShader(0, "src/spotlightfs.glsl");
  ecs_singleton_set_ptr(world, Spotlight, &spotlight);
}

void RenderImport(ecs_world_t *world) {
  ECS_MODULE(world, Render);

  // phases
  ecs_entity_t DrawBegin = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t CameraDrawBegin = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t Draw = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t CameraDraw = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t CameraDrawRoom = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t DrawEnd = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t CameraDrawEnd = ecs_new_w_id(world, EcsPhase);

  ecs_add_pair(world, DrawBegin, EcsDependsOn, EcsPreUpdate);
  ecs_add_pair(world, CameraDrawBegin, EcsDependsOn, DrawBegin);
  ecs_add_pair(world, CameraDrawRoom, EcsDependsOn, CameraDrawBegin);
  ecs_add_pair(world, CameraDraw, EcsDependsOn, CameraDrawRoom);
  ecs_add_pair(world, CameraDrawEnd, EcsDependsOn, CameraDraw);
  ecs_add_pair(world, Draw, EcsDependsOn, CameraDrawEnd);
  ecs_add_pair(world, DrawEnd, EcsDependsOn, Draw);

  ECS_COMPONENT_DEFINE(world, RectSprite);
  ECS_COMPONENT_DEFINE(world, CameraFollow);
  ECS_COMPONENT_DEFINE(world, Spotlight);
  InitSpotlight(world);

  ECS_SYSTEM_DEFINE(world, SystemDrawBegin, DrawBegin);
  ECS_SYSTEM_DEFINE(world, SystemCameraDrawBegin, CameraDrawBegin,
                    render.CameraFollow);
  ECS_SYSTEM_DEFINE(world, SystemDrawRoom, CameraDrawRoom,
                    resources.ResourceTable($));
  ECS_SYSTEM_DEFINE(world, SystemDrawRectSprite, CameraDraw, physics.Position,
                    render.RectSprite);
  ECS_SYSTEM_DEFINE(world, SystemDrawSprite, CameraDraw, physics.Position,
                    resources.Sprite);
  ECS_SYSTEM_DEFINE(world, SystemDrawAnimatedSprite, CameraDraw,
                    resources.AnimatedSprite, physics.Position);
  ECS_SYSTEM_DEFINE(world, SystemCameraDrawEnd, CameraDrawEnd,
                    render.CameraFollow);
  ECS_SYSTEM_DEFINE(world, SystemDrawEnd, DrawEnd);
  ECS_SYSTEM_DEFINE(world, SystemCameraUpdate, DrawEnd, render.CameraFollow,
                    physics.Position);
}
