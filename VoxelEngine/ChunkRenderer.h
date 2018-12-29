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

	glm::mat4 makeModelMatrix(ChunkCoord  coord);
	bool isVisible(ChunkCoord coord, glm::mat4 mvp);
};

