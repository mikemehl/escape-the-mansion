#ifndef __IMAGE__H__
#define __IMAGE__H__
#include <flecs.h>
#include <raylib.h>
#include <tmx.h>

typedef struct Sprite {
  Texture2D texture;
  Rectangle area;
} Sprite;

typedef tmx_map Tiled;

typedef struct AnimatedSprite {
  Texture2D texture;
  Rectangle *frames;
  uint32_t *durations;
  uint32_t num_frames;
  uint32_t curr_frame;
  uint32_t frame_time;
  bool paused;
} AnimatedSprite;

typedef enum AnimatedSpriteIndex {
  ANIM_PLAYER_WALK_FR = 0,
  ANIM_PLAYER_WALK_FL,
  ANIM_PLAYER_WALK_BR,
  ANIM_PLAYER_WALK_BL,
  NUM_ANIMATED_SPRITE_INDEXES
} AnimatedSpriteIndex;

typedef struct ResourceTable {
  AnimatedSprite animated_sprites[NUM_ANIMATED_SPRITE_INDEXES];
  Rectangle *floor_tiles;
  Rectangle *wall_tiles;
  Texture2D haunted_house_tileset;
  int width_tiles;
  int height_tiles;
  Vector2 player_start;
} ResourceTable;

extern ECS_COMPONENT_DECLARE(Sprite);
extern ECS_COMPONENT_DECLARE(Tiled);
extern ECS_COMPONENT_DECLARE(AnimatedSprite);
extern ECS_COMPONENT_DECLARE(ResourceTable);

void ResourcesImport(ecs_world_t *world);
void FreeResources();

#endif
