#include "ChunkRenderer.h"
#include <math.h>
#include <algorithm>
#include <limits>
#include <Windows.h>

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

	loadChunks();
	rebuildChunks();
	//initializeChunks();
	unloadChunks();
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

	{

		XMFLOAT3 corners[8];
		world_frustum.GetCorners(corners);
		BoundingBox frustum_aabb;
		frustum_aabb.CreateFromPoints(frustum_aabb, 8, corners, sizeof(XMFLOAT3));
		if (firstDraw_) {
			XMFLOAT3 extend = frustum_aabb.Extents;
			extend.x += 3.0f * Chunk::DIM;
			extend.y += 3.0f * Chunk::DIM;
			extend.z += 3.0f * Chunk::DIM;

			previousCenter_ = frustum_aabb.Center;
			firstDraw_ = false;
			minBounds_ = XMINT3(std::round((previousCenter_.x - extend.x) / 16.0f),
				std::round((previousCenter_.y - extend.y) / 16.0f),
				std::round((previousCenter_.z - extend.z) / 16.0f));
			maxBounds_ = XMINT3(std::round((previousCenter_.x + extend.x) / 16.0f),
				std::round((previousCenter_.y + extend.y) / 16.0f),
				std::round((previousCenter_.z + extend.z) / 16.0f));

			for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
				for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
					for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
						loadList_.push(ChunkCoord(x, y, z));
					}
				}
			}

		}

		XMVECTOR origin = XMLoadFloat3(&frustum_aabb.Center);
		XMVECTOR prev_center = XMLoadFloat3(&previousCenter_);
		XMVECTOR moved_distance = XMVectorSubtract(origin, prev_center);

		static XMFLOAT3 up(0.0f, 1.0f, 0.0f);
		static XMFLOAT3 right(1.0f, 0.0f, 0.0f);
		static XMFLOAT3 front(0.0f, 0.0f, 1.0f);

		static XMVECTOR up_vec = XMLoadFloat3(&up);
		static XMVECTOR right_vec = XMLoadFloat3(&right);
		static XMVECTOR front_vec = XMLoadFloat3(&front);

		XMVECTOR perp_proj;
		XMVECTOR up_proj;
		XMVector3ComponentsFromNormal(&up_proj, &perp_proj, moved_distance, up_vec);
		XMVECTOR right_proj;
		XMVector3ComponentsFromNormal(&right_proj, &perp_proj, moved_distance, right_vec);
		XMVECTOR front_proj;
		XMVector3ComponentsFromNormal(&front_proj, &perp_proj, moved_distance, front_vec);
		
		float right_move = XMVectorGetX(right_proj);
		float up_move = XMVectorGetY(up_proj);
		float front_move = XMVectorGetZ(front_proj);


		assert(right_move > -32.0f && right_move < 32.0f);
		assert(front_move > -32.0f && front_move < 32.0f);
		assert(up_move > -32.0f && up_move < 32.0f);

		if (right_move > 1.0f * Chunk::DIM) {
			OutputDebugStringA("Right Extend.\n");
			int diff = std::round(right_move / Chunk::DIM);
			for (int x = maxBounds_.x + 1; x <= maxBounds_.x + diff; x++) {
				for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
					for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
						loadList_.push(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = minBounds_.x; x < minBounds_.x + diff ; x++) {
				for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
					for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
						unloadList_.push(ChunkCoord(x, y, z));
					}
				}
			}
			
			previousCenter_.x = frustum_aabb.Center.x;
			maxBounds_.x += diff;
			minBounds_.x += diff;
		}
		else if (right_move < -1.0f * Chunk::DIM) {
			OutputDebugStringA("Left Extend.\n");
			int diff = std::round(std::abs(right_move) / Chunk::DIM);
			for (int x = minBounds_.x - diff; x < minBounds_.x ; x++) {
				for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
					for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
						loadList_.push(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = maxBounds_.x - diff + 1; x <= maxBounds_.x; x++) {
				for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
					for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
						unloadList_.push(ChunkCoord(x, y, z));
					}
				}
			}


			previousCenter_.x = frustum_aabb.Center.x;
			maxBounds_.x -= diff;
			minBounds_.x -= diff;
		}
		else if (up_move > 1.0f * Chunk::DIM) {
			OutputDebugStringA("Up Extend.\n");
			int diff = std::round(std::abs(up_move) / Chunk::DIM);
			for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
				for (int y = maxBounds_.y + 1; y <= maxBounds_.y + diff; y++) {
					for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
						loadList_.push(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = minBounds_.x; x <= maxBounds_.x ; x++) {
				for (int y = minBounds_.y; y < minBounds_.y + diff; y++) {
					for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
						unloadList_.push(ChunkCoord(x, y, z));
					}
				}
			}
			previousCenter_.y = frustum_aabb.Center.y;
			maxBounds_.y += diff;
			minBounds_.y += diff;
		}
		else if (up_move < -1.0f * Chunk::DIM) {
			OutputDebugStringA("Down Extend.\n");
			int diff = std::round(std::abs(right_move) / Chunk::DIM);
			for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
				for (int y = minBounds_.y - diff; y < minBounds_.y; y++) {
					for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
						loadList_.push(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
				for (int y = maxBounds_.y - diff + 1; y <= maxBounds_.y ; y++) {
					for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
						unloadList_.push(ChunkCoord(x, y, z));
					}
				}
			}
			previousCenter_.y = frustum_aabb.Center.y;
			maxBounds_.y -= diff;
			minBounds_.y -= diff;
		}
		else if (front_move > 1.0f * Chunk::DIM) {
			OutputDebugStringA("Front Extend.\n");
			int diff = std::round(right_move / Chunk::DIM);
			for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
				for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
					for (int z = maxBounds_.z + 1; z <= maxBounds_.z + diff; z++) {
						loadList_.push(ChunkCoord(x, y, z));
					}
				}
			}
			for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
				for (int y = minBounds_.y; y < maxBounds_.y + diff; y++) {
					for (int z = minBounds_.z; z <= minBounds_.z; z++) {
						unloadList_.push(ChunkCoord(x, y, z));
					}
				}
			}
			previousCenter_.z = frustum_aabb.Center.z;
			maxBounds_.z += diff;
			minBounds_.z += diff;
		}
		else if (front_move < -1.0f * Chunk::DIM) {
			OutputDebugStringA("Back Extend.\n");
			int diff = std::round(std::abs(right_move) / Chunk::DIM);
			for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
				for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
					for (int z = minBounds_.z - diff; z < minBounds_.z ; z++) {
						loadList_.push(ChunkCoord(x, y, z));
					}
				}
			}

			for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
				for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
					for (int z = maxBounds_.z + 1 - diff; z <= maxBounds_.z; z++) {
						unloadList_.push(ChunkCoord(x, y, z));
					}
				}
			}
			previousCenter_.z = frustum_aabb.Center.z;
			maxBounds_.z -= diff;
			minBounds_.z -= diff;
		}
	}

	renderList_.clear();
	renderList_ = octree_.collides(world_frustum);

}

void ChunkRenderer::unloadChunks() {
	static int max_unload_number = 20;
	int count = 0;
	while (count < max_unload_number && !unloadList_.empty()) {
		ChunkCoord coord = unloadList_.front();

		auto found = activeChunks_.find(coord);
		if (found != activeChunks_.end()) {
			octree_.remove(found->second);
			chunkPool_.release(found->second);
			activeChunks_.erase(found);
		}

		unloadList_.pop();
		count++;
	}


}

void ChunkRenderer::loadChunks() {
	static int max_load_number = 10;
	int count = 0;

	while (count < max_load_number && !loadList_.empty()) {
		ChunkCoord chunk_coord = loadList_.front();

		BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM] = {};
		world_->generateChunk(blocks, chunk_coord);
		Chunk* chunk = chunkPool_.create();
		if (chunk == nullptr) {
			assert(0);
		}
		chunk->initialize(chunk_coord, blocks);
		setAdjacentChunks(chunk);

		chunk->initializeMesh(device_, deviceContext_, CBVSObject_, texture_);
		octree_.insert(chunk);
		activeChunks_.insert(std::make_pair(chunk->getCoord(), chunk));

		loadList_.pop();
		count++;
	}
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

	for (auto chunk : initList_) {
		chunk->initializeMesh(device_, deviceContext_, CBVSObject_, texture_);
		//octree_.insert(chunk);
		++count;
		if (count >= max_init_number) {
			break;
		}
	}
	initList_.clear();
}

void ChunkRenderer::setAdjacentChunks(Chunk * chunk) {
	ChunkCoord left_coord = ChunkCoord(chunk->getCoord().x_ - 1, chunk->getCoord().y_, chunk->getCoord().z_);
	auto left_chunk = activeChunks_.find(left_coord);
	if (left_chunk != activeChunks_.end()) {
		chunk->neighbours_.left_ = left_chunk->second;
		left_chunk->second->neighbours_.right_ = chunk;
		left_chunk->second->changed_ = true;
	}

	ChunkCoord right_coord = ChunkCoord(chunk->getCoord().x_ + 1, chunk->getCoord().y_, chunk->getCoord().z_);
	auto right_chunk = activeChunks_.find(right_coord);
	if (right_chunk != activeChunks_.end()) {
		chunk->neighbours_.right_ = right_chunk->second;
		right_chunk->second->neighbours_.left_ = chunk;
		right_chunk->second->changed_ = true;
	}

	ChunkCoord up_coord = ChunkCoord(chunk->getCoord().x_, chunk->getCoord().y_ + 1, chunk->getCoord().z_);
	auto up_chunk = activeChunks_.find(up_coord);
	if (up_chunk != activeChunks_.end()) {
		chunk->neighbours_.up_ = up_chunk->second;
		up_chunk->second->neighbours_.down_ = chunk;
		up_chunk->second->changed_ = true;
	}

	ChunkCoord down_coord = ChunkCoord(chunk->getCoord().x_, chunk->getCoord().y_ - 1, chunk->getCoord().z_);
	auto down_chunk = activeChunks_.find(down_coord);
	if (down_chunk != activeChunks_.end()) {
		chunk->neighbours_.down_ = down_chunk->second;
		down_chunk->second->neighbours_.up_ = chunk;
		down_chunk->second->changed_ = true;
	}

	ChunkCoord front_coord = ChunkCoord(chunk->getCoord().x_, chunk->getCoord().y_, chunk->getCoord().z_ + 1);
	auto front_chunk = activeChunks_.find(front_coord);
	if (front_chunk != activeChunks_.end()) {
		chunk->neighbours_.front_ = front_chunk->second;
		front_chunk->second->neighbours_.back_ = chunk;
		front_chunk->second->changed_ = true;
	}

	ChunkCoord back_coord = ChunkCoord(chunk->getCoord().x_, chunk->getCoord().y_, chunk->getCoord().z_ - 1);
	auto back_chunk = activeChunks_.find(back_coord);
	if (back_chunk != activeChunks_.end()) {
		chunk->neighbours_.back_ = back_chunk->second;
		back_chunk->second->neighbours_.front_ = chunk;
		back_chunk->second->changed_ = true;
	}
}
