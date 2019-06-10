#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/Entity.h"
#include <DirectXMath.h>

using namespace DirectX;

struct TerrainChunkDestroyedEvent : public Event<TerrainChunkDestroyedEvent> {
	TerrainChunkDestroyedEvent(XMINT3 coord) : coord_(coord) {}
	TerrainChunkDestroyedEvent(int x, int y, int z) : coord_(x,y,z) {}

	DirectX::XMINT3 coord_;
};