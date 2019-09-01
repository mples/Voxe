#pragma once
#include "../ECS/Event/Event.h"
#include "../ECS/Component.h"
#include "../Graphics/BlocksTextureAtlas.h"

struct BlockTextureAtlasCreated : public Event<BlockTextureAtlasCreated> {
	BlockTextureAtlasCreated(BlocksTextureAtlas * ptr) : atlasPtr_(ptr) {}

	BlocksTextureAtlas * atlasPtr_;
};