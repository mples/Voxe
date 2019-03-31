#include "Texture.h"
#include<glad/glad.h>
#include <iostream>

Texture::Texture(const std::string & imageFile) {
	loadFromFile(imageFile);
}

void Texture::loadFromFile(const std::string & imageFile)
{
	int image_height, image_width;
	int ncolor_channels;

	glGenTextures(1, &textureId_);
	glBindTexture(GL_TEXTURE_2D, textureId_);

	glGenerateMipmap(GL_TEXTURE_2D);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	unsigned char* image_data = stbi_load(imageFile.c_str(), &image_width, &image_height, &ncolor_channels, 0);
	if (image_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Error: failed to load image." << std::endl;
	}

	stbi_image_free(image_data);
}


Texture::~Texture() {
	glDeleteTextures(1, &textureId_);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, textureId_);
}
