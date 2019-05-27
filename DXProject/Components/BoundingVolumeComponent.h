#pragma once
#include "../ECS/Component.h"
#include <DirectXCollision.h>

using namespace DirectX;

class BoundingVolumeComponent : public Component<BoundingVolumeComponent> {
public:
	BoundingVolumeComponent(BoundingBox bounding_box);
	~BoundingVolumeComponent();

	void setBoundingVolume(BoundingBox box);
	BoundingBox & getBoundingVolume();

	void setInsindeFrustum(bool v);
	bool isInsideFrusutm();
private:
	BoundingBox boundingBox_;
	bool isInsideFrustum_;
};