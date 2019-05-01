#pragma once
#include <string>
#include <unordered_map>
#include <algorithm>


using Block_t = unsigned char;
enum class BlockType : Block_t{
	AIR = 0,
	GRASS,
	GRASS_DIRT
};

inline std::string&  blockTypeToTextFile(BlockType type) {
	static std::unordered_map<BlockType, std::string> textures = {
		{ BlockType::GRASS, "res/textures/grass.jpg" },
		{ BlockType::AIR, "res/textures/grass.jpg" }
	};
	auto found = textures.find(type);
	if (found != textures.end()) {
		return (*found).second;
	}
	return textures.at(BlockType::AIR);
}
