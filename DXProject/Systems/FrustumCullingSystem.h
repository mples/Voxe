#pragma once

#include "../ECS/System.h"
#include "../ECS/Event/IEventListener.h"

#include "../Components/BoundingVolumeComponent.h"

#include "../Utilities/Structures/Octree/Octree.h"
#include "../Events/BoundingVolumeCreated.h"

class FrustumCullingSystem : public System<FrustumCullingSystem>, public IEventListener {
public:
	FrustumCullingSystem();
	~FrustumCullingSystem();

	virtual void preUpdate(float dt) override;

	virtual void update(float dt) override;

	virtual void postUpdate(float dt) override;

private:
	void onBoundingVolumeCreated(const BoundingVolumeCreated * e);

	Octree<BoundingVolumeComponent> octree_;
};