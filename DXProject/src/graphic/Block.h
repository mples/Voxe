#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BlockType.h"
#include "GraphicModel.h"
#include "Shader.h"
#include "Texture.h"

class Block
{
public:
	static Shader* blockShader_;

	Block(glm::vec3 pos, glm::vec3 rot, glm::vec3 dim, BlockType type);
	~Block();
	void draw();
	glm::mat4& getModelMatrix();
private:
	glm::vec3 position_;
	glm::vec3 rotation_;
	BlockType type_;
	GraphicModel quadModel_;
	glm::mat4 modelMatrix_;
	void makeModelMatrix();
	GraphicModel& genQuadModel(glm::vec3 dim, BlockType type);
};

