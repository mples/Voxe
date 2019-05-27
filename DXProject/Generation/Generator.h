#pragma once
#include "Noise.h"
#include "../Voxel/BlockType.h"
#include "../Voxel/TerrainCoord.h"

class Generator {
public:
	Generator(Noise* noise) : noise_(noise) {}
	virtual ~Generator() {};
	virtual void generate(BlockType blocks[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM], int x, int y, int z) = 0;
	virtual void generate(BlockType blocks[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM], TerrainCoord coord) = 0;
protected:
	Noise* noise_;
};

