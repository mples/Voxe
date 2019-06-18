#pragma once
#include "../ECS/Event/Event.h"
#include <DirectXMath.h>
#include "../Voxel/BlockType.h"

struct VoxelChangeRequest : public Event<VoxelChangeRequest> {
	VoxelChangeRequest(DirectX::XMINT3 chunk_coord, DirectX::XMINT3 voxel_coord, BlockType type) : chunkCoord_(chunk_coord), voxelCoord_(voxel_coord), type_(type) {}

	DirectX::XMINT3 chunkCoord_;
	DirectX::XMINT3 voxelCoord_;
	BlockType type_;
};