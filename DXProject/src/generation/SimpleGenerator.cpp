#include "SimpleGenerator.h"



SimpleGenerator::SimpleGenerator(Noise* noise) : Generator(noise) {
}


SimpleGenerator::~SimpleGenerator() {
}

Chunk * SimpleGenerator::generate(int x, int y, int z) {

	Chunk * chunk = new Chunk();
	for (int i = 0; i < Chunk::CHUNK_DIM; ++i) {
		for (int j = 0; j < Chunk::CHUNK_DIM; ++j) {
			float height = noise_->eval(chunkToNoiseCoord(x * Chunk::CHUNK_DIM + i, z * Chunk::CHUNK_DIM + j));
			height *= 20;
			int terr_height = std::roundf(height);

			for (int k = 0; (y * (int)Chunk::CHUNK_DIM + k <= terr_height && k < (int)Chunk::CHUNK_DIM); ++k) {
				chunk->setBlock(i, k, j, BlockType::GRASS_DIRT);
			}

		}
	}
	return chunk;
}

Chunk * SimpleGenerator::generate(glm::ivec3 coord) {
	return generate(coord.x, coord.y, coord.z);
}

glm::vec2 SimpleGenerator::chunkToNoiseCoord(int x, int z) {

	return glm::vec2((float)x / (Chunk::CHUNK_DIM * 10), (float)z / (Chunk::CHUNK_DIM * 10));
}