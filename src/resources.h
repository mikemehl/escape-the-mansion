#ifndef __IMAGE__H__
#define __IMAGE__H__
#include "flecs.h"
#include <raylib.h>
#include <tmx.h>

typedef struct Sprite {
  Texture2D texture;
  Rectangle area;
} Sprite;

extern ECS_COMPONENT_DECLARE(Sprite);

void     ResourcesImport(ecs_world_t *world);
tmx_map *load_tiled();
Sprite   load_walk_sprite();

#endif
