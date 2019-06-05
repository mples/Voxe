#include "BlocksDataComponent.h"

BlocksDataComponent::BlocksDataComponent() : isEmpty_(true) {
	ZeroMemory(blocks_, pow(TERRAIN_CHUNK_DIM, 3) * sizeof(BlockType));
}

BlocksDataComponent::BlocksDataComponent(BlockType b[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM]) : isEmpty_(true) {
	CopyMemory(blocks_, b, sizeof(BlockType) * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM);

	for (int x = 0; x < TERRAIN_CHUNK_DIM; x++) {
		for (int y = 0; y < TERRAIN_CHUNK_DIM; y++) {
			for (int z = 0; z < TERRAIN_CHUNK_DIM; z++) {
				if (blocks_[x][y][z] != BlockType::AIR) {
					isEmpty_ = false;
					return;
				}
			}
		}
	}
}

BlocksDataComponent::~BlocksDataComponent() {
}

void BlocksDataComponent::setBlocks(BlockType b[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM]) {
	CopyMemory(blocks_, b, sizeof(BlockType) * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM);

	isEmpty_ = true;

	for (int x = 0; x < TERRAIN_CHUNK_DIM; x++) {
		for (int y = 0; y < TERRAIN_CHUNK_DIM; y++) {
			for (int z = 0; z < TERRAIN_CHUNK_DIM; z++) {
				if (blocks_[x][y][z] != BlockType::AIR) {
					isEmpty_ = false;
					return;
				}
			}
		}
	}
}

BlockType BlocksDataComponent::getBlock(UINT x, UINT y, UINT z) {
	return blocks_[x][y][z];
}

bool BlocksDataComponent::isEmpty() {
	return isEmpty_;
}
