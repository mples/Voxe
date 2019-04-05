#pragma once
#include "Generator.h"

class SimpleGenerator : public Generator {
public:
	SimpleGenerator(Noise* noise);
	~SimpleGenerator();
	Chunk* generate(int x, int y, int z) override;
	Chunk* generate(ChunkCoord coord) override;

private:
	DirectX::XMFLOAT2 chunkToNoiseCoord(int x, int z);
};

