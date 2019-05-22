#pragma once

#include "../ECS/Component.h"
#include "../Voxel/BlockType.h"
#include <Windows.h>

static const UINT DIMENSION = 16;

class BlocksDataComponent : public Component<BlocksDataComponent> {
public:
public:
	BlocksDataComponent();
	BlocksDataComponent(BlockType b[DIMENSION][DIMENSION][DIMENSION]);
	~BlocksDataComponent();

	void setBlocks(BlockType b[DIMENSION][DIMENSION][DIMENSION]);
	BlockType getBlock(UINT x, UINT y, UINT z);
private:
	BlockType blocks_[DIMENSION][DIMENSION][DIMENSION];
};