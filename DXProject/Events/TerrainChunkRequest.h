#pragma once
#include "../ECS/Event/Event.h"
#include <DirectXMath.h>

struct TerrainChunkRequest : public Event<TerrainChunkRequest> {
	TerrainChunkRequest(DirectX::XMINT3 coord) : coord_(coord) {}
	DirectX::XMINT3 coord_;
};