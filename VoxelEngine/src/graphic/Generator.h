#pragma once
#include "Noise.h"
#include "Chunk.h"

class Generator {
public:
	Generator(Noise* noise);
	virtual ~Generator() {};
	virtual Chunk* generate(int x, int y, int z) = 0;
	virtual Chunk* generate(glm::ivec3 coord) = 0;
	Noise* noise_;

};

