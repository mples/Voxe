#include "ChunkRenderer.h"
#include "GraphicEngine.h"
#include <math.h>


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

	Camera * camera = GraphicEngine::getInstance().getActiveCamera();
	if (camera) {
		shader_->setMat4("viewMatrix", camera->getViewMatrix());
		shader_->setMat4("projMatrix", camera->getProjMatrix());
	}

	std::unordered_map<ChunkCoord, Chunk*> chunks_map = world->getChunks();
	for (auto chunk : chunks_map) {
		glm::mat4 model = makeModelMatrix(chunk.first);
		shader_->setMat4("modelMatrix", model);
		//if (isVisible(chunk.first, model * camera->getViewMatrix() * camera->getProjMatrix())) {
			chunk.second->draw();
		//}
	}

}

glm::mat4 ChunkRenderer::makeModelMatrix(ChunkCoord coord) {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3((float)coord.x * Chunk::CHUNK_DIM, (float)coord.y * Chunk::CHUNK_DIM, (float)coord.z * Chunk::CHUNK_DIM));

	return model;
}

bool ChunkRenderer::isVisible(ChunkCoord coord, glm::mat4 mvp) {
	float diameter = sqrtf(3.0f * Chunk::CHUNK_DIM * Chunk::CHUNK_DIM);
	
	glm::vec4 clip = mvp * glm::vec4(coord, 1);
	clip.x -= Chunk::CHUNK_DIM / 2;
	clip.y -= Chunk::CHUNK_DIM / 2;
	clip.z -= Chunk::CHUNK_DIM / 2;

	clip.x /= clip.w ;
	clip.y /= clip.w ;

	//if (clip.z > 0) {
		//std::cout << "dia: " << diameter << "clip z: " << clip.z << "coord z: " << coord.z  << std::endl;
	//	return false;
	//}

	diameter /= fabsf(clip.w);

	if (fabsf(clip.x) > 1 + diameter || fabsf(clip.y > 1 + diameter) ) {
		return false;
	}
	else {
		return true;
	}
}
