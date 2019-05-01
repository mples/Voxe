#pragma once
#include "Noise.h"
#include "../Voxel/Chunk.h"
#include "../Voxel/ChunkCoord.h"

class Generator {
public:
	Generator(Noise* noise) : noise_(noise) {}
	virtual ~Generator() {};
	virtual void generate(BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM], int x, int y, int z) = 0;
	virtual void generate(BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM], ChunkCoord coord) = 0;
protected:
	Noise* noise_;
};

