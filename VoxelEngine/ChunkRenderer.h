#pragma once
#include "Shader.h"
#include "Chunk.h"
#include "World.h"

class ChunkRenderer {
public:
	ChunkRenderer();
	~ChunkRenderer();

	void draw(World* world);
private:
	Shader* shader_;
};

