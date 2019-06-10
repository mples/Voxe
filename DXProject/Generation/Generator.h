#pragma once
#include "Noise.h"
#include "../Voxel/BlockType.h"
#include "../Voxel/TerrainCoord.h"
#include "../Utilities/Structures/Array3D.h"

class Generator {
public:
	Generator(Noise* noise) : noise_(noise) {}
	virtual ~Generator() {};
	virtual void generate(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & blocks, int x, int y, int z) = 0;
	virtual void generate(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & blocks, TerrainCoord coord) = 0;
protected:
	Noise* noise_;
};

