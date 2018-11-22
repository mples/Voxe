#pragma once
#include <string>
#include "stb_image.h"

class Texture
{
public:
	Texture(std::string & imageFile);
	~Texture();
	void bind();
private:
	void loadFromFile(std::string & imageFile);

	unsigned int textureId_;
};

