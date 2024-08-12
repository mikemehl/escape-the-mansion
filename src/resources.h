#ifndef __IMAGE__H__
#define __IMAGE__H__
#include "flecs.h"
#include <raylib.h>
#include <tmx.h>

typedef struct Sprite {
  Texture2D texture;
  Rectangle area;
} Sprite;

typedef tmx_map Tiled;

typedef struct AnimatedSprite {
  Sprite  *frames;
  uint32_t num_frames;
} AnimatedSprite;

extern ECS_COMPONENT_DECLARE(Sprite);
extern ECS_COMPONENT_DECLARE(Tiled);
extern ECS_COMPONENT_DECLARE(AnimatedSprite);

void    ResourcesImport(ecs_world_t *world);
Sprite  LoadWalkSprite();
Vector2 GetPlayerStartPoint(ecs_world_t *world);

#endif
