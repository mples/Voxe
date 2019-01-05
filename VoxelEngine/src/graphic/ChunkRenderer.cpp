#include "ChunkRenderer.h"
#include "GraphicEngine.h"
#include <math.h>
#include "Input.h"

ChunkRenderer::ChunkRenderer() {
	shader_ = new Shader("src/shaders/vert_shader.glsl", "src/shaders/frag_shader.glsl");
	BlockManager::getInstance().setTextureAtlas("res/textures/texture_atlas.jpg", std::vector<BlockType>({BlockType::GRASS, BlockType::GRASS_DIRT}));
	
}


ChunkRenderer::~ChunkRenderer() {
	delete shader_;
}

void ChunkRenderer::draw(World* world) {
	shader_->activate();

	shader_->setInt("texture1", 0);

	BlockManager::getInstance().getTextureAtlas()->bind();
	//Texture texture(blockTypeToTextFile(BlockType::GRASS));
	//texture.bind();

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
		//debug code
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
	coord = glm::ivec3(coord.x * Chunk::CHUNK_DIM, coord.y * Chunk::CHUNK_DIM, coord.z * Chunk::CHUNK_DIM);
	coord = glm::ivec3(coord.x + Chunk::CHUNK_DIM / 2, coord.y + Chunk::CHUNK_DIM / 2, coord.z + Chunk::CHUNK_DIM / 2);
	if (GraphicEngine::getInstance().getActiveCamera() != nullptr)
		return GraphicEngine::getInstance().getActiveCamera()->getFrustum().sphereIntersect(coord, Chunk::CHUNK_DIM * 2.0);
	else {
		
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
