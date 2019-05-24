#pragma once
#include "../ECS/Component.h"
#include <DirectXCollision.h>

using namespace DirectX;

class BoundingVolumeComponent : public Component<BoundingVolumeComponent> {
public:
	BoundingVolumeComponent();
	BoundingVolumeComponent(BoundingBox bounding_box);
	~BoundingVolumeComponent();

	BoundingBox & getBoundingVolume();
private:
	BoundingBox boundingBox_;
};