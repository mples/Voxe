#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

#include "../Utilities/ErrorLogger.h"

class VertexShader {
public:
	VertexShader();
	~VertexShader();
	bool initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shader_path, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);
	bool initialize(ID3D11Device * device, std::wstring shader_path, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);
	ID3D11VertexShader* getShader();
	ID3DBlob* getBuffer();
	ID3D11InputLayout* getInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader_;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer_;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout_;

};

