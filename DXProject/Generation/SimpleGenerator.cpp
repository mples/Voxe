#include "SimpleGenerator.h"



SimpleGenerator::SimpleGenerator(Noise* noise) : Generator(noise) {
}


SimpleGenerator::~SimpleGenerator() {
}

void SimpleGenerator::generate(BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM], int x, int y, int z) {

	//Chunk * chunk = new Chunk(x, y, z);
	for (int i = 0; i < Chunk::DIM; ++i) {
		for (int j = 0; j < Chunk::DIM; ++j) {
			//float height = noise_->eval(chunkToNoiseCoord(x * Chunk::DIM + i, z * Chunk::DIM + j));
			float height = -5;
			//height *= 20;
			int terr_height = std::roundf(height);

			for (int k = 0; (y * (int)Chunk::DIM + k <= terr_height && k < (int)Chunk::DIM); ++k) {
				//chunk->setBlock(i, k, j, BlockType::GRASS_DIRT);
				blocks[i][k][j] = BlockType::GRASS;
			}

		}
	}
	//return chunk;
}

void SimpleGenerator::generate(BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM], ChunkCoord coord) {
	return generate(blocks, coord.x_, coord.y_, coord.z_);
}

DirectX::XMFLOAT2 SimpleGenerator::chunkToNoiseCoord(int x, int z) {

	return DirectX::XMFLOAT2((float)x / (Chunk::DIM * 10), (float)z / (Chunk::DIM * 10));
}