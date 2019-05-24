#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/IEntity.h"

struct TerrainChunkChanged : public Event<TerrainChunkChanged> {
	TerrainChunkChanged(EntityId id) : id_(id) {}
	EntityId id_;
};