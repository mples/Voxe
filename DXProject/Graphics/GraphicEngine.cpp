#include "GraphicEngine.h"
#include <cassert>


GraphicEngine::GraphicEngine() : enableMsaa_(true)  {
	perlinNoise_ = new PerlinNoise(1234);
	simpleGenerator_ = new SimpleGenerator(perlinNoise_);
	world_ = new World(simpleGenerator_);
}


GraphicEngine::~GraphicEngine() {
	delete perlinNoise_;
	delete simpleGenerator_;
	delete world_;
}

bool GraphicEngine::initialize(HWND hwnd, int width, int height) {
	fpsTimer_.start();
	windowHeight_ = height;
	windowWidth_ = width;
	if (!initializeDirectX(hwnd)) {
		return  false;
	}
	if (!initializeShaders()) {
		return false;
	}
	if (!initializeScene()) {
		return false;
	}

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device_.Get(), deviceContext_.Get());
	ImGui::StyleColorsDark();

	return true;
}

bool GraphicEngine::initializeShaders() {
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

	if (!vertexShader_.initialize(device_, shader_folder + L"shader_vertex.cso", layout_desc, _countof(layout_desc) )) {
		return false;
	}

	if (!pixelShader_.initialize(device_, shader_folder + L"shader_pixel.cso")) {
		return false;
	}

	return true;
}

bool GraphicEngine::initializeScene() {
	try {
		HRESULT hr = DirectX::CreateWICTextureFromFile(device_.Get(), L"Data/Textures/grass_texture.jpg", nullptr, texture_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Error: Failed to create ID3D11ShaderResourceView.");

		hr = VSconstantBuffer_.initialize(device_.Get(), deviceContext_.Get());
		COM_ERROR_IF_FAILED(hr, L"Error: Failed to initialize VS ConstantBuffer.");

		hr = PSpointLightBuffer_.initialize(device_.Get(), deviceContext_.Get());
		COM_ERROR_IF_FAILED(hr, L"Error: Failed to initialize PS point light buffer.");

		hr = PSframeBuffer_.initialize(device_.Get(), deviceContext_.Get());
		COM_ERROR_IF_FAILED(hr, L"Error: Failed to initialize PS frame buffer.");

		PSpointLightBuffer_.data_.ambientColor_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
		PSpointLightBuffer_.data_.ambientStrength_ = 1.0f;

		if (!gameObject_.initialize("Data/Objects/nanosuit/nanosuit.obj", device_.Get(), deviceContext_.Get(), VSconstantBuffer_)) {
			return false;
		}

		camera_.setPos(0.0f, 0.0f, -5.0f);
		camera_.setProjData(90.0f, static_cast<float>(windowWidth_) / static_cast<float>(windowHeight_), 0.1f, 1000.0f);

		//test

		PSpointLightBuffer_.data_.diffuseColor_ = light_.color_;
		PSpointLightBuffer_.data_.diffuseStrength_ = light_.strength_;
		PSpointLightBuffer_.data_.specularColor_ = light_.color_;
		PSpointLightBuffer_.data_.specularStrength_ = light_.strength_;
		PSpointLightBuffer_.data_.pos_ = light_.getPos();
		PSpointLightBuffer_.data_.lightAttenuationA_ = light_.attenuationA_;
		PSpointLightBuffer_.data_.lightAttenuationB_ = light_.attenuationB_;
		PSpointLightBuffer_.data_.lightAttenuationC_ = light_.attenuationC_;

		Texture* tex = new Texture(device_.Get(), L"Data/Textures/grass.jpg", aiTextureType_DIFFUSE);

		//chunk_.initialize(device_.Get(), deviceContext_.Get(), VSconstantBuffer_, tex);

		if (!light_.initialize(device_.Get(), deviceContext_.Get(), VSconstantBuffer_)) {
			return false;
		}

		chunkRenderer_.setWorld(world_);
		chunkRenderer_.initialize(device_.Get(), deviceContext_.Get());
		
	}
	catch (COMException ex) {
		ErrorLogger::log(ex);
		return false;
	}
	return true;
}

void GraphicEngine::draw() {
	float bg_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	deviceContext_->ClearRenderTargetView(renderTargetView_.Get(), bg_color);
	deviceContext_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	deviceContext_->IASetInputLayout(vertexShader_.getInputLayout() );
	deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext_->RSSetState(rasterizerState_.Get());
	deviceContext_->OMSetBlendState(blendState_.Get(), NULL, 0xffffffff);
	deviceContext_->OMSetDepthStencilState(depthStencilState_.Get(), 0);

	deviceContext_->PSSetSamplers(0, 1, samplerState_.GetAddressOf());

	deviceContext_->VSSetShader(vertexShader_.getShader(), NULL, 0);
	deviceContext_->PSSetShader(pixelShader_.getShader(), NULL, 0);
	//set pont light buffer
	//PSpointLightBuffer_.data_.diffuseColor_ = light_.color_;
	//PSpointLightBuffer_.data_.diffuseStrength_ = light_.strength_;
	//PSpointLightBuffer_.data_.specularColor_ = light_.color_;
	//PSpointLightBuffer_.data_.specularStrength_ = light_.strength_;
	PSpointLightBuffer_.data_.pos_ = light_.getPos();
	//PSpointLightBuffer_.data_.lightAttenuationA_ = light_.attenuationA_;
	//PSpointLightBuffer_.data_.lightAttenuationB_ = light_.attenuationB_;
	//PSpointLightBuffer_.data_.lightAttenuationC_ = light_.attenuationC_;
	PSpointLightBuffer_.applyChanges();
	deviceContext_->PSSetConstantBuffers(1, 1, PSpointLightBuffer_.getAddressOf());

	//set fram buffer
	PSframeBuffer_.data_.eyePos_ = camera_.getPos();
	PSframeBuffer_.applyChanges();
	deviceContext_->PSSetConstantBuffers(0, 1, PSframeBuffer_.getAddressOf());

	//draw
	gameObject_.draw(camera_.getViewMatrix() * camera_.getProjMatrix());

	light_.draw(camera_.getViewMatrix() * camera_.getProjMatrix());

	//chunk_.draw(XMMatrixIdentity(), camera_.getViewMatrix() * camera_.getProjMatrix());

	chunkRenderer_.draw(camera_.getViewMatrix() * camera_.getProjMatrix());

	static int fps_counter = 0;
	static std::wstring fps_wstring = L"FPS: 0";
	fps_counter++;
	if (fpsTimer_.getTimePassedMilisec() > 1000.0) {
		fps_wstring = L"FPS: " + std::to_wstring(fps_counter);
		fps_counter = 0;
		fpsTimer_.restart();
	}
	spriteBatch_->Begin();
	spriteFont_->DrawString(spriteBatch_.get(), fps_wstring.c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::LightSeaGreen, 0.0f, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch_->End();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Light controls");
	ImGui::DragFloat("Ambient Light Strength", &PSpointLightBuffer_.data_.ambientStrength_, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Ambient Light Color", &PSpointLightBuffer_.data_.ambientColor_.x, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Dynamic Light attenuation A", &PSpointLightBuffer_.data_.lightAttenuationA_, 0.01f, 0.0f, 5.0f);
	ImGui::DragFloat("Dynamic Light attenuation B", &PSpointLightBuffer_.data_.lightAttenuationB_, 0.01f, 0.0f, 5.0f);
	ImGui::DragFloat("Dynamic Light attenuation C", &PSpointLightBuffer_.data_.lightAttenuationC_, 0.01f, 0.0f, 5.0f);
	ImGui::DragFloat("Diffuse Light Strength", &PSpointLightBuffer_.data_.diffuseStrength_, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat3("Diffuse Light Color", &PSpointLightBuffer_.data_.diffuseColor_.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Specular Light Strength", &PSpointLightBuffer_.data_.specularStrength_, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat3("Specular Light Color", &PSpointLightBuffer_.data_.specularColor_.x, 0.01f, 0.0f, 1.0f);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	swapChain_->Present(0, NULL);
}

bool GraphicEngine::initializeDirectX(HWND hwnd) {
	try {
		AdapterData* adapter = AdapterReader::getMainAdapter();
		if (adapter == nullptr) {
			ErrorLogger::log("Error: No IDXGI Adapters found.");
			return false;
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
											deviceContext_.GetAddressOf() );
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
			return false;
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

		deviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Height = static_cast<float>(windowHeight_);
		viewport.Width = static_cast<float>(windowWidth_);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		deviceContext_->RSSetViewports(1, &viewport);

		D3D11_RASTERIZER_DESC rast_desc;
		ZeroMemory(&rast_desc, sizeof(D3D11_RASTERIZER_DESC));

		rast_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rast_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

		hr = device_->CreateRasterizerState(&rast_desc, rasterizerState_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11RasterizerState.");

		D3D11_DEPTH_STENCIL_DESC ds_desc;
		ZeroMemory(&ds_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		ds_desc.DepthEnable = true;
		ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		ds_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

		device_->CreateDepthStencilState(&ds_desc, depthStencilState_.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create ID3D11DepthStencilState.");

		spriteBatch_ = std::make_unique<DirectX::SpriteBatch>(deviceContext_.Get());
		spriteFont_ = std::make_unique<DirectX::SpriteFont>(device_.Get(), L"Data/Fonts/arial_16.spritefont");

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
	catch (COMException ex) {
		ErrorLogger::log(ex);
		return false;
	}

	return true;
}
