#include "render.h"
#include "physics.h"
#include "resources.h"
#include <raylib.h>

ECS_COMPONENT_DECLARE(RectSprite);
ECS_COMPONENT_DECLARE(CameraFollow);
ECS_SYSTEM_DECLARE(system_draw_sprite);

void system_draw_sprite(ecs_iter_t *it) {
  Position *p      = ecs_field(it, Position, 0);
  Sprite   *sprite = ecs_field(it, Sprite, 1);

  for (int i = 0; i < it->count; i++) {
    DrawTextureRec(sprite->texture, sprite->area, *p, WHITE);
  }
}

void RenderRoom() {
  tmx_map const *const room  = load_tiled();
  tmx_layer const     *layer = room->ly_head;
  while (layer) {
    if (layer->type != L_LAYER) {
      layer = layer->next;
      continue;
    }
    uint32_t const *gids = layer->content.gids;
    for (uint32_t x = 0; x < room->width; x++) {
      for (uint32_t y = 0; y < room->height; y++) {
        uint32_t gid = (gids[y * room->width + x]) & TMX_FLIP_BITS_REMOVAL;
        if (gid == 0) {
          continue;
        }
        tmx_tile const *const    tile      = room->tiles[gid];
        tmx_tileset const *const tileset   = room->tiles[gid]->tileset;
        Texture2D               *img       = tileset->image->resource_image;
        Rectangle                tile_rect = {.x      = tile->ul_x,
                                              .y      = tile->ul_y,
                                              .width  = tile->width,
                                              .height = tile->height};
        Vector2 pos = {.x = x * room->tile_width, .y = y * room->tile_height};
        DrawTextureRec(*img, tile_rect, pos, WHITE);
      }
    }
    layer = layer->next;
  }
}

void RenderImport(ecs_world_t *world) {
  ECS_MODULE(world, Render);
  ECS_COMPONENT_DEFINE(world, RectSprite);
  ECS_COMPONENT_DEFINE(world, CameraFollow);
  ECS_SYSTEM_DEFINE(world, system_draw_sprite, EcsOnUpdate, physics.Position,
                    resources.Sprite);
}
