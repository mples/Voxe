#include "ChunkRenderer.h"
#include <math.h>
#include <algorithm>
#include <limits>
#include <Windows.h>
//#include "../OcclusionCulling.h"

ChunkRenderer::ChunkRenderer() : device_(nullptr), deviceContext_(nullptr), enableCull_(false), octree_(BoundingBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.0f, 100.0f, 100.0f))) {
}


ChunkRenderer::~ChunkRenderer() {
}

bool ChunkRenderer::initialize(ID3D11Device * device, ID3D11DeviceContext * device_context) {
	device_ = device;
	deviceContext_ = device_context;

	texture_ = new Texture(device_, L"Data/Textures/grass.jpg", aiTextureType_DIFFUSE);
	HRESULT hr = CBVSObject_.initialize(device, device_context);
	COM_ERROR_IF_FAILED(hr, L"Falied to initialize constant buffer.");

	initializeRenderState();

	initializeShaders();

	chunkContext_.addActionMapping(Action::CULL, KeyboardEvent(KeyboardEvent::Type::PRESS, 'C'));

	INPUT.addFrontContext(&chunkContext_);
	InputCallback callback = [&](MappedInput& input) {
		auto cull = input.actions_.find(Action::CULL);
		if (cull != input.actions_.end()) {
			this->setEnableCulling(!this->getEnableCull());

			input.actions_.erase(cull);

		}
	};
	INPUT.addCallback(callback, InputCallbackPriority::HIGH);

	boundingVolume_.initialize(device, device_context);
	occlusionCulling_.initialize(device_, deviceContext_);
	return true;
}

bool ChunkRenderer::initializeShaders() {
	std::wstring shader_folder;
#pragma region DetermineShaderPath
	if (IsDebuggerPresent()) {
#ifdef _DEBUG
#ifdef _WIN64 //x64
		shader_folder = L"../x64/Debug/";
#else	//x86
		shader_folder = L"../Debug/";
#endif
#else
#ifdef _WIN64 //x64
		shader_folder = L"../x64/Release/";
#else	//x86
		shader_folder = L"../Release/";
#endif
#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout_desc[] = {
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEX_COORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (!vertexShader_.initialize(device_, shader_folder + L"shader_vertex.cso", layout_desc, _countof(layout_desc))) {
		return false;
	}

	if (!pixelShader_.initialize(device_, shader_folder + L"shader_pixel.cso")) {
		return false;
	}

	return true;
}

void ChunkRenderer::initializeRenderState() {
	D3D11_RASTERIZER_DESC rast_desc;
	ZeroMemory(&rast_desc, sizeof(D3D11_RASTERIZER_DESC));

	rast_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rast_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	HRESULT hr = device_->CreateRasterizerState(&rast_desc, rasterizerState_.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11RasterizerState.");

	D3D11_DEPTH_STENCIL_DESC ds_desc;
	ZeroMemory(&ds_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	ds_desc.DepthEnable = true;
	ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	ds_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

	device_->CreateDepthStencilState(&ds_desc, depthStencilState_.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11DepthStencilState.");

	D3D11_SAMPLER_DESC sampl_desc;
	ZeroMemory(&sampl_desc, sizeof(D3D11_SAMPLER_DESC));
	sampl_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampl_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampl_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampl_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampl_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampl_desc.MinLOD = 0;
	sampl_desc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device_->CreateSamplerState(&sampl_desc, samplerState_.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11SampleState.");

	D3D11_BLEND_DESC blend_desc;
	ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));

	D3D11_RENDER_TARGET_BLEND_DESC rtbdesc;
	ZeroMemory(&rtbdesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));

	rtbdesc.BlendEnable = true;
	rtbdesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	rtbdesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	rtbdesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbdesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbdesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	rtbdesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbdesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	blend_desc.RenderTarget[0] = rtbdesc;

	hr = device_->CreateBlendState(&blend_desc, blendState_.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, L"Error: Falied to create ID3D11BlendState.");

}

void ChunkRenderer::applyRendererState() {
	//deviceContext_->OMSetRenderTargets(1, renderTargetView_->GetAddressOf(), depthStencilView_->Get());
	
	deviceContext_->RSSetState(rasterizerState_.Get());
	deviceContext_->OMSetBlendState(blendState_.Get(), NULL, 0xffffffff);
	deviceContext_->OMSetDepthStencilState(depthStencilState_.Get(), 0);

	deviceContext_->PSSetSamplers(0, 1, samplerState_.GetAddressOf());

	deviceContext_->IASetInputLayout(vertexShader_.getInputLayout());
	deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext_->VSSetShader(vertexShader_.getShader(), NULL, 0);
	deviceContext_->PSSetShader(pixelShader_.getShader(), NULL, 0);


}

void ChunkRenderer::draw(const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix, BoundingFrustum & frustum) {
	applyRendererState();


	cullChunks(view_matrix, frustum);

	if (enableCull_) {

		std::vector<XMINT3> invisible_coord = occlusionCulling_.cull(view_matrix , proj_matrix, renderList_);
		applyRendererState();
		std::vector<Chunk*> render_list;
		/*for (XMINT3 & coord : invisible_coord) {
			activeChunks_.at(ChunkCoord(coord))->visible_ = false;
		}*/
		applyRendererState();
		for (auto chunk : renderList_) {
			if (chunk->visible_) {
				chunk->draw(view_matrix * proj_matrix);
				if (chunk->changed_ == true) {
					rebuildList_.push_back(chunk);
				}
				/*if (chunk->initialized_ == false) {
					initList_.push_back(chunk);
				}*/

			}
		}
	}
	else {
		for (auto chunk : renderList_) {
			chunk->draw(view_matrix * proj_matrix);
			if (chunk->changed_ == true) {
				rebuildList_.push_back(chunk);
			}
			/*if (chunk->initialized_ == false) {
				initList_.push_back(chunk);
			}*/
		}

	}
	JOB_SYSTEM.execute([&, this] {
		loadChunks();
	});


	JOB_SYSTEM.execute([&, this] {
		unloadChunks();
	});

	JOB_SYSTEM.execute([&, this] {
		rebuildChunks();
	});


	JOB_SYSTEM.wait();


}

void ChunkRenderer::setWorld(World * world) {
	if (world != nullptr){
		world_ = world;		
	}
}

void ChunkRenderer::setEnableCulling(bool state) {
	enableCull_ = state;
}

bool ChunkRenderer::getEnableCull() {
	return enableCull_;
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
			previousCenter_ = frustum_aabb.Center;
			firstDraw_ = false;
			minBounds_ = XMINT3(std::round((previousCenter_.x - frustum_aabb.Extents.x) / 16.0f),
				std::round((previousCenter_.y - frustum_aabb.Extents.y) / 16.0f),
				std::round((previousCenter_.z - frustum_aabb.Extents.z) / 16.0f));
			maxBounds_ = XMINT3(std::round((previousCenter_.x + frustum_aabb.Extents.x) / 16.0f),
				std::round((previousCenter_.y + frustum_aabb.Extents.y) / 16.0f),
				std::round((previousCenter_.z + frustum_aabb.Extents.z) / 16.0f));

			minBounds_.x -= 3;
			minBounds_.y -= 3;
			minBounds_.z -= 3;

			maxBounds_.x += 3;
			maxBounds_.y += 3;
			maxBounds_.z += 3;

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

		if (right_move > static_cast<float>(Chunk::DIM)) {
			OutputDebugStringA("Right entends.\n");
			while (right_move > static_cast<float>(Chunk::DIM)) {
				extendPosX();
				right_move -= static_cast<float>(Chunk::DIM);
			}
			
			previousCenter_.x = frustum_aabb.Center.x;
		}
		else if (right_move < -1.0f * static_cast<float>(Chunk::DIM)) {
			OutputDebugStringA("Left entends.\n");
			while (right_move < -1.0f * static_cast<float>(Chunk::DIM)) {
				extendNegX();
				right_move += static_cast<float>(Chunk::DIM);
			}

			previousCenter_.x = frustum_aabb.Center.x;
		}
		else if (up_move > static_cast<float>(Chunk::DIM)) {
			OutputDebugStringA("Up entends.\n");
			while (up_move > static_cast<float>(Chunk::DIM)) {
				extendPosY();
				up_move -= static_cast<float>(Chunk::DIM);
			}

			previousCenter_.y = frustum_aabb.Center.y;
		}
		else if (up_move < -1.0f * static_cast<float>(Chunk::DIM)) {
			OutputDebugStringA("Down entends.\n");

			while (up_move < -1.0f * static_cast<float>(Chunk::DIM)) {
				extendNegY();
				up_move += static_cast<float>(Chunk::DIM);
			}
			previousCenter_.y = frustum_aabb.Center.y;

		}
		else if (front_move > static_cast<float>(Chunk::DIM)) {
			OutputDebugStringA("entends.\n");
			while (front_move > static_cast<float>(Chunk::DIM)) {
				extendPosZ();
				front_move -= static_cast<float>(Chunk::DIM);
			}

			previousCenter_.z = frustum_aabb.Center.z;

		}
		else if (front_move < -1.0f * static_cast<float>(Chunk::DIM)) {
			OutputDebugStringA("Back entends.\n");

			while (front_move < -1.0f * static_cast<float>(Chunk::DIM)) {
				extendNegZ();
				front_move += static_cast<float>(Chunk::DIM);
			}
			previousCenter_.z = frustum_aabb.Center.z;
		}
	}

	renderList_.clear();
	renderList_ = octree_.collides(world_frustum);

}

void ChunkRenderer::unloadChunks() {
	static int max_unload_number = 5;
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
	static int max_load_number = 5;
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
	static int max_rebuild_number = 5;
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

void ChunkRenderer::extendPosX() {

	for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
		for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
			loadList_.push(ChunkCoord(maxBounds_.x + 1, y, z));
		}
	}

	for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
		for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
			unloadList_.push(ChunkCoord(minBounds_.x + 1, y, z));
		}
	}
	maxBounds_.x += 1;
	minBounds_.x += 1;
}

void ChunkRenderer::extendNegX() {

	for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
		for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
			loadList_.push(ChunkCoord(minBounds_.x - 1, y, z));
		}
	}

	for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
		for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
			unloadList_.push(ChunkCoord(maxBounds_.x, y, z));
		}
	}
	maxBounds_.x -= 1;
	minBounds_.x -= 1;
}

void ChunkRenderer::extendPosY() {

	for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
		for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
			loadList_.push(ChunkCoord(x, maxBounds_.y + 1, z));
		}
	}

	for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
		for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
			unloadList_.push(ChunkCoord(x, minBounds_.y, z));
		}
	}
	maxBounds_.y += 1;
	minBounds_.y += 1;
}

void ChunkRenderer::extendNegY() {

	for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
		for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
			loadList_.push(ChunkCoord(x, minBounds_.y - 1, z));
		}
	}

	for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
		for (int z = minBounds_.z; z <= maxBounds_.z; z++) {
			unloadList_.push(ChunkCoord(x, maxBounds_.y, z));
		}
	}
	maxBounds_.y -= 1;
	minBounds_.y -= 1;
}

void ChunkRenderer::extendPosZ() {

	for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
		for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
			loadList_.push(ChunkCoord(x, y, maxBounds_.z + 1));
		}
	}

	for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
		for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
			loadList_.push(ChunkCoord(x, y, minBounds_.z));
		}
	}
	maxBounds_.z += 1;
	minBounds_.z += 1;
}

void ChunkRenderer::extendNegZ() {

	for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
		for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
			loadList_.push(ChunkCoord(x, y, minBounds_.z - 1));
		}
	}

	for (int x = minBounds_.x; x <= maxBounds_.x; x++) {
		for (int y = minBounds_.y; y <= maxBounds_.y; y++) {
			loadList_.push(ChunkCoord(x, y, maxBounds_.z));
		}
	}
	maxBounds_.z -= 1;
	minBounds_.z -= 1;
}
