#include "ChunkRenderer.h"
#include "GraphicEngine.h"
#include <math.h>
#include "Input.h"
#include <algorithm>

ChunkRenderer::ChunkRenderer() {
	shader_ = new Shader("src/shaders/vert_shader.glsl", "src/shaders/frag_shader.glsl");
	BlockManager::getInstance().setTextureAtlas("res/textures/texture_atlas.jpg", std::vector<BlockType>({BlockType::GRASS, BlockType::GRASS_DIRT}));
	
}


ChunkRenderer::~ChunkRenderer() {
	delete shader_;
}

void ChunkRenderer::draw(World* world) {
	//std::cout << "draw chunk enter " << std::endl;
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

	for (auto chunk : renderList_) {
		glm::mat4 model = makeModelMatrix(chunk.first);
		shader_->setMat4("modelMatrix", model);
		chunk.second->draw();
	}


	activeChunks_ = renderList_;
	renderList_.clear();
	loadChunks();
	// unloadChunks();
	//std::cout << "draw chunk exit " << std::endl;
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
	//std::cout << "cull chunk enter " << std::endl;
	std::vector<glm::vec4> far_coord = GraphicEngine::getInstance().getActiveCamera()->getFrustum().getFarCoord();
	std::vector<glm::vec4> near_coord = GraphicEngine::getInstance().getActiveCamera()->getFrustum().getNearCoord();

	std::vector<int> x_coord;
	std::vector<int> y_coord;
	std::vector<int> z_coord;
	for (auto v : far_coord) {
		glm::ivec3 temp = World::getChunkCoord(v.x, v.y, v.z);
		x_coord.push_back(temp.x);
		y_coord.push_back(temp.y);
		z_coord.push_back(temp.z);
	}

	for (auto v : near_coord) {
		glm::ivec3 temp = World::getChunkCoord(v.x, v.y, v.z);
		x_coord.push_back(temp.x);
		y_coord.push_back(temp.y);
		z_coord.push_back(temp.z);
	}

	auto x_minmax = std::minmax_element(x_coord.begin(), x_coord.end());

	int x_min = *(x_minmax.first);
	int x_max = *(x_minmax.second);

	auto y_minmax = std::minmax_element(y_coord.begin(), y_coord.end());

	int y_min = *(y_minmax.first);
	int y_max = *(y_minmax.second);

	auto z_minmax = std::minmax_element(z_coord.begin(), z_coord.end());

	int z_min = *(z_minmax.first);
	int z_max = *(z_minmax.second);

	//std::cout << "x min: " << x_min << "x max: " << x_max << "y min: " << y_min << "y max: " << y_max << "z min: " << z_min << "z max: " << z_max << std::endl;

	for (int x = x_min; x <= x_max; ++x) {
		for (int y = y_min; y <= y_max; ++y) {
			for (int z = z_min; z <= z_max; ++z) {
				auto find = activeChunks_.find(ChunkCoord(x, y, z));
				if (find == activeChunks_.end()) {
					loadList_.push_back(ChunkCoord(x, y, z));
				}
				else {
					renderList_.insert(std::make_pair(find->first, find->second));
					activeChunks_.erase(find);
				}
				
			}
		}
	}
	unloadList_.swap(activeChunks_);
	activeChunks_.clear();
	//std::cout << "cull chunk exit " << std::endl;
}

void ChunkRenderer::unloadChunks() {
	//std::cout << "unload chunk enter "<< unloadList_.size() << std::endl;
	static int max_unload_number = 3;
	int count = 0;
	for (auto chunk : unloadList_) {
		if (count >= max_unload_number)
			break;
		delete chunk.second;
		++count;
	}
	unloadList_.clear();
	//std::cout << "load chunk exit " << std::endl;
}

void ChunkRenderer::loadChunks() {
	//std::cout << "load chunk enter " << std::endl;
	static int max_load_number = 3;
	int count = 0;
	for (auto chunk_coord : loadList_) {
		if (count >= max_load_number)
			break;
		renderList_.insert(std::make_pair(chunk_coord, GraphicEngine::getInstance().getActiveWorld()->getChunk(chunk_coord)));
		++count;
	}
	loadList_.clear();
	//std::cout << "unloa chunk exit " << std::endl;
}
