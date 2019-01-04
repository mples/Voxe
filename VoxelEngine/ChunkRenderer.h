#pragma once
#include "Shader.h"
#include "Chunk.h"
#include "World.h"

class ChunkRenderer {
public:
	ChunkRenderer();
	~ChunkRenderer();

	void draw(World* world);
	void setWorld(World* world);
private:
	Shader* shader_;
	World* worldToRender_;

	glm::mat4 makeModelMatrix(ChunkCoord  coord);
	bool isVisible(ChunkCoord coord);
	void cullChunks();
	std::unordered_map<ChunkCoord, Chunk*> activeChunks_;
	std::unordered_map<ChunkCoord, Chunk*> allChunks_;
};

