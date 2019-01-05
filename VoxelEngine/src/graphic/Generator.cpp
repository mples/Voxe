#include "Generator.h"
#include <iostream>


Generator::Generator() : noise_(34702) {

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
	//std::cout << "chunk: " << x << " " << y << " " << z << std::endl;
	for (int i = 0; i < Chunk::CHUNK_DIM; ++i) {
		for (int j = 0; j < Chunk::CHUNK_DIM; ++j) {
			float height = noise_.eval(chunkToNoiseCoord(x * Chunk::CHUNK_DIM + i, z * Chunk::CHUNK_DIM + j) );
			//std::cout << "height: " << height ;
			height *= 20;
			//std::cout << " " << height;
			int terr_height = std::roundf(height);
			//std::cout << "terr height: " << terr_height << std::endl;
			//if (terr_height >= 16) {
			//	std::cout << "height is more than 16 " << std::endl;
			//}
			for (int k = 0; (y * (int)Chunk::CHUNK_DIM + k <= terr_height && k < (int)Chunk::CHUNK_DIM ); ++k) {
				//std::cout << "terr height bool: " << (y * (int)Chunk::CHUNK_DIM + k <= terr_height && k < (int)Chunk::CHUNK_DIM ) << std::endl;
				//std::cout << "setblock " << i << " " << j << " " << k << std::endl;
				//std::cout << "setblock coord: " << x * Chunk::CHUNK_DIM + i << " " << y * Chunk::CHUNK_DIM + k << " " << z * Chunk::CHUNK_DIM + j << std::endl;
				chunk->setBlock(i, k, j, BlockType::GRASS);
			}

		}
	}
	return chunk;
}

glm::vec2 Generator::chunkToNoiseCoord(int x, int z) {

	return glm::vec2 ((float)x / (Chunk::CHUNK_DIM * 10), (float)z / (Chunk::CHUNK_DIM * 10));
}
