#include "TextureAtlas.h"



TextureAtlas::TextureAtlas(const std::string & imageFile, std::vector<BlockType> texture_types)  {
	int ncolor_channels;

	glGenTextures(1, &textureId_);
	glBindTexture(GL_TEXTURE_2D, textureId_);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* image_data = stbi_load(imageFile.c_str(), &imageWidth_, &imageHeight_, &ncolor_channels, 0);
	if (image_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth_, imageHeight_, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Error: failed to load image." << std::endl;
	}

	stbi_image_free(image_data);

	unsigned int texture_count = texture_types.size();
	sideHeight_ = (float) imageHeight_ / 6;
	sideWidth_ = (float)imageWidth_ / texture_count;
	calculateCoordinates(texture_types);
}


TextureAtlas::~TextureAtlas() {
	glDeleteTextures(1, &textureId_);
}

void TextureAtlas::bind() {
	glBindTexture(GL_TEXTURE_2D, textureId_);
}

std::unordered_map<TextureVertex, glm::vec2>& TextureAtlas::getTextureCoord(BlockType type, TextureSide side) {
	
	return textCoord_.at(type).at((int)side);
}

void TextureAtlas::calculateCoordinates(std::vector<BlockType> texture_types) {
	for (int tex_num = 0; tex_num < texture_types.size(); ++tex_num) {
		std::vector<std::unordered_map<TextureVertex, glm::vec2>> temp;
		for (int side = 0; side < 6; ++side) {
			std::unordered_map<TextureVertex, glm::vec2> text_coord;
			int height = tex_num;
			int width =  5 - side;
			text_coord.insert(std::make_pair(TextureVertex::_01, glm::vec2(((height + 1) * sideWidth_) / imageWidth_, (width * sideHeight_) / imageHeight_)));
			text_coord.insert(std::make_pair(TextureVertex::_00, glm::vec2(((height + 1) * sideWidth_) / imageWidth_, ((width + 1) * sideHeight_) / imageHeight_)));
			text_coord.insert(std::make_pair(TextureVertex::_11, glm::vec2((height * sideWidth_) / imageWidth_, (width * sideHeight_) / imageHeight_)));
			text_coord.insert(std::make_pair(TextureVertex::_10, glm::vec2((height * sideWidth_) / imageWidth_, ((width + 1) * sideHeight_) / imageHeight_)));
			temp.push_back(text_coord);
		}
		textCoord_.insert(std::make_pair(texture_types[tex_num], temp));
	}
}
