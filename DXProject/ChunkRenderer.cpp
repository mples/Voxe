#include "ChunkRenderer.h"
#include <math.h>
#include <algorithm>
#include <limits>

ChunkRenderer::ChunkRenderer() : device_(nullptr), deviceContext_(nullptr), enableCull_(true), octree_(BoundingBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f))) {
	//previousViewBox_ = BoundingBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1000.0f, 1000.0f, 1000.0f));
}


ChunkRenderer::~ChunkRenderer() {
}

bool ChunkRenderer::initialize(ID3D11Device * device, ID3D11DeviceContext * device_context) {
	device_ = device;
	deviceContext_ = device_context;

	texture_ = new Texture(device_, L"Data/Textures/grass.jpg", aiTextureType_DIFFUSE);
	HRESULT hr = CBVSObject_.initialize(device, device_context);
	COM_ERROR_IF_FAILED(hr, L"Falied to initialize constant buffer.");

	////debug
	//for (int i = -1; i < 1; i++) {
	//	for (int j = -1; j < 1; j++) {
	//		for (int k = -1; k < 1; k++) {
	//			loadList_.push_back(ChunkCoord(i, j, k));
	//			//Chunk * chunk = world_->getChunk(i, j, k);
	//			//chunk->initialize(device_, deviceContext_, CBVSObject_, texture_);
	//			//activeChunks_.push_back(chunk);
	//			//octree_.insert(chunk);
	//		}
	//	}
	//}

	chunkContext_.addActionMapping(Action::CULL, KeyboardEvent(KeyboardEvent::Type::PRESS, 'C'));

	INPUT.addFrontContext(&chunkContext_);
	InputCallback callback = [=](MappedInput& input) {
		auto cull = input.actions_.find(Action::CULL);
		if (cull != input.actions_.end()) {
			

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
		if (chunk->changed_ == true) {
			rebuildList_.push_back(chunk);
		}
		if (chunk->initialized_ == false) {
			initList_.push_back(chunk);
		}
	}

	//activeChunks_ = renderList_;
	//renderList_.clear();
	loadChunks();
	rebuildChunks();
	initializeChunks();
	//unloadChunks();
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
	XMVECTOR det = XMMatrixDeterminant(view_matrix);
	XMMATRIX inv_view = XMMatrixInverse(&det, view_matrix);

	XMVECTOR scale;
	XMVECTOR rotation;
	XMVECTOR tranlation;

	XMMatrixDecompose(&scale, &rotation, &tranlation, inv_view);
	BoundingFrustum world_frustum;
	frustum.Transform(world_frustum, XMVectorGetX(scale), rotation, tranlation);

	/*if (octree_ == nullptr) {
		XMFLOAT3 corners[8];
		world_frustum.GetCorners(corners);
		previousViewBox_.CreateFromPoints(previousViewBox_, 8, corners, sizeof(XMFLOAT3));

		octree_ = new Octree<Chunk>(previousViewBox_);

		XMINT3 min_bounds(std::round((previousViewBox_.Center.x - previousViewBox_.Extents.x) / 16.0f),
			std::round((previousViewBox_.Center.y - previousViewBox_.Extents.y) / 16.0f),
			std::round((previousViewBox_.Center.z - previousViewBox_.Extents.z) / 16.0f));
		XMINT3 max_bounds(std::round((previousViewBox_.Center.x + previousViewBox_.Extents.x) / 16.0f),
			std::round((previousViewBox_.Center.y + previousViewBox_.Extents.y) / 16.0f),
			std::round((previousViewBox_.Center.z + previousViewBox_.Extents.z) / 16.0f));

		for (int x = min_bounds.x; x <= max_bounds.x; x++) {
			for (int y = min_bounds.y; y <= max_bounds.y; y++) {
				for (int z = min_bounds.z; z <= max_bounds.z; z++) {
					loadList_.push_back(ChunkCoord(x, y, z));
				}
			}
		}
	}
	else*/
	{

		XMFLOAT3 corners[8];
		world_frustum.GetCorners(corners);
		BoundingBox frustum_aabb;
		frustum_aabb.CreateFromPoints(frustum_aabb, 8, corners, sizeof(XMFLOAT3));

		if (firstDraw_) {
			previousExtend_ = frustum_aabb.Extents;
			previousCenter_ = frustum_aabb.Center;
			firstDraw_ = false;
			XMINT3 min_bounds(std::round((previousCenter_.x - previousExtend_.x) / 16.0f),
				std::round((previousCenter_.y - previousExtend_.y) / 16.0f),
				std::round((previousCenter_.z - previousExtend_.z) / 16.0f));
			XMINT3 max_bounds(std::round((previousCenter_.x + previousExtend_.x) / 16.0f),
				std::round((previousCenter_.y + previousExtend_.y) / 16.0f),
				std::round((previousCenter_.z + previousExtend_.z) / 16.0f));

			for (int x = min_bounds.x; x < max_bounds.x; x++) {
				for (int y = min_bounds.y; y < max_bounds.y; y++) {
					for (int z = min_bounds.z; z < max_bounds.z; z++) {
						loadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}

		}

		XMVECTOR origin = XMLoadFloat3(&frustum_aabb.Center);
		XMVECTOR prev_center = XMLoadFloat3(&previousCenter_);
		XMVECTOR moved_distance = XMVectorSubtract(origin, prev_center);

		XMVECTOR distance = XMVector3Length(moved_distance);

		static XMFLOAT3 up(0.0f, 1.0f, 0.0f);
		static XMFLOAT3 right(1.0f, 0.0f, 0.0f);
		static XMFLOAT3 front(0.0f, 0.0f, 1.0f);

		XMVECTOR up_vec = XMLoadFloat3(&up);
		XMVECTOR right_vec = XMLoadFloat3(&right);
		XMVECTOR front_vec = XMLoadFloat3(&front);

		XMVECTOR perp_proj;
		XMVECTOR up_proj;
		XMVector3ComponentsFromNormal(&up_proj, &perp_proj, moved_distance, up_vec);
		XMVECTOR right_proj;
		XMVector3ComponentsFromNormal(&right_proj, &perp_proj, moved_distance, right_vec);
		XMVECTOR front_proj;
		XMVector3ComponentsFromNormal(&front_proj, &perp_proj, moved_distance, front_vec);
		
		float up_move = XMVectorGetX(XMVector3Length(up_proj));
		float right_move = XMVectorGetX(XMVector3Length(right_proj));
		float front_move = XMVectorGetX(XMVector3Length(front_proj));

		XMINT3 min_bounds(std::round((previousCenter_.x - previousExtend_.x) / 16.0f),
			std::round((previousCenter_.y - previousExtend_.y) / 16.0f),
			std::round((previousCenter_.z - previousExtend_.z) / 16.0f));
		XMINT3 max_bounds(std::round((previousCenter_.x + previousExtend_.x) / 16.0f),
			std::round((previousCenter_.y + previousExtend_.y) / 16.0f),
			std::round((previousCenter_.z + previousExtend_.z) / 16.0f));

	/*	XMINT3 min_bounds(std::round((octree_.getBoundingBox().Center.x - octree_.getBoundingBox().Extents.x) / 16.0f),
			std::round((octree_.getBoundingBox().Center.y - octree_.getBoundingBox().Extents.y) / 16.0f),
			std::round((octree_.getBoundingBox().Center.z - octree_.getBoundingBox().Extents.z) / 16.0f));
		XMINT3 max_bounds(std::round((octree_.getBoundingBox().Center.x + octree_.getBoundingBox().Extents.x) / 16.0f),
			std::round((octree_.getBoundingBox().Center.y + octree_.getBoundingBox().Extents.y) / 16.0f),
			std::round((octree_.getBoundingBox().Center.z + octree_.getBoundingBox().Extents.z) / 16.0f));*/

		if (right_move > 0.0f && std::round(right_move) > 1) {

			for (int x = max_bounds.x; x < max_bounds.x + 1; x++) {
				for (int y = min_bounds.y; y < max_bounds.y; y++) {
					for (int z = min_bounds.z; z < max_bounds.z; z++) {
						loadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = min_bounds.x - 1 ; x < min_bounds.x ; x++) {
				for (int y = min_bounds.y; y < max_bounds.y; y++) {
					for (int z = min_bounds.z; z < max_bounds.z; z++) {
						unloadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}
			previousCenter_.x = frustum_aabb.Center.x;
			previousExtend_.x = frustum_aabb.Extents.x;
		}
		else if (right_move < 0.0f && std::round(right_move) < -1) {
			for (int x = max_bounds.x; x < max_bounds.x + 1; x++) {
				for (int y = min_bounds.y; y < max_bounds.y; y++) {
					for (int z = min_bounds.z; z < max_bounds.z; z++) {
						unloadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = min_bounds.x - 1; x < min_bounds.x ; x++) {
				for (int y = min_bounds.y; y < max_bounds.y; y++) {
					for (int z = min_bounds.z; z < max_bounds.z; z++) {
						loadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}
			previousCenter_.x = frustum_aabb.Center.x;
			previousExtend_.x = frustum_aabb.Extents.x;
		}

		if (up_move > 0.0f && std::round(up_move) > 1) {

			for (int x = min_bounds.x; x < max_bounds.x; x++) {
				for (int y = max_bounds.y; y < max_bounds.y + 1; y++) {
					for (int z = min_bounds.z; z < max_bounds.z; z++) {
						loadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = min_bounds.x; x < max_bounds.x ; x++) {
				for (int y = min_bounds.y - 1; y < min_bounds.y; y++) {
					for (int z = min_bounds.z; z < max_bounds.z; z++) {
						unloadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}
			previousCenter_.y = frustum_aabb.Center.y;
			previousExtend_.y = frustum_aabb.Extents.y;
		}
		else if (up_move < 0.0f && std::round(up_move) < -1) {
			for (int x = min_bounds.x; x < max_bounds.x; x++) {
				for (int y = max_bounds.y; y < max_bounds.y + 1; y++) {
					for (int z = min_bounds.z; z < max_bounds.z; z++) {
						unloadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = min_bounds.x; x < max_bounds.x; x++) {
				for (int y = min_bounds.y - 1; y < min_bounds.y; y++) {
					for (int z = min_bounds.z; z < max_bounds.z; z++) {
						loadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}
			previousCenter_.y = frustum_aabb.Center.y;
			previousExtend_.y = frustum_aabb.Extents.y;
		}

		if (front_move > 0.0f && std::round(front_move) > 1 ) {

			for (int x = min_bounds.x; x < max_bounds.x; x++) {
				for (int y = min_bounds.y; y < max_bounds.y; y++) {
					for (int z = max_bounds.z; z < max_bounds.z + 1; z++) {
						loadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = min_bounds.x; x < max_bounds.x; x++) {
				for (int y = min_bounds.y; y < max_bounds.y; y++) {
					for (int z = min_bounds.z - 1; z < min_bounds.z; z++) {
						unloadList_.push_back(ChunkCoord(x, y, z));
					}
				}
			}
			previousCenter_.z = frustum_aabb.Center.z;
			previousExtend_.z = frustum_aabb.Extents.z;
		}
		else if (front_move < 0.0f && std::round(front_move) < -1) {
			int z_max = max_bounds.z + 1;
			for (int x = min_bounds.x; x < max_bounds.x; x++) {
				for (int y = min_bounds.y; y < max_bounds.y; y++) {
					unloadList_.push_back(ChunkCoord(x, y, z_max));
				}
			}
			int z_min = min_bounds.z - 1;
			for (int x = min_bounds.x; x < max_bounds.x; x++) {
				for (int y = min_bounds.y; y < max_bounds.y; y++) {
						loadList_.push_back(ChunkCoord(x, y, z_min));
				}
			}
			previousCenter_.z = frustum_aabb.Center.z;
			previousExtend_.z = frustum_aabb.Extents.z;
		}
	}


	/*XMVECTOR det = XMMatrixDeterminant(view_matrix);
	XMMATRIX inv_view = XMMatrixInverse(&det, view_matrix);

	XMVECTOR scale;
	XMVECTOR rotation;
	XMVECTOR tranlation;

	XMMatrixDecompose(&scale, &rotation, &tranlation, inv_view);
	BoundingFrustum world_frustum;
	frustum.Transform(world_frustum, XMVectorGetX(scale), rotation, tranlation);*/

	renderList_.clear();
	renderList_ = octree_.collides(world_frustum);

}

void ChunkRenderer::unloadChunks() {
	static int max_unload_number = 20;
	int count = 0;
	while (count < max_unload_number && !unloadList_.empty()) {
		ChunkCoord coord = unloadList_.back();
		Chunk * chunk = world_->getChunk(coord);
		auto found = std::find(activeChunks_.begin(), activeChunks_.end(), chunk);
		if (found != activeChunks_.end()) {
			activeChunks_.erase(found);
		}
		octree_.remove(chunk);
		//world_->remove(chunk);
		//delete chunk;
		unloadList_.pop_back();
		count++;
	}


}

void ChunkRenderer::loadChunks() {
	static int max_load_number = 20;
	int count = 0;
	//while (count < max_load_number && !loadList_.empty()) {
	//	Chunk* chunk = world_->getChunk(loadList_.back());
	//	//octree_->insert(chunk);
	//	renderList_.push_back(chunk);
	//	loadList_.pop_back();
	//	count++;
	//}
	
	for (auto chunk_coord : loadList_) {
		Chunk * chunk = world_->getChunk(chunk_coord);
		octree_.insert(chunk);
		activeChunks_.push_back(chunk);
		++count;
		//if (count >= max_load_number) {
		//	break;
		//}
	}
	loadList_.clear();
}

void ChunkRenderer::rebuildChunks() {
	static int max_rebuild_number = 3;
	int count = 0;
	for (auto chunk : rebuildList_) {
		if (count >= max_rebuild_number) {
			break;
		}
		chunk->update(device_, deviceContext_, CBVSObject_, texture_);
		++count;
	}
	rebuildList_.clear();
}

void ChunkRenderer::initializeChunks() {
	static int max_init_number = 20;
	int count = 0;
	//while (count < max_load_number && !loadList_.empty()) {
	//	Chunk* chunk = world_->getChunk(loadList_.back());
	//	//octree_->insert(chunk);
	//	renderList_.push_back(chunk);
	//	loadList_.pop_back();
	//	count++;
	//}

	for (auto chunk : initList_) {
		//Chunk * chunk = world_->getChunk(chunk_coord);
		chunk->initialize(device_, deviceContext_, CBVSObject_, texture_);
		//octree_.insert(chunk);
		//activeChunks_.push_back(chunk);
		++count;
		if (count >= max_init_number) {
			break;
		}
	}
	initList_.clear();
}
