#pragma once
#include "glm/glm.hpp"
#include "Chunk.h"
#include "PerlinNoise.h"

class Generator {
public:
	Generator();
	~Generator();
	Chunk* generate(int x, int y, int z);
	Chunk* generate(glm::ivec3 coord);
	PerlinNoise noise_;

private:
	glm::vec2 chunkToNoiseCoord(int x, int z);
};

