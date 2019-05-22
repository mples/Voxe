#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/IEntity.h"

struct VoxelDataGeneratedEvent : public Event<VoxelDataGeneratedEvent> {
	VoxelDataGeneratedEvent(EntityId id) : id_(id) {}
	EntityId id_;
};