#pragma once

#include "../ECS/Component.h"
#include "../Voxel/BlockType.h"
#include "../Utilities/Structures/Array3D.h"
#include <Windows.h>

class BlocksDataComponent : public Component<BlocksDataComponent> {
public:
public:
	BlocksDataComponent();
	BlocksDataComponent(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> &b);
	~BlocksDataComponent();

	void setBlocks(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> &b);
	BlockType getBlock(UINT x, UINT y, UINT z);

	bool isEmpty();
private:
	Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> blocks_;
	bool isEmpty_;
};