#include "BlocksDataComponent.h"

BlocksDataComponent::BlocksDataComponent() {
	ZeroMemory(blocks_, pow(TERRAIN_CHUNK_DIM, 3) * sizeof(BlockType));
}

BlocksDataComponent::BlocksDataComponent(BlockType b[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM]) {
	CopyMemory(blocks_, b, sizeof(BlockType) * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM);
}

BlocksDataComponent::~BlocksDataComponent() {
}

void BlocksDataComponent::setBlocks(BlockType b[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM]) {
	CopyMemory(blocks_, b, sizeof(BlockType) * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM * TERRAIN_CHUNK_DIM);
}

BlockType BlocksDataComponent::getBlock(UINT x, UINT y, UINT z) {
	return blocks_[x][y][z];
}
