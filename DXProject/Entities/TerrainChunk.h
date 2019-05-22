#pragma once
#include "../ECS/Entity.h"

class TerrainChunk : public Entity<TerrainChunk> {
public:
	TerrainChunk();
	~TerrainChunk();
};