#pragma once
#include "Generator.h"

class SimpleGenerator : public Generator {
public:
	SimpleGenerator(Noise* noise);
	~SimpleGenerator();
	void generate(BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM], int x, int y, int z) override;
	void generate(BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM], ChunkCoord coord) override;

private:
	DirectX::XMFLOAT2 chunkToNoiseCoord(int x, int z);
};

