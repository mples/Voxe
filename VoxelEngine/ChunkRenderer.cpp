#include "ChunkRenderer.h"
#include "GraphicEngine.h"



ChunkRenderer::ChunkRenderer() {
	shader_ = new Shader("src/shaders/vert_shader.glsl", "src/shaders/frag_shader.glsl");
}


ChunkRenderer::~ChunkRenderer() {
	delete shader_;
}

void ChunkRenderer::draw(World* world) {
	shader_->activate();

	shader_->setInt("texture1", 0);

	Texture texture(blockTypeToTextFile(BlockType::GRASS));
	texture.bind();
	/*
	glm::mat4 model(1);
	shader_->setMat4("modelMatrix", model);
	*/
	Camera * camera = GraphicEngine::getInstance().getActiveCamera();
	if (camera) {
		shader_->setMat4("viewMatrix", camera->getViewMatrix());
		shader_->setMat4("projMatrix", camera->getProjMatrix());
	}

	world->draw(shader_);
}
