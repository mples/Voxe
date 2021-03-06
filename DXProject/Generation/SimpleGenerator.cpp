#include "SimpleGenerator.h"



SimpleGenerator::SimpleGenerator(Noise* noise) : Generator(noise) {
}


SimpleGenerator::~SimpleGenerator() {
}

void SimpleGenerator::generate(BlockType blocks[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM], int x, int y, int z) {

	for (int i = 0; i < TERRAIN_CHUNK_DIM; ++i) {
		for (int j = 0; j < TERRAIN_CHUNK_DIM; ++j) {
			float height = noise_->noise((x * static_cast<float>(TERRAIN_CHUNK_DIM) + i) / static_cast<float>(TERRAIN_CHUNK_DIM), (z * static_cast<float>(TERRAIN_CHUNK_DIM) + j) / static_cast<float>(TERRAIN_CHUNK_DIM));
			int terr_height = std::roundf(height);

			for (int k = 0; (y * (int)TERRAIN_CHUNK_DIM + k <= terr_height && k < (int)TERRAIN_CHUNK_DIM); ++k) {
				blocks[i][k][j] = BlockType::GRASS;
			}

		}
	}
}

void SimpleGenerator::generate(BlockType blocks[TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM][TERRAIN_CHUNK_DIM], TerrainCoord coord) {
	return generate(blocks, coord.x_, coord.y_, coord.z_);
}

