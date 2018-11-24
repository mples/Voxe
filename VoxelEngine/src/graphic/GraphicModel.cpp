#include "GraphicModel.h"



GraphicModel::GraphicModel() {
}

GraphicModel::GraphicModel(const std::vector<float>& vertices, const std::vector<float>& texture_coord) {
	loadData(vertices, texture_coord);
}


GraphicModel::~GraphicModel() {
}

void GraphicModel::loadData(const std::vector<float>& vertices, const std::vector<float>& texture_coord) {

	glGenVertexArrays(1, &vaoId_);

	glBindVertexArray(vaoId_);

	loadVBO(3, vertices);
	loadVBO(2, texture_coord);
}

void GraphicModel::bindVAO() {
	glBindVertexArray(vaoId_);
}

void GraphicModel::loadVBO(int dim, const std::vector<float>& data) {
	unsigned int vbo_id;

	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(vboCount_, dim, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(vboCount_);

	++vboCount_;
	buffers.push_back(vbo_id);
}

void GraphicModel::loadEBO(const std::vector<unsigned int>& indices) {
	unsigned int ebo_id;

	glGenBuffers(1, &ebo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	indicesCount_ = indices.size();
}
