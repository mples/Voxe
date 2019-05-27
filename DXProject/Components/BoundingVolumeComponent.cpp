#include "BoundingVolumeComponent.h"
#include "../Engine.h"
#include "../Events/BoundingVolumeCreated.h"
#include "../Events/BoundingVolumeDestroyed.h"

BoundingVolumeComponent::BoundingVolumeComponent(BoundingBox bounding_box) : boundingBox_(bounding_box), isInsideFrustum_(false) {
	ENGINE.sendEvent<BoundingVolumeCreated>(getId());
}

BoundingVolumeComponent::~BoundingVolumeComponent() {
	ENGINE.sendEvent<BoundingVolumeDestroyed>(getId());
}

void BoundingVolumeComponent::setBoundingVolume(BoundingBox box) {
	boundingBox_ = box;
}

BoundingBox & BoundingVolumeComponent::getBoundingVolume() {
	return boundingBox_;
}

void BoundingVolumeComponent::setInsindeFrustum(bool v) {
	isInsideFrustum_ = v;
}

bool BoundingVolumeComponent::isInsideFrusutm() {
	return isInsideFrustum_;
}
