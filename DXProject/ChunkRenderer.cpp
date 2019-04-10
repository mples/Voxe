#include "ChunkRenderer.h"
#include <math.h>
#include <algorithm>

ChunkRenderer::ChunkRenderer() : device_(nullptr), deviceContext_(nullptr), enableCull_(true), octree_(BoundingBox(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(500.0f, 500.0f, 500.0f))) {
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
				activeChunks_.push_back(chunk);
				octree_.insert(chunk);
			}
		}
	}

	chunkContext_.addActionMapping(Action::CULL, KeyboardEvent(KeyboardEvent::Type::PRESS, 'C'));

	INPUT.addFrontContext(&chunkContext_);
	InputCallback callback = [=](MappedInput& input) {
		auto cull = input.actions_.find(Action::CULL);
		if (cull != input.actions_.end()) {
			this->setEnableCulling(false);
			for (int i = 0; i < 500; i++) {
				this->octree_.remove(activeChunks_[i]);

			}
			input.actions_.erase(cull);

		}
	};
	INPUT.addCallback(callback, InputCallbackPriority::HIGH);

	return true;
}

void ChunkRenderer::draw(const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix, BoundingFrustum & frustum) {
	
	if (enableCull_) {
		cullChunks(view_matrix, frustum);
	}

	for (auto chunk : renderList_) {
		chunk->draw(view_matrix * proj_matrix);
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
	return DirectX::XMMatrixTranslation(static_cast<float>(coord.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord.z_ * static_cast<int>(Chunk::DIM)));
}

void ChunkRenderer::cullChunks(const DirectX::XMMATRIX & view_matrix, BoundingFrustum & frustum) {

	renderList_.clear();
	XMVECTOR det = XMMatrixDeterminant(view_matrix);
	XMMATRIX inv_view = XMMatrixInverse(&det, view_matrix);

	XMVECTOR scale;
	XMVECTOR rotation;
	XMVECTOR tranlation;

	XMMatrixDecompose(&scale, &rotation, &tranlation, inv_view);
	BoundingFrustum world_frustum;
	frustum.Transform(world_frustum, XMVectorGetX(scale), rotation, tranlation);

	renderList_ = octree_.collides(world_frustum);

}

void ChunkRenderer::unloadChunks() {
	static int max_unload_number = 3;
	int count = 0;
	for (auto chunk : unloadList_) {
		if (count >= max_unload_number)
			break;
		delete chunk;
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
		renderList_.push_back(world_->getChunk(chunk_coord));
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
