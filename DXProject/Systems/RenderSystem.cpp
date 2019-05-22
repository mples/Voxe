#include "RenderSystem.h"
#include "../Engine.h"

RenderSystem::RenderSystem(HWND hwd, int width, int height) : windowWidth_(width), windowHeight_(height), enableMsaa_(true) {
	initializeDirectX(hwd);
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::preUpdate(float dt) {
}

void RenderSystem::update(float dt) {
}

void RenderSystem::postUpdate(float dt) {
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

		//deviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Height = static_cast<float>(windowHeight_);
		viewport.Width = static_cast<float>(windowWidth_);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		deviceContext_->RSSetViewports(1, &viewport);

		spriteBatch_ = std::make_unique<DirectX::SpriteBatch>(deviceContext_.Get());
		spriteFont_ = std::make_unique<DirectX::SpriteFont>(device_.Get(), L"Data/Fonts/arial_16.spritefont");

	}
	catch (COMException ex) {
		ErrorLogger::log(ex);
		return;
	}

	return;
}
