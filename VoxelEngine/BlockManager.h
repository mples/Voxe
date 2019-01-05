#pragma once
#include "Singleton.h"
#include "TextureAtlas.h"


class BlockManager : public Singleton<BlockManager> {
public:
	BlockManager();
	~BlockManager();

	void setTextureAtlas(const std::string & imageFile, std::vector<BlockType> texture_types);
	TextureAtlas* getTextureAtlas();
private:
	TextureAtlas* atlas_;
};

