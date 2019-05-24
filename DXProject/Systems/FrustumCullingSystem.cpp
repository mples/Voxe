#include "FrustumCullingSystem.h"
#include "../Engine.h"


FrustumCullingSystem::FrustumCullingSystem() : IEventListener(ENGINE.getEventHandler()) {
	std::function<void(const BoundingVolumeCreated * e)> bounding_created_callback = [&](const BoundingVolumeCreated * e) {
		onBoundingVolumeCreated(e);
	};
	registerEventCallback<BoundingVolumeCreated>(bounding_created_callback);
}

FrustumCullingSystem::~FrustumCullingSystem() {
}

void FrustumCullingSystem::preUpdate(float dt) {
}

void FrustumCullingSystem::update(float dt) {
}

void FrustumCullingSystem::postUpdate(float dt) {
}

void FrustumCullingSystem::onBoundingVolumeCreated(const BoundingVolumeCreated * e) {
	BoundingVolumeComponent * bv_comp = ENGINE.getComponentManager().getComponentByComponentId<BoundingVolumeComponent>(e->id_);
	octree_.insert(bv_comp);
}
