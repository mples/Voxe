#include "ChunkRenderer.h"
#include <math.h>
#include <algorithm>

ChunkRenderer::ChunkRenderer() : device_(nullptr), deviceContext_(nullptr) {
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
				renderList_.insert(std::make_pair( ChunkCoord(i, j, k), chunk));
			}
		}
	}

	return true;
}

void ChunkRenderer::draw(const DirectX::XMMATRIX & view_proj_matrix) {
		
	//cullChunks();

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

DirectX::XMMATRIX ChunkRenderer::makeModelMatrix(ChunkCoord coord) {
	DirectX::XMMATRIX amt = DirectX::XMMatrixTranslation(static_cast<float>( coord.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.z_ * static_cast<int>(Chunk::DIM)));
	return DirectX::XMMatrixTranslation(static_cast<float>(coord.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.z_ * static_cast<int>(Chunk::DIM)));
}

void ChunkRenderer::cullChunks() {
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
