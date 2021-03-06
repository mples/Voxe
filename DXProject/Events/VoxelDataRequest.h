#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/IEntity.h"

struct VoxelDataRequest : public Event<VoxelDataRequest> {
	VoxelDataRequest(EntityId id ) : id_(id) {}
	EntityId id_;
};