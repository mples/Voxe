#include "VertexShader.h"



VertexShader::VertexShader() {
}


VertexShader::~VertexShader() {
}

bool VertexShader::initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shader_path, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements) {


	HRESULT hr = D3DReadFileToBlob(shader_path.c_str(), shaderBuffer_.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Failed to load shader: " + shader_path); 
		return false;
	}
	hr = device->CreateVertexShader(shaderBuffer_->GetBufferPointer(), shaderBuffer_->GetBufferSize(), nullptr, shader_.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Failed to create shader: " + shader_path);
		return false;
	}

 	hr = device->CreateInputLayout(input_element_desc, num_elements, shaderBuffer_->GetBufferPointer(), shaderBuffer_->GetBufferSize(), inputLayout_.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::log(hr, "Error: Failed to create ID3D11InputLayout.");
		return false;
	}
	return true;
}

bool VertexShader::initialize(ID3D11Device * device, std::wstring shader_path, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements) {


	HRESULT hr = D3DReadFileToBlob(shader_path.c_str(), shaderBuffer_.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Failed to load shader: " + shader_path);
		return false;
	}
	hr = device->CreateVertexShader(shaderBuffer_->GetBufferPointer(), shaderBuffer_->GetBufferSize(), nullptr, shader_.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::log(hr, L"Failed to create shader: " + shader_path);
		return false;
	}

	hr = device->CreateInputLayout(input_element_desc, num_elements, shaderBuffer_->GetBufferPointer(), shaderBuffer_->GetBufferSize(), inputLayout_.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::log(hr, "Error: Failed to create ID3D11InputLayout.");
		return false;
	}
	return true;
}

ID3D11VertexShader * VertexShader::getShader() {
	return shader_.Get();
}

ID3DBlob * VertexShader::getBuffer() {
	return shaderBuffer_.Get();
}

ID3D11InputLayout * VertexShader::getInputLayout() {
	return inputLayout_.Get();
}
