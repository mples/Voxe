#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <assimp/material.h>

#include "Color.h"

enum class TextureStorageType {
	INVALID,
	NONE,
	EMBEDDED_INDEX_COMPRESSED,
	EMBEDDED_INDEX_NON_COMPRESSED,
	EMBEDDED_COMPRESSED,
	EMBEDDED_NON_COMPRESSED,
	DISK
};

class Texture {
public:
	Texture(ID3D11Device * device, const Color & color);
	Texture(ID3D11Device * device, const Color * color_data, UINT width, UINT height);
	Texture(ID3D11Device * device, const std::wstring & file_path);
	Texture(ID3D11Device * device, const uint8_t* data, size_t size);
	Texture(const Texture & other);
	~Texture();
	ID3D11ShaderResourceView * getResourceView();
	ID3D11ShaderResourceView ** getResourceViewAddress();
private:
	void initializeColorTexture(ID3D11Device * device, const Color * color_data, UINT width, UINT height);

	Microsoft::WRL::ComPtr<ID3D11Resource> resource_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView_;
};

