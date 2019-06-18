#pragma once
#include <DirectXMath.h>
#include "../ECS/Component.h"
#include "../Voxel/BlockType.h"
#include "../Utilities/Structures/Array3D.h"
#include <Windows.h>

using namespace DirectX;
class BlocksDataComponent : public Component<BlocksDataComponent> {
public:
public:
	BlocksDataComponent();
	BlocksDataComponent(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> &b);
	~BlocksDataComponent();

	void setBlocks(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> &b);
	BlockType getBlock(UINT x, UINT y, UINT z);
	BlockType getBlock(XMINT3 coord);

	void setBlock(XMINT3 coord, BlockType type);
	void setBlock(UINT x, UINT y, UINT z, BlockType type);
	bool isEmpty();
private:
	Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> blocks_;
	bool isEmpty_;
};