#include "FrustumCullingSystem.h"
#include "../Engine.h"
#include "../Entities/GameCamera.h"
#include "../Components/MeshComponent.h"
#include <vector>

FrustumCullingSystem::FrustumCullingSystem() : IEventListener(ENGINE.getEventHandler()), activeCamera_(nullptr), octree_(BoundingBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(50.0f, 50.0f, 50.0f))) {
	std::function<void(const BoundingVolumeCreated * e)> bounding_created_callback = [&](const BoundingVolumeCreated * e) {
		onBoundingVolumeCreated(e);
	};
	registerEventCallback<BoundingVolumeCreated>(bounding_created_callback);

	std::function<void(const BoundingVolumeDestroyed * e)> bounding_destroyed_callback = [&](const BoundingVolumeDestroyed * e) {
		onBoundingVolumeDestroyed(e);
	};
	registerEventCallback<BoundingVolumeDestroyed>(bounding_destroyed_callback);

	std::function<void(const CameraCreated * e)> camera_created_callback = [&](const CameraCreated * e) {
		onCameraCreated(e);
	};
	registerEventCallback<CameraCreated>(camera_created_callback);

	std::function<void(const CameraDestroyed * e)> camera_destroyed_callback = [&](const CameraDestroyed * e) {
		onCameraDestroyed(e);
	};
	registerEventCallback<CameraDestroyed>(camera_destroyed_callback);
}

FrustumCullingSystem::~FrustumCullingSystem() {
}

void FrustumCullingSystem::preUpdate(float dt) {
	auto it = ENGINE.getComponentManager().begin<BoundingVolumeComponent>();
	auto end = ENGINE.getComponentManager().end<BoundingVolumeComponent>();
	while (it != end) {
		it->setInsindeFrustum(false);
		++it;
	}

	for (ComponentId & c_id : boundingVolumesToInsert_) {
		BoundingVolumeComponent * bv_comp = ENGINE.getComponentManager().getComponentByComponentId<BoundingVolumeComponent>(c_id);
		if (bv_comp != nullptr) {
			octree_.insert(bv_comp);
		}
	}
	boundingVolumesToInsert_.clear();

	for (ComponentId & c_id : boundingVolumesToRemove_) {
		BoundingVolumeComponent * bv_comp = ENGINE.getComponentManager().getComponentByComponentId<BoundingVolumeComponent>(c_id);
		if (bv_comp != nullptr) {
			octree_.remove(bv_comp);
		}
	}
	boundingVolumesToRemove_.clear();

}

void FrustumCullingSystem::update(float dt) {
	if (activeCamera_ == nullptr) {
		return;
	}
	BoundingFrustum fr = activeCamera_->getWorldSpaceFrustum();
	std::vector<BoundingVolumeComponent*> visible_volumes = octree_.collides(activeCamera_->getWorldSpaceFrustum());

	char s[256];
	sprintf(s, "Visible Chunks: %u\n", visible_volumes.size());
	OutputDebugStringA(s);

	for (BoundingVolumeComponent* bvc : visible_volumes) {
		bvc->setInsindeFrustum(true);
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
