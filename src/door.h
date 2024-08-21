#ifndef __DOOR__H__
#define __DOOR__H__
#include "flecs.h"
#include "raylib.h"

typedef struct DoorTile {
  Vector2 to;
  bool locked;
  int cooldown;
} DoorTile;

extern ECS_COMPONENT_DECLARE(DoorTile);
extern ECS_COMPONENT_DECLARE(UsingDoor);
extern ecs_entity_t UsingDoor;

void DoorImport(ecs_world_t *world);
#endif
