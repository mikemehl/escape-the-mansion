#ifndef __PLAYER__H__
#define __PLAYER__H__
#include "flecs.h"

extern ECS_TAG_DECLARE(PlayerTag);
extern ECS_SYSTEM_DECLARE(SystemPlayerMove);
extern ECS_SYSTEM_DECLARE(SystemPlayerAnimationUpdate);
extern ECS_SYSTEM_DECLARE(SystemPlayerSpriteUpdate);
extern ECS_QUERY_DECLARE(PlayerCollisionQuery);

void PlayerImport(ecs_world_t *world);
void AddPlayer(ecs_world_t *world);
#endif
