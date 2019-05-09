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
	
	boundingVolume_.initialize(device_, deviceContext_);

	if (!object_.initialize("Data/Objects/nanosuit/nanosuit.obj", device_, deviceContext_, objectBuffer_)) {
		return false;
	}
	
	spatialMap_.insert(&boundingVolume_);

	return true;
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

void OcclusionCulling::cull(const DirectX::XMMATRIX & view_matrix, const DirectX::XMMATRIX & proj_matrix, std::vector<Chunk*>& chunks) {
	spatialMap_.clear();

	for (Chunk* chunk : chunks) {
		spatialMap_.insert(&chunk->getBV());
	}
	XMFLOAT3 camera_front = getCameraFrontDir(view_matrix);




	deviceContext_->OMSetRenderTargets(0, NULL, depthStencilView_.Get());

	deviceContext_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext_->IASetInputLayout(vertexShader_.getInputLayout());
	deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext_->VSSetShader(vertexShader_.getShader(), NULL, 0);
	deviceContext_->PSSetShader(NULL, NULL, 0);


	D3D11_QUERY_DESC query_desc;
	query_desc.Query = D3D11_QUERY::D3D11_QUERY_OCCLUSION;
	query_desc.MiscFlags = 0u;
	ID3D11Query * query;
	device_->CreateQuery(&query_desc, &query);

	deviceContext_->Begin(query);

	deviceContext_->VSSetConstantBuffers(0, 1, objectBuffer_.getAddressOf());
	object_.draw(XMMatrixIdentity());


	objectBuffer_.data_.mvpMatrix_ = XMMatrixIdentity() * view_matrix * proj_matrix;
	objectBuffer_.data_.modelMatrix_ = XMMatrixIdentity();

	objectBuffer_.applyChanges();

	boundingVolume_.draw();

	deviceContext_->End(query);

	UINT64 query_data;

	while (S_OK != deviceContext_->GetData(query, &query_data, sizeof(UINT64), 0)) {

	}

	char buffer[100];
	sprintf_s(buffer, "Pixels drawn: %d\n", query_data);
	OutputDebugStringA(buffer);
}

XMFLOAT3 OcclusionCulling::getCameraFrontDir(const DirectX::XMMATRIX & view_matrix) {
	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, view_matrix);
	return XMFLOAT3(view._13, view._23, view._33);
}
