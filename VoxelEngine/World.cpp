#include "World.h"
#include <cmath>
#include "GraphicEngine.h"

World::World() {
	GraphicEngine::getInstance().setWorld(this);
}


World::~World() {
}


void World::setBlock(int x, int y, int z, BlockType type) {
	//std::cout << "World x: " << x << " y: " << y << " z: " << z << std::endl;
	ChunkCoord coord = getChunkCoord(x, y, z);
	auto found = chunks_.find(coord);
	if (found == chunks_.end()) {
		Chunk* new_chunk = new Chunk();
		chunks_.insert(std::make_pair(coord, new_chunk));
		setAdjacentChunks(new_chunk, coord);
		new_chunk->setBlock(x % Chunk::CHUNK_DIM, y % Chunk::CHUNK_DIM, z % Chunk::CHUNK_DIM, type);
		return;
	}
	found->second->setBlock(x % Chunk::CHUNK_DIM, y % Chunk::CHUNK_DIM, z % Chunk::CHUNK_DIM, type);
}

std::unordered_map<ChunkCoord, Chunk*>& World::getChunks() {
	return chunks_;
}

ChunkCoord World::getChunkCoord(int x, int y, int z) {
	return ChunkCoord(std::floor((double)x / Chunk::CHUNK_DIM) , std::floor((double)y / Chunk::CHUNK_DIM), std::floor((double)z / Chunk::CHUNK_DIM));
}

void World::setAdjacentChunks(Chunk * chunk, ChunkCoord& coord) {
	ChunkCoord left_coord = ChunkCoord(coord.x -1 , coord.y, coord.z);
	Chunk* left_chunk = getChunk(left_coord);
	if (left_chunk) {
		chunk->left_ = left_chunk;
		left_chunk->right_ = chunk;
	}

	ChunkCoord right_coord = ChunkCoord(coord.x + 1, coord.y, coord.z);
	Chunk* right_chunk = getChunk(right_coord);
	if (right_chunk) {
		chunk->right_ = right_chunk;
		right_chunk->left_= chunk;
	}

	ChunkCoord up_coord = ChunkCoord(coord.x, coord.y + 1, coord.z);
	Chunk* up_chunk = getChunk(up_coord);
	if (up_chunk) {
		chunk->up_ = up_chunk;
		up_chunk->down_ = chunk;
	}

	ChunkCoord down_coord = ChunkCoord(coord.x, coord.y - 1, coord.z);
	Chunk* down_chunk = getChunk(down_coord);
	if (down_chunk) {
		chunk->down_= down_chunk;
		down_chunk->up_ = chunk;
	}

	ChunkCoord front_coord = ChunkCoord(coord.x, coord.y, coord.z  - 1);
	Chunk* front_chunk = getChunk(front_coord);
	if (front_chunk) {
		chunk->front_= front_chunk;
		front_chunk->back_ = chunk;
	}

	ChunkCoord back_coord = ChunkCoord(coord.x, coord.y, coord.z + 1);
	Chunk* back_chunk = getChunk(back_coord);
	if (back_chunk) {
		chunk->back_ = back_chunk;
		back_chunk->front_ = chunk;
	}

}

Chunk * World::getChunk(ChunkCoord& coord) {
	auto found = chunks_.find(coord);
	if (found != chunks_.end()) {
		return found->second;
	}
	return nullptr;
}
