#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

#include "../Utilities/ErrorLogger.h"

class PixelShader {
public:
	PixelShader();
	~PixelShader();
	bool initialize(Microsoft::WRL::ComPtr<ID3D11Device > & device, std::wstring shader_path);
	bool initialize(ID3D11Device * device, std::wstring shader_path);
	ID3D11PixelShader* getShader();
	ID3DBlob* getBuffer();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader_;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer_;
};

