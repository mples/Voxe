#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/IEntity.h"
#include <DirectXMath.h>

struct TerrainChunkEditedEvent : public Event<TerrainChunkEditedEvent> {
	TerrainChunkEditedEvent(EntityId id) : id_(id) {}
	EntityId id_;
};