#pragma once
#include "Noise.h"
#include "../Chunk.h"
#include "ChunkCoord.h"

class Generator {
public:
	Generator(Noise* noise) : noise_(noise) {}
	virtual ~Generator() {};
	virtual Chunk* generate(int x, int y, int z) = 0;
	virtual Chunk* generate(ChunkCoord coord) = 0;
protected:
	Noise* noise_;
};

