#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/Entity.h"

struct CameraCreated : public Event<CameraCreated> {
	CameraCreated(EntityId eid) : id_(eid) {}

	EntityId id_;
};