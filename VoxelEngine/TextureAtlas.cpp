#include "TextureAtlas.h"



TextureAtlas::TextureAtlas(const std::string & imageFile, std::vector<BlockType> texture_types)  {
	int ncolor_channels;

	glGenTextures(1, &textureId_);
	glBindTexture(GL_TEXTURE_2D, textureId_);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


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
	sideHeight_ = (float) imageHeight_ / texture_count;
	sideWidth_ = (float)imageWidth_ / texture_count;
	for (unsigned int i = 0; i < texture_count; ++i) {
		textures_.insert(std::make_pair(texture_types[i], i));
	}
}


TextureAtlas::~TextureAtlas() {
	glDeleteTextures(1, &textureId_);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, textureId_);
}

std::vector<std::pair<TextureVertex, glm::vec2>> TextureAtlas::getTextureCoord(BlockType type, TextureSide side) {
	auto found = textures_.find(type);
	if (found == textures_.end()) {
		return std::vector<std::pair<TextureVertex, glm::vec2>>();
	}
	std::vector<std::pair<TextureVertex, glm::vec2>> text_coord;
	unsigned int height = (unsigned int) side;
	
	text_coord.push_back(std::make_pair(TextureVertex::_00, glm::vec2((height * sideHeight_) / imageHeight_, (found->second * sideWidth_ ) / imageWidth_) ) );
	text_coord.push_back(std::make_pair(TextureVertex::_01, glm::vec2( ((height + 1) * sideHeight_) / imageHeight_, (found->second * sideWidth_) / imageWidth_)));
	text_coord.push_back(std::make_pair(TextureVertex::_10, glm::vec2((height * sideHeight_) / imageHeight_, ((found->second + 1) * sideWidth_) / imageWidth_ )));
	text_coord.push_back(std::make_pair(TextureVertex::_00, glm::vec2(((height + 1) * sideHeight_) / imageHeight_, ((found->second + 1) * sideWidth_) / imageWidth_)));

	return text_coord;
	
}
