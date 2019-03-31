#pragma once
#include <string>
#include "stb_image.h"

class Texture
{
public:
	Texture(const std::string & imageFile);
	~Texture();
	void bind();
private:
	void loadFromFile(const std::string & imageFile);

	unsigned int textureId_;
};

