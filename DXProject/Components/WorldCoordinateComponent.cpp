#include "WorldCoordinateComponent.h"
#include "BlocksDataComponent.h"

WorldCoordinateComponent::WorldCoordinateComponent() : coord_(0, 0, 0) {
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x * static_cast<int>(TERRAIN_CHUNK_DIM)),
										static_cast<float>(coord_.y * static_cast<int>(TERRAIN_CHUNK_DIM)),
										static_cast<float>(coord_.z * static_cast<int>(TERRAIN_CHUNK_DIM)));
}

WorldCoordinateComponent::WorldCoordinateComponent(int x, int y, int z) : coord_(x, y, z) {
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x * static_cast<int>(TERRAIN_CHUNK_DIM)),
										static_cast<float>(coord_.y * static_cast<int>(TERRAIN_CHUNK_DIM)),
										static_cast<float>(coord_.z * static_cast<int>(TERRAIN_CHUNK_DIM)));
}

WorldCoordinateComponent::WorldCoordinateComponent(XMINT3 coord) : coord_(coord) {
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x * static_cast<int>(TERRAIN_CHUNK_DIM)),
										static_cast<float>(coord_.y * static_cast<int>(TERRAIN_CHUNK_DIM)),
										static_cast<float>(coord_.z * static_cast<int>(TERRAIN_CHUNK_DIM)));
}

WorldCoordinateComponent::~WorldCoordinateComponent() {
}

XMINT3 & WorldCoordinateComponent::getCoord() {
	return coord_;
}

XMMATRIX & WorldCoordinateComponent::getWorldMatrix() {
	return worldMatrix_;
}
