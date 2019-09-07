#pragma once
#include <string>
#include <unordered_map>
#include <algorithm>

static const unsigned int TERRAIN_CHUNK_DIM = 16;

using Block_t = unsigned char;
enum class BlockType : Block_t{
	AIR = 0,
	GRASS,
	GRASS_DIRT
};

