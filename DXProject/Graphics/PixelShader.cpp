#include "PixelShader.h"



PixelShader::PixelShader() {
}


PixelShader::~PixelShader() {
}

bool PixelShader::initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shader_path) {
	HRESULT hr = D3DReadFileToBlob(shader_path.c_str(), shaderBuffer_.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Failed to load shader: " + shader_path);
		return false;
	}

	hr = device->CreatePixelShader(shaderBuffer_->GetBufferPointer(), shaderBuffer_->GetBufferSize(), NULL, shader_.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Failed to create ID3D11PixelShader." + shader_path);
		return false;
	}

	return true;
}

ID3D11PixelShader * PixelShader::getShader() {
	return shader_.Get();
}

ID3DBlob * PixelShader::getBuffer() {
	return shaderBuffer_.Get();
}
