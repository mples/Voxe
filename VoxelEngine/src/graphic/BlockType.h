#pragma once
#include <string>
#include <unordered_map>
#include <algorithm>

#include "Texture.h"

enum class BlockType {
	Grass,
	Unknown
};

inline std::string&  blockTypeToTextFile(BlockType type) {
	static std::unordered_map<BlockType, std::string> textures = {
		{ BlockType::Grass, "res/textures/grass.jpg" },
		{ BlockType::Unknown, "res/textures/grass.jpg" }
	};
	auto found = textures.find(type);
	if (found != textures.end()) {
		return (*found).second;
	}
	return textures.at(BlockType::Unknown);
}
