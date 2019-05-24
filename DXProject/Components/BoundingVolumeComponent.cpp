#include "BoundingVolumeComponent.h"

BoundingVolumeComponent::BoundingVolumeComponent() {
}

BoundingVolumeComponent::BoundingVolumeComponent(BoundingBox bounding_box) : boundingBox_(bounding_box) {
}

BoundingVolumeComponent::~BoundingVolumeComponent() {
}

BoundingBox & BoundingVolumeComponent::getBoundingVolume() {
	return boundingBox_;
}
