#include "SimpleGenerator.h"



SimpleGenerator::SimpleGenerator(Noise* noise) : Generator(noise) {
}


SimpleGenerator::~SimpleGenerator() {
	if (noise_ != nullptr) {
		delete noise_;
	}

}

void SimpleGenerator::generate(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & blocks, int x, int y, int z) {

	for (int i = 0; i < TERRAIN_CHUNK_DIM; ++i) {
		for (int j = 0; j < TERRAIN_CHUNK_DIM; ++j) {
			float height = noise_->noise((x * static_cast<float>(TERRAIN_CHUNK_DIM) + i) / static_cast<float>(TERRAIN_CHUNK_DIM), (z * static_cast<float>(TERRAIN_CHUNK_DIM) + j) / static_cast<float>(TERRAIN_CHUNK_DIM));
			int terr_height = std::roundf(height);

			for (int k = 0; (y * (int)TERRAIN_CHUNK_DIM + k <= terr_height && k < (int)TERRAIN_CHUNK_DIM); ++k) {
				blocks.at(i,k,j) = BlockType::GRASS_DIRT;
			}

		}
	}
}

void SimpleGenerator::generate(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> & blocks, TerrainCoord coord) {
	return generate(blocks, coord.x_, coord.y_, coord.z_);
}

