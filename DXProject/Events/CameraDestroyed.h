#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/Entity.h"

struct CameraDestroyed : public Event<CameraDestroyed> {
	CameraDestroyed(EntityId eid) : id_(eid) {}

	EntityId id_;
};