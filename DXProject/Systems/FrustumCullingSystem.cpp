#include "FrustumCullingSystem.h"
#include "../Engine.h"
#include "../Entities/GameCamera.h"
#include "../Components/MeshComponent.h"
#include <vector>

FrustumCullingSystem::FrustumCullingSystem() : IEventListener(ENGINE.getEventHandler()), activeCamera_(nullptr), octree_(BoundingBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(50.0f, 50.0f, 50.0f))) {
	registerEventCallback<BoundingVolumeCreated>([&](const BoundingVolumeCreated * e) {
		onBoundingVolumeCreated(e);
	});

	registerEventCallback<BoundingVolumeDestroyed>([&](const BoundingVolumeDestroyed * e) {
		onBoundingVolumeDestroyed(e);
	});

	registerEventCallback<CameraCreated>([&](const CameraCreated * e) {
		onCameraCreated(e);
	});

	registerEventCallback<CameraDestroyed>([&](const CameraDestroyed * e) {
		onCameraDestroyed(e);
	});
}

FrustumCullingSystem::~FrustumCullingSystem() {
}

void FrustumCullingSystem::preUpdate(float dt) {
	for (ComponentId & c_id : boundingVolumesToInsert_) {
		BoundingVolumeComponent * bv_comp = ENGINE.getComponentManager().getComponentByComponentId<BoundingVolumeComponent>(c_id);
		if (bv_comp != nullptr) {
			octree_.insert(FrustumCullingInstance(bv_comp));
		}
	}
	boundingVolumesToInsert_.clear();

	for (ComponentId & c_id : boundingVolumesToRemove_) {

		octree_.remove(FrustumCullingInstance(c_id));

	}
	boundingVolumesToRemove_.clear();

}

void FrustumCullingSystem::update(float dt) {
	if (activeCamera_ == nullptr) {
		return;
	}

	std::vector<FrustumCullingInstance> visible_volumes = octree_.collides(activeCamera_->getWorldSpaceFrustum());

	auto it = ENGINE.getComponentManager().begin<BoundingVolumeComponent>();
	auto end = ENGINE.getComponentManager().end<BoundingVolumeComponent>();
	while (it != end) {
		it->setInsindeFrustum(false);
		++it;
	}

	for (FrustumCullingInstance& fc_instance : visible_volumes) {
		BoundingVolumeComponent * bv_comp = ENGINE.getComponentManager().getComponentByComponentId<BoundingVolumeComponent>(fc_instance.id_);
		if (bv_comp) {
			bv_comp->setInsindeFrustum(true);
		}
	}
}

void FrustumCullingSystem::postUpdate(float dt) {

}

void FrustumCullingSystem::onBoundingVolumeCreated(const BoundingVolumeCreated * e) {
	boundingVolumesToInsert_.push_back(e->id_);
}

void FrustumCullingSystem::onBoundingVolumeDestroyed(const BoundingVolumeDestroyed * e) {
	boundingVolumesToRemove_.push_back(e->id_);
}

void FrustumCullingSystem::onCameraCreated(const CameraCreated * e) {
	if (activeCamera_ == nullptr) {
		activeCamera_ = dynamic_cast<IGameCamera*>(ENGINE.getEntityManager().getEntity<GameCamera>(e->id_));
		assert(activeCamera_ != nullptr && "Falied to get IGameCamera pointer.");
	}
	else {
		assert(0 && "There should bo only one active camera");
	}
}

void FrustumCullingSystem::onCameraDestroyed(const CameraDestroyed * e) {
	activeCamera_ = nullptr;
}
