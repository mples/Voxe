#include "RenderSystem.h"
#include "../Engine.h"
#include "../Events/DirectXDeviceCreated.h"
#include "../Events/BlockTextureAtlasCreated.h"

RenderSystem::RenderSystem(HWND hwd, int width, int height) : windowWidth_(width),
																windowHeight_(height), 
																enableMsaa_(true), 
																IEventListener(ENGINE.getEventHandler() ),
																activeCamera_(nullptr) {
	initializeDirectX(hwd);

	ENGINE.sendEvent<DirectXDeviceCreated>(device_.Get(), deviceContext_.Get());

	initializeRenderState();
	if (!initializeShaders()) {
		assert(0 && "Failed to initialized shaders");
	}

	HRESULT hr = objectBufferVS_.initialize(device_.Get(), deviceContext_.Get());
	COM_ERROR_IF_FAILED(hr, L"Falied to initialize constant buffer - objectBufferVS_.");

	hr = frameBufferPS_.initialize(device_.Get(), deviceContext_.Get());
	COM_ERROR_IF_FAILED(hr, L"Falied to initialize constant buffer - frameBufferPS_.");

	hr = fogBuffer_.initialize(device_.Get(), deviceContext_.Get());
	COM_ERROR_IF_FAILED(hr, L"Falied to initialize constant buffer - fogBuffer_.");

	hr = pointLightBuffer_.initialize(device_.Get(), deviceContext_.Get());
	COM_ERROR_IF_FAILED(hr, L"Falied to initialize constant buffer - pointLightBuffer_.");

	//default values for pointlight
	pointLightBuffer_.data_.ambientColor_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	pointLightBuffer_.data_.ambientStrength_ = 0.7f;

	pointLightBuffer_.data_.diffuseColor_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	pointLightBuffer_.data_.diffuseStrength_ = 1.0f;

	pointLightBuffer_.data_.specularColor_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	pointLightBuffer_.data_.specularStrength_ = 1.0f;

	pointLightBuffer_.data_.lightAttenuationA_ = 0.0f;
	pointLightBuffer_.data_.lightAttenuationB_ = 0.1f;
	pointLightBuffer_.data_.lightAttenuationC_ = 0.0f;

	pointLightBuffer_.data_.pos_ = XMFLOAT3(0.0f, 25.0f, -5.0f);
	pointLightBuffer_.data_.range_ = 160.0f;

	pointLightBuffer_.applyChanges();

	//deafault values for fog rendering
	fogBuffer_.data_.fogColor_ = XMFLOAT3(0.6f, 0.8f, 1.0f);
	fogBuffer_.data_.fogStart_ = 32.0f;
	fogBuffer_.data_.fogEnd_ = 96.0f;
	fogBuffer_.applyChanges();

	invalidTexture_ = new Texture(device_.Get(), Color(0, 0, 0));

	registerEventCallback<CameraCreated>([&](const CameraCreated* e) {
		onCameraCreatedEvent(e);
	});
	registerEventCallback<CameraDestroyed>([&](const CameraDestroyed* e) {
		onCameraDestroyedEvent(e);
	});
	registerEventCallback<FogChangeRequest>([&](const FogChangeRequest* e) {
		onFogChangeRequest(e);
	});
	registerEventCallback<SetPointLightRequest>([&](const SetPointLightRequest* e) {
		onSetPointLightRequest(e);
	});
}

RenderSystem::~RenderSystem() {
	if(blocksTextureAtlas_ != nullptr)
		delete blocksTextureAtlas_;
	if(invalidTexture_ != nullptr)
		delete invalidTexture_;
}

void RenderSystem::preUpdate(float dt) {
	deviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

	float bg_color[] = { 0.25f, 0.25f, 1.0f, 1.0f };

	deviceContext_->ClearRenderTargetView(renderTargetView_.Get(), bg_color);
	deviceContext_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	deviceContext_->RSSetState(rasterizerState_.Get());
	deviceContext_->OMSetBlendState(blendState_.Get(), NULL, 0xffffffff);
	deviceContext_->OMSetDepthStencilState(depthStencilState_.Get(), 0);

	deviceContext_->PSSetSamplers(0, 1, samplerState_.GetAddressOf());

	deviceContext_->IASetInputLayout(vertexShader_.getInputLayout());
	deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext_->VSSetShader(vertexShader_.getShader(), NULL, 0);
	deviceContext_->PSSetShader(pixelShader_.getShader(), NULL, 0);

	deviceContext_->RSSetViewports(1, &viewport_);
}

void RenderSystem::update(float dt) {
	auto it = ENGINE.getComponentManager().begin<MeshComponent>();
	auto end = ENGINE.getComponentManager().end<MeshComponent>();

	while (it != end) {
		if (it->getVisiblility() == true) {
			WorldCoordinateComponent * wcoord_comp = ENGINE.getComponentManager().getComponentByEntityId<WorldCoordinateComponent>(it->getOwner());
			if (wcoord_comp != nullptr) {
				drawObject(&(*it), wcoord_comp);
			}


		}
		++it;
	}
}

void RenderSystem::postUpdate(float dt) {
	swapChain_->Present(0, NULL);
}

void RenderSystem::createTextureAtlas(std::wstring texture_atlas_path, const std::vector<BlockType> block_types) {
	blocksTextureAtlas_ = new BlocksTextureAtlas(device_.Get(), texture_atlas_path, block_types);
	ENGINE.sendEvent<BlockTextureAtlasCreated>(blocksTextureAtlas_);
}

void RenderSystem::initializeDirectX(HWND hwnd) {
	try {
		AdapterData* adapter = AdapterReader::getMainAdapter();
		if (adapter == nullptr) {
			ErrorLogger::log("Error: No IDXGI Adapters found.");
			return;
		}

		UINT create_device_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		create_device_flags = D3D11_CREATE_DEVICE_DEBUG;
#endif // defined(DEBUG) || defined(_DEBUG)


		D3D_FEATURE_LEVEL feature_lvls_table[] = { D3D_FEATURE_LEVEL_11_1,
														D3D_FEATURE_LEVEL_11_0,
														D3D_FEATURE_LEVEL_10_1,
														D3D_FEATURE_LEVEL_10_0,
														D3D_FEATURE_LEVEL_9_3,
														D3D_FEATURE_LEVEL_9_2,
														D3D_FEATURE_LEVEL_9_1 };
		D3D_FEATURE_LEVEL feature_level;

		HRESULT hr = D3D11CreateDevice(adapter->adapter_,
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			create_device_flags,
			feature_lvls_table,
			_countof(feature_lvls_table),
			D3D11_SDK_VERSION,
			device_.GetAddressOf(),
			&feature_level,
			deviceContext_.GetAddressOf());
		if (hr == E_INVALIDARG) {
			hr = D3D11CreateDevice(adapter->adapter_,
				D3D_DRIVER_TYPE_UNKNOWN,
				nullptr,
				create_device_flags,
				&feature_lvls_table[1],
				_countof(feature_lvls_table) - 1,
				D3D11_SDK_VERSION,
				device_.GetAddressOf(),
				&feature_level,
				deviceContext_.GetAddressOf());
		}
		COM_ERROR_IF_FAILED(hr, L"Failed to create D3D11Device.");

		if (feature_level != D3D_FEATURE_LEVEL_11_0 && feature_level != D3D_FEATURE_LEVEL_11_1) {
			ErrorLogger::log(hr, "Error: Feature level DirectX 11 is not supported.");
			return;
		}

		UINT msaa_quality;
		device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_quality);
		assert(msaa_quality > 0);

		DXGI_SWAP_CHAIN_DESC scd;
		scd.BufferDesc.Width = windowWidth_;
		scd.BufferDesc.Height = windowHeight_;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		if (enableMsaa_) {
			scd.SampleDesc.Count = 4;
			scd.SampleDesc.Quality = msaa_quality - 1;
		}
		else {
			scd.SampleDesc.Count = 1;
			scd.SampleDesc.Quality = 0;
		}

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = true;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgi_device;
		hr = device_->QueryInterface(__uuidof(IDXGIDevice), (reinterpret_cast<void**>(dxgi_device.GetAddressOf())));
		COM_ERROR_IF_FAILED(hr, L"Failed to get IDXGIDevice.");

		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgi_adapter;
		hr = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (reinterpret_cast<void**>(dxgi_adapter.GetAddressOf())));
		COM_ERROR_IF_FAILED(hr, L"Failed to get IDXGIAdapter.");

		Microsoft::WRL::ComPtr<IDXGIFactory> dxgi_factory;
		hr = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (reinterpret_cast<void**>(dxgi_factory.GetAddressOf())));
		COM_ERROR_IF_FAILED(hr, L"Error: Failed to get IDXGIFactory.");

		hr = dxgi_factory->CreateSwapChain(device_.Get(), &scd, swapChain_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create IDXGISwapChain.");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;

		hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, L"Failed to get back buffer.");

		hr = device_->CreateRenderTargetView(back_buffer.Get(), NULL, renderTargetView_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11RenderTargetView.");

		D3D11_TEXTURE2D_DESC dsbd;
		dsbd.Width = windowWidth_;
		dsbd.Height = windowHeight_;
		dsbd.MipLevels = 1;
		dsbd.ArraySize = 1;
		dsbd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		if (enableMsaa_) {
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

		hr = device_->CreateTexture2D(&dsbd, nullptr, depthStencilBuffer_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11Texture2D for depth stancil buffer.");

		hr = device_->CreateDepthStencilView(depthStencilBuffer_.Get(), nullptr, depthStencilView_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11DepthStencilView.");

		ZeroMemory(&viewport_, sizeof(D3D11_VIEWPORT));

		viewport_.TopLeftX = 0;
		viewport_.TopLeftY = 0;
		viewport_.Height = static_cast<float>(windowHeight_);
		viewport_.Width = static_cast<float>(windowWidth_);
		viewport_.MinDepth = 0.0f;
		viewport_.MaxDepth = 1.0f;



		spriteBatch_ = std::make_unique<DirectX::SpriteBatch>(deviceContext_.Get());
		spriteFont_ = std::make_unique<DirectX::SpriteFont>(device_.Get(), L"Data/Fonts/arial_16.spritefont");
	}
	catch (COMException ex) {
		ErrorLogger::log(ex);
		return;
	}

	return;
}

void RenderSystem::initializeRenderState() {
	try {
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

		//TODO delete debugging
		D3D11_DEPTH_STENCIL_DESC ds_off_desc;
		ZeroMemory(&ds_off_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		ds_off_desc.DepthEnable = false;
		ds_off_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		ds_off_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

		device_->CreateDepthStencilState(&ds_off_desc, depthStencilStateOff_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11DepthStencilStateoff.");
		//end delete

		D3D11_SAMPLER_DESC sampl_desc;
		ZeroMemory(&sampl_desc, sizeof(D3D11_SAMPLER_DESC));
		//sampl_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampl_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
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
	catch (COMException e) {
		ErrorLogger::log(e);
		return;
	}
}

bool RenderSystem::initializeShaders() {
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

void RenderSystem::drawObject(MeshComponent * mesh, WorldCoordinateComponent* coord) {
	deviceContext_->PSSetConstantBuffers(2, 1, fogBuffer_.getAddressOf());
	deviceContext_->PSSetConstantBuffers(1, 1, pointLightBuffer_.getAddressOf());

	deviceContext_->VSSetConstantBuffers(0, 1, objectBufferVS_.getAddressOf());
	if (activeCamera_ != nullptr) {
		objectBufferVS_.data_.mvpMatrix_ = coord->getWorldMatrix() * activeCamera_->getViewMatrix() * activeCamera_->getProjectionMatrix();
		frameBufferPS_.data_.eyePos_ = activeCamera_->getCameraPosition();
		frameBufferPS_.applyChanges();
		deviceContext_->PSSetConstantBuffers(0, 1, frameBufferPS_.getAddressOf());
	}
	else {
		objectBufferVS_.data_.mvpMatrix_ = coord->getWorldMatrix();
	}
	objectBufferVS_.data_.modelMatrix_ = coord->getWorldMatrix();
	objectBufferVS_.applyChanges();

	if (blocksTextureAtlas_) {
		deviceContext_->PSSetShaderResources(0, 1, blocksTextureAtlas_->getResourceViewAddress());
	}
	else {
		deviceContext_->PSSetShaderResources(0, 1, invalidTexture_->getResourceViewAddress());
	}

	UINT offset = 0;
	deviceContext_->IASetVertexBuffers(0, 1, mesh->getVertexBuffer().getAddressOf(), mesh->getVertexBuffer().stridePtr(), &offset);
	deviceContext_->IASetIndexBuffer(mesh->getIndexBuffer().get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	deviceContext_->DrawIndexed(mesh->getIndexBuffer().indicesCount(), 0, 0);

}

void RenderSystem::onSetPointLightRequest(const SetPointLightRequest * e) {
	pointLightBuffer_.data_.ambientColor_ = e->color_;
	pointLightBuffer_.data_.ambientStrength_ = e->ambStrenght_;

	pointLightBuffer_.data_.diffuseColor_ = e->color_;
	pointLightBuffer_.data_.diffuseStrength_ = e->diffStrenght_;

	pointLightBuffer_.data_.specularColor_ = e->color_;
	pointLightBuffer_.data_.specularStrength_ = e->specStrenght_;

	pointLightBuffer_.data_.pos_ = e->position_;
	pointLightBuffer_.data_.range_ = e->range_;

	pointLightBuffer_.applyChanges();
}

void RenderSystem::onFogChangeRequest(const FogChangeRequest * e) {
	fogBuffer_.data_.fogColor_ = e->fogColor_;
	fogBuffer_.data_.fogStart_ = e->fogStart_;
	fogBuffer_.data_.fogEnd_ = e->fogEnd_;
	fogBuffer_.applyChanges();
}

void RenderSystem::onCameraCreatedEvent(const CameraCreated * e) {
	if (activeCamera_ == nullptr) {
		activeCamera_ = dynamic_cast<IGameCamera*>(ENGINE.getEntityManager().getEntity<GameCamera>(e->id_));
		assert(activeCamera_ != nullptr && "Falied to get IGameCamera pointer.");
	}
	else {
		assert(0 && "There should bo only one active camera");
	}
}

void RenderSystem::onCameraDestroyedEvent(const CameraDestroyed * e) {
	activeCamera_ = nullptr;
}

