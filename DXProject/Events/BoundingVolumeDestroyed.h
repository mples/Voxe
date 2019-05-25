#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/Component.h"

struct BoundingVolumeDestroyed : public Event<BoundingVolumeDestroyed> {
	BoundingVolumeDestroyed(ComponentId cid) : id_(cid) {}

	ComponentId id_;
};