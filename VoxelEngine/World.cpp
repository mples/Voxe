#include "World.h"
#include <cmath>
#include "GraphicEngine.h"

World::World() {
	GraphicEngine::getInstance().setWorld(this);
}


World::~World() {
}

void World::draw(Shader* shader) {
	for (auto chunk : chunks_) {
		glm::mat4 model = makeModelMatrix(chunk.first);
		shader->setMat4("modelMatrix", model);
		chunk.second->draw();
	}
}

void World::setBlock(int x, int y, int z, BlockType type) {
	ChunkCoord coord = getChunkCoord(x, y, z);
	auto found = chunks_.find(coord);
	if (found == chunks_.end()) {
		Chunk* new_chunk = new Chunk();
		chunks_.insert(std::make_pair(coord, new_chunk));
		new_chunk->setBlock(x % Chunk::CHUNK_DIM, y % Chunk::CHUNK_DIM, z % Chunk::CHUNK_DIM, type);
		return;
	}
	found->second->setBlock(x % Chunk::CHUNK_DIM, y % Chunk::CHUNK_DIM, z % Chunk::CHUNK_DIM, type);
}

std::unordered_map<ChunkCoord, Chunk*>& World::getChunks() {
	return chunks_;
}

glm::mat4 World::makeModelMatrix(ChunkCoord coord) {
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(coord.x, coord.x, coord.z));

	return model;
}

ChunkCoord World::getChunkCoord(int x, int y, int z) {
	return ChunkCoord(std::floor(x / Chunk::CHUNK_DIM) , std::floor(y / Chunk::CHUNK_DIM), std::floor(z / Chunk::CHUNK_DIM));
}
