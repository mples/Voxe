#pragma once
#include <vector>
#include <glad/glad.h>

class GraphicModel {
public:
	GraphicModel();
	GraphicModel(const std::vector<float>& vertices, const std::vector<float>& texture_coord);
	~GraphicModel();
	void loadData(const std::vector<float>& vertices, const std::vector<float>& texture_coord);
	void bindVAO();
private:
	unsigned int vaoId_;
	int vboCount_;
	int indicesCount_;

	void loadVBO(int dim, const std::vector<float>& data);
	void loadEBO(const std::vector<unsigned int>& indices);
	std::vector<unsigned int> buffers;

};

