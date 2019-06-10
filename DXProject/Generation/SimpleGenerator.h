#pragma once
#include "Generator.h"

class SimpleGenerator : public Generator {
public:
	SimpleGenerator(Noise* noise);
	~SimpleGenerator();
	void generate(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & blocks, int x, int y, int z) override;
	void generate(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & blocks, TerrainCoord coord) override;

private:
};

