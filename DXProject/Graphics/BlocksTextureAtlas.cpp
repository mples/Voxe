#include "BlocksTextureAtlas.h"
#include "../Utilities//ErrorLogger.h"
#include "../Utilities/StringHelper.h"

#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

BlocksTextureAtlas::BlocksTextureAtlas(ID3D11Device * device, const std::wstring & file_path, std::vector<BlockType> block_types) {
	std::wstring extension = StringHelper::getFileExtension(file_path);
	if (extension == L".dds") {
		HRESULT hr = DirectX::CreateDDSTextureFromFile(device, file_path.c_str(), resource_.GetAddressOf(), resourceView_.GetAddressOf());
		if (FAILED(hr)) {
			initializeColorTexture(device, &Colors::UnloadedColor, 1, 1);
		}
	}
	else {
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, file_path.c_str(), resource_.GetAddressOf(), resourceView_.GetAddressOf());
		if (FAILED(hr)) {
			initializeColorTexture(device, &Colors::UnloadedColor, 1, 1);
		}
	}
	ID3D11Texture2D * texture_interface = 0;

	resource_->QueryInterface<ID3D11Texture2D>(&texture_interface);
	D3D11_TEXTURE2D_DESC desc;
	texture_interface->GetDesc(&desc);

	width_ = desc.Width;
	height_ = desc.Height;

	singleTextureDim_ = desc.Height / 6;
	

	for (int i = 0; i < block_types.size(); i++) {
		coordinateMap_.insert(std::make_pair(block_types[i], XMFLOAT2(i * singleTextureDim_, 0)));
	}
}

BlocksTextureAtlas::~BlocksTextureAtlas() {
	
}

ID3D11ShaderResourceView * BlocksTextureAtlas::getResourceView() {
	return resourceView_.Get();
}

ID3D11ShaderResourceView ** BlocksTextureAtlas::getResourceViewAddress() {
	return resourceView_.GetAddressOf();
}

XMFLOAT2 BlocksTextureAtlas::getTextureCoord(BlockType type, TextureSide side, TextureVertex vertex) {
	XMFLOAT2 tex_origin = coordinateMap_.at(type);

	tex_origin.y += static_cast<float>(side) * singleTextureDim_;

	switch (vertex) {
	case TextureVertex::_00:
		tex_origin.y += static_cast<float>(singleTextureDim_) - 1.0f;
		break;
	case TextureVertex::_01:
		tex_origin.x += static_cast<float>(singleTextureDim_) - 1.0f;
		tex_origin.y += static_cast<float>(singleTextureDim_) - 1.0f;
		break;
	case TextureVertex::_10:
		break;
	case TextureVertex::_11:
		tex_origin.x += static_cast<float>(singleTextureDim_) - 1.0f;
		break;
	}
	tex_origin.x /= width_;
	tex_origin.y /= height_;
	return tex_origin;
}

void BlocksTextureAtlas::initializeColorTexture(ID3D11Device * device, const Color * color_data, UINT width, UINT height) {
	CD3D11_TEXTURE2D_DESC tex_desc(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D * texture = nullptr;
	D3D11_SUBRESOURCE_DATA init_data{};
	init_data.pSysMem = color_data;
	init_data.SysMemPitch = width * sizeof(Color);
	HRESULT hr = device->CreateTexture2D(&tex_desc, &init_data, &texture);
	COM_ERROR_IF_FAILED(hr, L"Falied to create texture2d from color data");
	resource_ = static_cast<ID3D11Resource*>(texture);

	CD3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_desc(D3D11_SRV_DIMENSION_TEXTURE2D, tex_desc.Format);
	hr = device->CreateShaderResourceView(resource_.Get(), &shader_resource_desc, resourceView_.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, L"Falied to create shader resource view.");

}