#include "BlocksDataComponent.h"

BlocksDataComponent::BlocksDataComponent() {
	ZeroMemory(blocks_, pow(DIMENSION, 3) * sizeof(BlockType));
}

BlocksDataComponent::BlocksDataComponent(BlockType b[DIMENSION][DIMENSION][DIMENSION]) {
	CopyMemory(blocks_, b, sizeof(BlockType) * DIMENSION * DIMENSION * DIMENSION);
}

BlocksDataComponent::~BlocksDataComponent() {
}

void BlocksDataComponent::setBlocks(BlockType b[DIMENSION][DIMENSION][DIMENSION]) {
	CopyMemory(blocks_, b, sizeof(BlockType) * DIMENSION * DIMENSION * DIMENSION);
}

BlockType BlocksDataComponent::getBlock(UINT x, UINT y, UINT z) {
	return blocks_[x][y][z];
}
