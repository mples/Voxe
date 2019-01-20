#include "BlockManager.h"



BlockManager::BlockManager() : Singleton<BlockManager>() {
}


BlockManager::~BlockManager() {
	if (atlas_ != nullptr) {
		delete atlas_;
	}
}

void BlockManager::setTextureAtlas(const std::string & imageFile, std::vector<BlockType> texture_types) {
	if (atlas_ != nullptr) {
		delete atlas_;
	}
	atlas_ = new TextureAtlas(imageFile, texture_types);
}

TextureAtlas * BlockManager::getTextureAtlas() {
	return atlas_;
}
