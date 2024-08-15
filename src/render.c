#include "render.h"
#include "physics.h"
#include "resources.h"
#include "tmx.h"
#include <flecs.h>
#include <raylib.h>

ECS_COMPONENT_DECLARE(RectSprite);
ECS_COMPONENT_DECLARE(CameraFollow);
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

static void DrawTile(tmx_tile *tile, Vector2 pos) {
  tmx_tileset *tileset = tile->tileset;
  assert(tileset);

  Texture2D *texture = tileset->image->resource_image;
  assert(texture);

  Rectangle tile_rect = {.x = tile->ul_x,
                         .y = tile->ul_y,
                         .width = tile->width,
                         .height = tile->height};

  DrawTextureRec(*texture, tile_rect, pos, WHITE);
}

static void SystemDrawRoom(ecs_iter_t *it) {
  const Tiled *tiled = ecs_field(it, Tiled, 0);
  assert(tiled);
  tmx_layer *layer = tiled->ly_head;
  tmx_tile **tiles = tiled->tiles;
  assert(layer);
  while (layer) {
    if (layer->type != L_LAYER) {
      layer = layer->next;
      continue;
    }
    uint32_t *cells = layer->content.gids;
    for (uint32_t x = 0; x < tiled->width; x++) {
      for (uint32_t y = 0; y < tiled->height; y++) {
        uint32_t gid = cells[x + y * tiled->width] & TMX_FLIP_BITS_REMOVAL;
        if (gid != 0) {
          tmx_tile *tile = tiles[gid];
          assert(tile);
          Position pos = {.x = x * tiled->tile_width,
                          .y = y * tiled->tile_height};
          DrawTile(tile, pos);
        }
      }
    }
    layer = layer->next;
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
  BeginDrawing();
  ClearBackground(BLACK);
}

static void SystemDrawEnd(ecs_iter_t *it) { EndDrawing(); }

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
  ECS_SYSTEM_DEFINE(world, SystemDrawBegin, DrawBegin);
  ECS_SYSTEM_DEFINE(world, SystemCameraDrawBegin, CameraDrawBegin,
                    render.CameraFollow);
  ECS_SYSTEM_DEFINE(world, SystemDrawRoom, CameraDrawRoom, resources.Tiled($));
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
