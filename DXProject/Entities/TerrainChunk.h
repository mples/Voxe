#pragma once
#include "../ECS/Entity.h"
#include "../Components/WorldCoordinateComponent.h"


class TerrainChunk : public Entity<TerrainChunk> {
public:
	TerrainChunk(XMINT3 coord);
	~TerrainChunk();

private:
	WorldCoordinateComponent * coordComp_;
};