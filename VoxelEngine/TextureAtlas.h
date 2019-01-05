#pragma once
#include <vector>
#include <unordered_map>
#include <BlockType.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include "stb_image.h"
enum class TextureSide : unsigned int {
	TOP = 5,
	BOTTOM = 4,
	LEFT = 3,
	RIGHT = 2,
	FRONT = 1,
	BACK = 0
};

enum class TextureVertex {
	_00,
	_01,
	_10,
	_11
};
class TextureAtlas  {
public:
	TextureAtlas(const std::string & imageFile, std::vector<BlockType> texture_types);
	~TextureAtlas();
	std::vector<std::pair<TextureVertex, float>> getTextureCoord(BlockType type, TextureSide side);
private:
	unsigned int textureId_;
	std::unordered_map<BlockType, unsigned int> textures_;
	unsigned int sideHeight_;
	unsigned int sideWidth_;
};

