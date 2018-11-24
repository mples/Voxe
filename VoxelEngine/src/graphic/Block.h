#pragma once
#include <glm/glm.hpp>
#include "BlockType.h"
#include "GraphicModel.h"

class Block
{
public:
	Block(glm::vec3 pos, glm::vec3 dim, BlockType type);
	~Block();
	void draw();
private:
	glm::vec3 position_;
	BlockType type_;
	GraphicModel quadModel_;

	GraphicModel& genQuadModel(glm::vec3 dim, BlockType type);
};

