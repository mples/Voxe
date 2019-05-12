#include "OcclusionCulling.h"



OcclusionCulling::OcclusionCulling() : spatialMap_(50){
}


OcclusionCulling::~OcclusionCulling() {
}

bool OcclusionCulling::initialize(ID3D11Device * device, ID3D11DeviceContext * device_context) {
	device_ = device;
	deviceContext_ = device_context;

	if (!initializeShaders(device)) {
		return false;
	}

	UINT msaa_quality;
	device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_quality);
	assert(msaa_quality > 0);

	D3D11_TEXTURE2D_DESC dsbd;
	dsbd.Width = depthBufferWidth_;
	dsbd.Height = depthBufferHeight_;
	dsbd.MipLevels = 1;
	dsbd.ArraySize = 1;
	dsbd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (true) {
		dsbd.SampleDesc.Count = 4;
		dsbd.SampleDesc.Quality = msaa_quality - 1;
	}
	else {
		dsbd.SampleDesc.Count = 1;
		dsbd.SampleDesc.Quality = 0;
	}
	dsbd.Usage = D3D11_USAGE_DEFAULT;
	dsbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsbd.CPUAccessFlags = 0;
	dsbd.MiscFlags = 0;

	HRESULT hr = device_->CreateTexture2D(&dsbd, nullptr, depthStencilBuffer_.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11Texture2D for depth stancil buffer.");

	hr = device_->CreateDepthStencilView(depthStencilBuffer_.Get(), nullptr, depthStencilView_.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11DepthStencilView.");

	objectBuffer_.initialize(device_, deviceContext_);
	
	return true;
}

void OcclusionCulling::initializeRenderState() {
	D3D11_RASTERIZER_DESC rast_desc;
	ZeroMemory(&rast_desc, sizeof(D3D11_RASTERIZER_DESC));

	rast_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rast_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

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

bool OcclusionCulling::initializeShaders(ID3D11Device * device) {
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
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (!vertexShader_.initialize(device, shader_folder + L"occlusion_culling_vs.cso", layout_desc, _countof(layout_desc))) {
		return false;
	}

	return true;
}

std::vector<XMINT3> OcclusionCulling::cull(const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix, std::vector<Chunk*>& chunks) {
	//spatialMap_.clear();

	std::vector<Chunk*> sorted_chunks;
	for (Chunk* chunk : chunks) {
		if (!chunk->isEmpty_) {
			//spatialMap_.insert(&chunk->getBV());
			sorted_chunks.push_back(chunk);
		}
	}



	XMFLOAT3 camera_front = getCameraFrontDir(view_matrix);
	XMFLOAT3 camera_pos = getCameraPos(view_matrix);
	


	XMFLOAT3 camera_front_abs(abs(camera_front.x), abs(camera_front.y), abs(camera_front.z));
	if (camera_front_abs.x >= camera_front_abs.y && camera_front_abs.x >= camera_front_abs.z) {
		int move = 0;
		if (camera_front.x > 0) {
			std::sort(sorted_chunks.begin(), sorted_chunks.end(), [](Chunk* a, Chunk * b) {
				return a->getCoord().x_ > b->getCoord().x_;
			});
		}
		else {
			std::sort(sorted_chunks.begin(), sorted_chunks.end(), [](Chunk* a, Chunk * b) {
				return a->getCoord().x_ < b->getCoord().x_;
			});
		}
		
	}
	else if (camera_front_abs.y >= camera_front_abs.x && camera_front_abs.y >= camera_front_abs.z) {
		int move = 0;
		if (camera_front.y > 0) {
			std::sort(sorted_chunks.begin(), sorted_chunks.end(), [](Chunk* a, Chunk * b) {
				return a->getCoord().y_ > b->getCoord().y_;
			});
		}
		else {
			std::sort(sorted_chunks.begin(), sorted_chunks.end(), [](Chunk* a, Chunk * b) {
				return a->getCoord().y_ < b->getCoord().y_;
			});
		}
		
	}
	else if (camera_front_abs.z >= camera_front_abs.x && camera_front_abs.z >= camera_front_abs.y) {
		int move = 0;
		if (camera_front.z > 0) {
			std::sort(sorted_chunks.begin(), sorted_chunks.end(), [](Chunk* a, Chunk * b) {
				return a->getCoord().z_ > b->getCoord().z_;
			});
		}
		else {
			std::sort(sorted_chunks.begin(), sorted_chunks.end(), [](Chunk* a, Chunk * b) {
				return a->getCoord().z_ < b->getCoord().z_;
			});
		}
		
	}
	ID3D11RenderTargetView * rtv[1];
	ID3D11DepthStencilView * dsv  = nullptr;
	deviceContext_->OMGetRenderTargets(1, rtv, &dsv);

	deviceContext_->OMSetRenderTargets(0, NULL, depthStencilView_.Get());
	deviceContext_->RSSetState(rasterizerState_.Get());

	deviceContext_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//deviceContext_->IASetInputLayout(vertexShader_.getInputLayout());
	deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//deviceContext_->VSSetShader(vertexShader_.getShader(), NULL, 0);
	//deviceContext_->PSSetShader(NULL, NULL, 0);

	for (Chunk * chunk : sorted_chunks) {
		D3D11_QUERY_DESC query_desc;
		query_desc.Query = D3D11_QUERY::D3D11_QUERY_OCCLUSION;
		query_desc.MiscFlags = 0u;
		ID3D11Query * query;
		device_->CreateQuery(&query_desc, &query);

		deviceContext_->Begin(query);

		//deviceContext_->VSSetConstantBuffers(0, 1, objectBuffer_.getAddressOf());


		//objectBuffer_.data_.mvpMatrix_ = XMMatrixIdentity() * view_matrix * proj_matrix;
		//objectBuffer_.data_.modelMatrix_ = XMMatrixIdentity();

		objectBuffer_.applyChanges();

		chunk->draw(view_matrix * proj_matrix);

		deviceContext_->End(query);

		queriesBuffer_.push_back(OcclusionQuery(chunk, query));
	}

	//querySpatialMap(camera_front, camera_pos, view_matrix, proj_matrix);

	deviceContext_->OMSetRenderTargets(1, rtv, dsv);


	std::vector<XMINT3> invisible_coord;
	std::vector<OcclusionQuery> avaliable_queries;

	UINT64 query_data;
	while (!queriesBuffer_.empty() && queriesBuffer_.front().getDataIfAvailable(deviceContext_, &query_data)) {
		if (query_data == 0) {
			//invisible_coord.push_back(query.getBoundingVolume()->getCoord());
			queriesBuffer_.front().getChunk()->visible_ = false;
		}
		else {
			queriesBuffer_.front().getChunk()->visible_ = true;
		}
		queriesBuffer_.pop_front();
	}
	//
	//for (auto query : queriesBuffer_) {
	//	if (query.getDataIfAvailable(deviceContext_, &query_data)) {
	//		avaliable_queries.push_back(query);
	//	}
	//	/*UINT64 query_data = it->getData(deviceContext_);
	//	if (query_data > 0) {
	//		visible_coord.push_back(it->getBoundingVolume()->getCoord());
	//	}*/
	//
	//}

	//for (auto query : avaliable_queries) {
	//	UINT64 query_data = query.getData(deviceContext_);
	//	if (query_data == 0) {
	//		//invisible_coord.push_back(query.getBoundingVolume()->getCoord());
	//		query.getChunk()->visible_ = false;
	//	}
	//	else {
	//		query.getChunk()->visible_ = true;
	//	}
	//	queriesBuffer_.remove(query);
	//}
	//queriesBuffer_.clear();

	return invisible_coord;
	
}

XMFLOAT3 OcclusionCulling::getCameraFrontDir(const DirectX::XMMATRIX & view_matrix) {
	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, view_matrix);
	return XMFLOAT3(-view._31, -view._31, -view._33);
}

XMFLOAT3 OcclusionCulling::getCameraPos(const DirectX::XMMATRIX & view_matrix) {
	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, view_matrix);
	return XMFLOAT3(-view._41, -view._42, -view._43);
}

void OcclusionCulling::query(BoundingVolume * bv, const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix) {
	D3D11_QUERY_DESC query_desc;
	query_desc.Query = D3D11_QUERY::D3D11_QUERY_OCCLUSION;
	query_desc.MiscFlags = 0u;
	ID3D11Query * query;
	device_->CreateQuery(&query_desc, &query);

	deviceContext_->Begin(query);

	deviceContext_->VSSetConstantBuffers(0, 1, objectBuffer_.getAddressOf());


	objectBuffer_.data_.mvpMatrix_ = XMMatrixIdentity() * view_matrix * proj_matrix;
	objectBuffer_.data_.modelMatrix_ = XMMatrixIdentity();

	objectBuffer_.applyChanges();

	bv->draw();

	deviceContext_->End(query);

	//queriesBuffer_.push_back(OcclusionQuery(bv, query));
}

void OcclusionCulling::querySpatialMap(XMFLOAT3 camera_front, XMFLOAT3 camera_pos, const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix) {
	XMFLOAT3 camera_front_abs(abs(camera_front.x), abs(camera_front.y), abs(camera_front.z));
	if (camera_front_abs.x >= camera_front_abs.y && camera_front_abs.x >= camera_front_abs.z) {
		int move = 0;
		if (camera_front.x > 0) {
			move = 1;
		}
		else {
			move = -1;
		}
		int x = camera_pos.x;
		int y = camera_pos.y;
		int z = camera_pos.z;

		std::vector<BoundingVolume*>* bucket;
		Point p = spatialMap_.hash(x, y, z);
		while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_, p.z_))) {
			for (BoundingVolume* bv : *bucket) {
				query(bv, view_matrix, proj_matrix);
			}
			int i = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_ + i, p.z_))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				i++;
			}
			i = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_ - i, p.z_))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				i++;
			}
			int j = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_, p.z_ + j))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				j++;
			}
			j = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_, p.z_ - j))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				j++;
			}
			p.x_ += move;
		}
	}
	else if (camera_front_abs.y >= camera_front_abs.x && camera_front_abs.y >= camera_front_abs.z) {
		int move = 0;
		if (camera_front.y > 0) {
			move = 1;
		}
		else {
			move = -1;
		}
		int x = camera_pos.x;
		int y = camera_pos.y;
		int z = camera_pos.z;

		std::vector<BoundingVolume*>* bucket;
		Point p = spatialMap_.hash(x, y, z);
		while (nullptr != (bucket = spatialMap_.getBucket(p.x_ , p.y_, p.z_))) {
			for (BoundingVolume* bv : *bucket) {
				query(bv, view_matrix, proj_matrix);
			}
			Point p = spatialMap_.hash(x, y, z);
			int i = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_ + i, p.y_, p.z_))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				i++;
			}
			i = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_ - i, p.y_, p.z_))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				i++;
			}
			int j = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_, p.z_ + j))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				j++;
			}
			j = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_, p.z_ - j))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				j++;
			}
			p.y_ += move;
		}
	}
	else if (camera_front_abs.z >= camera_front_abs.x && camera_front_abs.z >= camera_front_abs.y) {
		int move = 0;
		if (camera_front.z > 0) {
			move = 1;
		}
		else {
			move = -1;
		}
		int x = camera_pos.x;
		int y = camera_pos.y;
		int z = camera_pos.z;

		std::vector<BoundingVolume*>* bucket;
		Point p = spatialMap_.hash(x, y, z);
		while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_, p.z_))) {
			for (BoundingVolume* bv : *bucket) {
				query(bv, view_matrix, proj_matrix);
			}
			int i = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_ + i, p.y_, p.z_))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				i++;
			}
			i = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_ - i, p.y_, p.z_))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				i++;
			}
			int j = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_ + j, p.z_))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				j++;
			}
			j = 1;
			while (nullptr != (bucket = spatialMap_.getBucket(p.x_, p.y_ - j, p.z_))) {
				for (BoundingVolume* bv : *bucket) {
					query(bv, view_matrix, proj_matrix);
				}
				j++;
			}
			p.z_ += move;
		}
	}
}
