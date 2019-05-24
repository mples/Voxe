#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/Component.h"

struct BoundingVolumeCreated : public Event<BoundingVolumeCreated> {
	BoundingVolumeCreated(ComponentId cid) : id_(cid) {}

	ComponentId id_;
};