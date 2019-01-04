#include "ChunkRenderer.h"
#include "GraphicEngine.h"
#include <math.h>
#include "Input.h"

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
	cullChunks();
	for (auto chunk : activeChunks_) {
		glm::mat4 model = makeModelMatrix(chunk.first);
		shader_->setMat4("modelMatrix", model);
		chunk.second->draw();
	}

}

void ChunkRenderer::setWorld(World * world) {
	if (world != nullptr){
		worldToRender_ = world;
		allChunks_ = world->getChunks();
		//activeChunks_ = worldToRender_->getChunks();
		if (GraphicEngine::getInstance().getActiveCamera() != nullptr)
			cullChunks();
		InputContext * input_context = new InputContext();

		input_context->addActionMapping(Action::CULL, RawButton(GLFW_KEY_C));

		Input::getInstance().getMapper().pushBackContext(input_context);

		Input::getInstance().getMapper().addCallback([=](MappedInput& input) {
			auto found = std::find(input.actions_.begin(), input.actions_.end(), Action::CULL);
			if (found != input.actions_.end()) {
				cullChunks();
				input.eatAction(Action::CULL);
			}
		}, InputCallbackPriority::HIGH);
	}
}

glm::mat4 ChunkRenderer::makeModelMatrix(ChunkCoord coord) {
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3((float)coord.x * Chunk::CHUNK_DIM, (float)coord.y * Chunk::CHUNK_DIM, (float)coord.z * Chunk::CHUNK_DIM));

	return model;
}

bool ChunkRenderer::isVisible(ChunkCoord coord) {
	/*glm::vec3 world_coord((float)coord.x * Chunk::CHUNK_DIM + ((float) Chunk::CHUNK_DIM /2), (float)coord.y * Chunk::CHUNK_DIM + ((float)Chunk::CHUNK_DIM / 2), (float)coord.z * Chunk::CHUNK_DIM + ((float)Chunk::CHUNK_DIM / 2));
	float dim = (float) Chunk::CHUNK_DIM / 2;
	glm::mat4 model = makeModelMatrix(coord);
	glm::vec4 model_coord = model * GraphicEngine::getInstance().getActiveCamera()->getViewMatrix() * GraphicEngine::getInstance().getActiveCamera()->getProjMatrix() * glm::vec4(coord, 1.0f);
	*/
	if(GraphicEngine::getInstance().getActiveCamera() != nullptr)
		return GraphicEngine::getInstance().getActiveCamera()->getFrustum().cubeIntersect(glm::ivec3(coord.x * Chunk::CHUNK_DIM, coord.y * Chunk::CHUNK_DIM, coord.z * Chunk::CHUNK_DIM), Chunk::CHUNK_DIM);
	else {
		//error
		return true;
	}
}

void ChunkRenderer::cullChunks() {
	activeChunks_.clear();
	for (auto it : allChunks_) {
		if (isVisible(it.first)) {
			activeChunks_.insert(it);
		}
	}		
}
