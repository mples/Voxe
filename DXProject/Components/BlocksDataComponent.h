#pragma once

#include "../ECS/Component.h"
#include "../Voxel/BlockType.h"
#include <Windows.h>

class BlocksDataComponent : public Component<BlocksDataComponent> {
public:
public:
	BlocksDataComponent();
	BlocksDataComponent(BlockType b[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM]);
	~BlocksDataComponent();

	void setBlocks(BlockType b[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM]);
	BlockType getBlock(UINT x, UINT y, UINT z);
private:
	BlockType blocks_[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM];
};