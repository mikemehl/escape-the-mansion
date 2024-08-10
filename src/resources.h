#ifndef __IMAGE__H__
#define __IMAGE__H__
#include "flecs.h"
#include <raylib.h>
#include <tmx.h>

typedef struct Sprite {
  Texture2D texture;
  Rectangle area;
} Sprite;

typedef tmx_map *Tiled;

extern ECS_COMPONENT_DECLARE(Sprite);
extern ECS_COMPONENT_DECLARE(Tiled);

void   ResourcesImport(ecs_world_t *world);
Sprite load_walk_sprite();

#endif
