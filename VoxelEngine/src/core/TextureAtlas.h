#pragma once
#include <vector>
#include <unordered_map>
#include <BlockType.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include "stb_image.h"

enum class TextureSide : unsigned int {
	BACK = 0,
	FRONT = 1,
	RIGHT = 2,
	LEFT = 3,
	BOTTOM = 4,
	TOP = 5
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
	std::unordered_map<TextureVertex, glm::vec2>& getTextureCoord(BlockType type, TextureSide side);
	void bind();
private:
	void calculateCoordinates(std::vector<BlockType> texture_types);

	unsigned int textureId_;
	std::unordered_map<BlockType, std::vector<std::unordered_map<TextureVertex, glm::vec2>>> textCoord_;
	float sideHeight_;
	float sideWidth_;
	int imageHeight_;
	int imageWidth_;
};

