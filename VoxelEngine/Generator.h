#pragma once
#include "Chunk.h"
#include "PerlinNoise.h"

class Generator {
public:
	Generator();
	~Generator();
	Chunk* generate(int x, int y, int z);
	PerlinNoise noise_;

private:
	glm::vec2 chunkToNoiseCoord(int x, int z);
};

