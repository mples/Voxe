#include "Generator.h"



Generator::Generator() : noise_(102) {

}


Generator::~Generator() {
}

Chunk * Generator::generate(int x, int y, int z) {
	/*int max_height = 100;
	int min_height = -100;

	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			glm::vec2 vec((float)i / (Chunk::CHUNK_DIM * 100), (float)j / (Chunk::CHUNK_DIM * 100));
			float height = noise_.eval(vec) * 20000;
			if (height > max_height)
				height = max_height;
			if (height < min_height)
				height = min_height;
			world->setBlock(i, (int) height, j, BlockType::GRASS);
		}
	}
	*/
	Chunk * chunk = new Chunk();
	
	for (int i = 0; i < Chunk::CHUNK_DIM; ++i) {
		for (int j = 0; j < Chunk::CHUNK_DIM; ++j) {
			float height = noise_.eval(chunkToNoiseCoord(x * Chunk::CHUNK_DIM + i, z * Chunk::CHUNK_DIM + j) );
			height *= 2000;
			int terr_height = std::roundf(height);
			for (int k = y * Chunk::CHUNK_DIM; k <= terr_height ; ++k) {
				chunk->setBlock(i, k, j, BlockType::GRASS);
			}

		}
	}
	return chunk;
}

glm::vec2 Generator::chunkToNoiseCoord(int x, int z) {

	return glm::vec2 ((float)x / (Chunk::CHUNK_DIM * 100), (float)z / (Chunk::CHUNK_DIM * 100));
}
