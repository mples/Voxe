#pragma once
#include "Shader.h"
#include "Chunk.h"
#include "World.h"
#include "../../BlockManager.h"

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
	void unloadChunks();
	void loadChunks();

	std::unordered_map<ChunkCoord, Chunk*> activeChunks_;
	std::unordered_map<ChunkCoord, Chunk*> allChunks_;
	std::vector<ChunkCoord> loadList_;
	std::unordered_map<ChunkCoord, Chunk*> unloadList_;
	std::unordered_map<ChunkCoord, Chunk*> renderList_;
	std::vector<ChunkCoord> visibilityList_;
};

