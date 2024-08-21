#include "door.h"
#include "flecs/addons/flecs_c.h"
#include "flecs/private/addons.h"
#include "physics.h"
#include "resources.h"

ECS_COMPONENT_DECLARE(DoorTile);
ECS_COMPONENT_DECLARE(UsingDoor);
ECS_SYSTEM_DECLARE(SystemCheckDoors);

static void SystemCheckDoors(ecs_iter_t *it) {
  Position *pos = ecs_field(it, Position, 0);
  assert(pos);

  TraceLog(LOG_INFO, "USING DOOR COUNT: %d", it->count);
  for (int i = 0; i < it->count; i++) {
    ecs_entity_t door_id =
        ecs_get_target(it->world, it->entities[i], ecs_id(UsingDoor), 0);
    if (door_id == 0) {
      continue;
    }
    const DoorTile *door = ecs_get(it->world, door_id, DoorTile);
    assert(door);

    pos[i] = door->to;
  }
}

void AddDoors(ecs_world_t *world) {
  ResourceTable const *const resource_table =
      ecs_singleton_get(world, ResourceTable);
  for (int i = 0; i < resource_table->num_doors; i++) {
    ecs_entity_t door = ecs_new(world);
    ecs_set(world, door, Position,
            {.x = resource_table->doors[i].pos.x,
             .y = resource_table->doors[i].pos.y});
    ecs_set(world, door, DoorTile,
            {.to = resource_table->doors[i].obj.to,
             .locked = resource_table->doors[i].obj.locked,
             resource_table->doors[i].obj.cooldown});
  }
}

void DoorImport(ecs_world_t *world) {
  ECS_MODULE(world, Door);
  ECS_COMPONENT_DEFINE(world, DoorTile);
  ECS_COMPONENT_DEFINE(world, UsingDoor);
  ECS_SYSTEM_DEFINE(world, SystemCheckDoors, EcsOnUpdate, physics.Position,
                    UsingDoor);
  AddDoors(world);
}
