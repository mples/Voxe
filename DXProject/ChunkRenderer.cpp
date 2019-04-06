#include "ChunkRenderer.h"
#include <math.h>
#include <algorithm>

ChunkRenderer::ChunkRenderer() : device_(nullptr), deviceContext_(nullptr), enableCull_(true) {
}


ChunkRenderer::~ChunkRenderer() {
}

bool ChunkRenderer::initialize(ID3D11Device * device, ID3D11DeviceContext * device_context) {
	device_ = device;
	deviceContext_ = device_context;

	texture_ = new Texture(device_, L"Data/Textures/grass.jpg", aiTextureType_DIFFUSE);
	HRESULT hr = CBVSObject_.initialize(device, device_context);
	COM_ERROR_IF_FAILED(hr, L"Falied to initialize constant buffer.");

	//debug
	for (int i = -5; i < 5; i++) {
		for (int j = -5; j < 5; j++) {
			for (int k = -5; k < 5; k++) {
				Chunk * chunk = world_->getChunk(i, j, k);
				chunk->initialize(device_, deviceContext_, CBVSObject_, texture_);
				activeChunks_.insert(std::make_pair( ChunkCoord(i, j, k), chunk));
			}
		}
	}

	chunkContext_.addActionMapping(Action::CULL, KeyboardEvent(KeyboardEvent::Type::PRESS, 'C'));

	INPUT.addFrontContext(&chunkContext_);
	InputCallback callback = [=](MappedInput& input) {
		auto cull = input.actions_.find(Action::CULL);
		if (cull != input.actions_.end()) {
			this->setEnableCulling(false);
			input.actions_.erase(cull);

		}
	};
	INPUT.addCallback(callback, InputCallbackPriority::HIGH);

	return true;
}

void ChunkRenderer::draw(const DirectX::XMMATRIX & view_proj_matrix, BoundingFrustum & frustum) {
	
	if (enableCull_) {
		cullChunks(frustum);
	}

	for (auto chunk : renderList_) {
		chunk.second->draw(makeModelMatrix(chunk.first), view_proj_matrix);
	}


	//activeChunks_ = renderList_;
	//renderList_.clear();
	//loadChunks();
	//rebuildChunks();
	// unloadChunks();
}

void ChunkRenderer::setWorld(World * world) {
	if (world != nullptr){
		world_ = world;		
	}
}

void ChunkRenderer::setEnableCulling(bool state) {
	enableCull_ = state;
}

DirectX::XMMATRIX ChunkRenderer::makeModelMatrix(ChunkCoord coord) {
	DirectX::XMMATRIX amt = DirectX::XMMatrixTranslation(static_cast<float>( coord.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.z_ * static_cast<int>(Chunk::DIM)));
	return DirectX::XMMatrixTranslation(static_cast<float>(coord.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.z_ * static_cast<int>(Chunk::DIM)));
}

void ChunkRenderer::cullChunks(BoundingFrustum & frustum) {
	/*std::vector<glm::vec4> far_coord = GraphicEngine::getInstance().getActiveCamera()->getFrustum().getFarCoord();
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


	for (int x = x_min; x <= x_max; ++x) {
		for (int y = y_min; y <= y_max; ++y) {
			for (int z = z_min; z <= z_max; ++z) {
				auto find = activeChunks_.find(ChunkCoord(x, y, z));
				if (find == activeChunks_.end()) {
					loadList_.push_back(ChunkCoord(x, y, z));
				}
				else {
					renderList_.insert(std::make_pair(find->first, find->second));
					if (find->second->chagned()) {
						rebuildList_.push_back(find->second);
					}
					activeChunks_.erase(find);
				}
				
			}
		}
	}
	unloadList_.swap(activeChunks_);
	activeChunks_.clear();*/
	renderList_ = activeChunks_;
	

	std::unordered_map<ChunkCoord, Chunk*> new_list;
	for (auto chunk : renderList_) {
		XMMATRIX world_mat = makeModelMatrix(chunk.first);
		XMMATRIX inv_world = XMMatrixInverse(&XMMatrixDeterminant(world_mat), world_mat);

		XMVECTOR scale;
		XMVECTOR rotation;
		XMVECTOR tranlation;

		XMMatrixDecompose(&scale, &rotation, &tranlation, world_mat);
		BoundingFrustum world_frustum;
		frustum.Transform(world_frustum, XMVectorGetX(scale), rotation, tranlation);
		

		ContainmentType contains = world_frustum.Contains(chunk.second->getBoundingBox());
		if (contains != ContainmentType::DISJOINT) {
			new_list.insert(chunk);
		}
	}
	renderList_.clear();
	renderList_ = new_list;
}

void ChunkRenderer::unloadChunks() {
	static int max_unload_number = 3;
	int count = 0;
	for (auto chunk : unloadList_) {
		if (count >= max_unload_number)
			break;
		delete chunk.second;
		++count;
	}
	unloadList_.clear();
}

void ChunkRenderer::loadChunks() {
	static int max_load_number = 3;
	int count = 0;
	for (auto chunk_coord : loadList_) {
		if (count >= max_load_number)
			break;
		renderList_.insert(std::make_pair(chunk_coord, world_->getChunk(chunk_coord)));
		++count;
	}
	loadList_.clear();
}

void ChunkRenderer::rebuildChunks() {
	static int max_rebuild_number = 3;
	int count = 0;
	for (auto chunk : rebuildList_) {
		if (count >= max_rebuild_number)
			break;
		chunk->update();
		++count;
	}
	rebuildList_.clear();
}
