#pragma once
#include "BlockType.h"
#include <DirectXMath.h>

using namespace DirectX;

class VoxelRayCast {
public:
	static bool rayCast(XMFLOAT3 origin, XMFLOAT3 direction, float distance, XMINT3 & out_chunk_coord, XMINT3 & out_voxel_coord);
};