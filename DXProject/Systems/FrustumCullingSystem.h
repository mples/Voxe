#pragma once

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

	Octree<BoundingVolumeComponent> octree_;
	IGameCamera * activeCamera_;
};