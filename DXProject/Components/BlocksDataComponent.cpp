#include "BlocksDataComponent.h"

BlocksDataComponent::BlocksDataComponent() : isEmpty_(true) {
	blocks_.fillWith(BlockType::AIR);
}

BlocksDataComponent::BlocksDataComponent(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> &b) : isEmpty_(true) {
	blocks_ = b;
	for (int x = 0; x < TERRAIN_CHUNK_DIM; x++) {
		for (int y = 0; y < TERRAIN_CHUNK_DIM; y++) {
			for (int z = 0; z < TERRAIN_CHUNK_DIM; z++) {
				if (blocks_.at(x, y, z) != BlockType::AIR) {
					isEmpty_ = false;
					return;
				}
			}
		}
	}
}

BlocksDataComponent::~BlocksDataComponent() {
}

void BlocksDataComponent::setBlocks(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> &b) {
	blocks_ = b;
	isEmpty_ = true;

	for (int x = 0; x < TERRAIN_CHUNK_DIM; x++) {
		for (int y = 0; y < TERRAIN_CHUNK_DIM; y++) {
			for (int z = 0; z < TERRAIN_CHUNK_DIM; z++) {
				if (blocks_.at(x,y,z) != BlockType::AIR) {
					isEmpty_ = false;
					return;
				}
			}
		}
	}
}

BlockType BlocksDataComponent::getBlock(UINT x, UINT y, UINT z) {
	return blocks_.at(x, y, z);
}

bool BlocksDataComponent::isEmpty() {
	return isEmpty_;
}
