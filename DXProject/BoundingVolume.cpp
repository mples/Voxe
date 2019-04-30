#include "BoundingVolume.h"


BoundingVolume::BoundingVolume() {
}

BoundingVolume::BoundingVolume(XMINT3 coord, BoundingBox bounding_box) : coord_(coord), boundingBox_(bounding_box) {
}

BoundingVolume::BoundingVolume(int x, int y, int z, BoundingBox bounding_box) : coord_(x,y,z), boundingBox_(bounding_box) {
}

BoundingVolume::~BoundingVolume() {
}

BoundingBox& BoundingVolume::getBoundingVolume() {
	return boundingBox_;
}

XMINT3 & BoundingVolume::getCoord() {
	return coord_;
}
