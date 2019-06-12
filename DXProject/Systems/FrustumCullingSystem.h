#pragma once
#include <vector>
#include "../ECS/System.h"
#include "../ECS/Event/IEventListener.h"

#include "../Components/BoundingVolumeComponent.h"

#include "../Entities/IGameCamera.h"

#include "../Utilities/Structures/Octree/Octree.h"
#include "../Events/BoundingVolumeCreated.h"
#include "../Events/BoundingVolumeDestroyed.h"
#include "../Events/CameraCreated.h"
#include "../Events/CameraDestroyed.h"

class FrustumCullingSystem : public System<FrustumCullingSystem>, public IEventListener {
	struct FrustumCullingInstance {
		FrustumCullingInstance(BoundingVolumeComponent * bv) : id_(bv->getId()), boundingBox_(bv->getBoundingVolume()) {}
		FrustumCullingInstance(ComponentId c_id) : id_(c_id) {}
		bool operator==(const FrustumCullingInstance& other) {
			return id_ == other.id_;
		}
		bool operator!=(const FrustumCullingInstance& other) {
			return id_ != other.id_;
		}
		BoundingBox & getBoundingVolume() {
			return boundingBox_;
		}
		BoundingBox boundingBox_;
		ComponentId id_;
	};
public:
	FrustumCullingSystem();
	~FrustumCullingSystem();

	virtual void preUpdate(float dt) override;

	virtual void update(float dt) override;

	virtual void postUpdate(float dt) override;

private:
	void onBoundingVolumeCreated(const BoundingVolumeCreated * e);
	void onBoundingVolumeDestroyed(const BoundingVolumeDestroyed* e);

	void onCameraCreated(const CameraCreated * e);
	void onCameraDestroyed(const CameraDestroyed * e);

	Octree<FrustumCullingInstance> octree_;
	IGameCamera * activeCamera_;
	std::vector<ComponentId> boundingVolumesToInsert_;
	std::vector<ComponentId> boundingVolumesToRemove_;
};