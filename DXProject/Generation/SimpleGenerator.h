#pragma once
#include "Generator.h"

class SimpleGenerator : public Generator {
public:
	SimpleGenerator(Noise* noise);
	~SimpleGenerator();
	void generate(BlockType blocks[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM], int x, int y, int z) override;
	void generate(BlockType blocks[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM], TerrainCoord coord) override;

private:
};

