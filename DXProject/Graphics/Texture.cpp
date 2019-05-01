#include "Texture.h"
#include "../Utilities//ErrorLogger.h"
#include "../Utilities/StringHelper.h"

#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

Texture::Texture(ID3D11Device * device, const Color & color, aiTextureType type) : type_(aiTextureType::aiTextureType_UNKNOWN){
	initializeColorTexture(device, &color, 1, 1, type);
}

Texture::Texture(ID3D11Device * device, const Color * color_data, UINT width, UINT height, aiTextureType type) : type_(aiTextureType::aiTextureType_UNKNOWN) {
	initializeColorTexture(device, color_data, width, height, type);
}

Texture::Texture(ID3D11Device * device, const std::wstring & file_path, aiTextureType type) {
	std::wstring extension = StringHelper::getFileExtension(file_path);
	if (extension == L".dds") {
		HRESULT hr = DirectX::CreateDDSTextureFromFile(device, file_path.c_str(), resource_.GetAddressOf(), resourceView_.GetAddressOf());
		if (FAILED(hr)) {
			initializeColorTexture(device, &Colors::UnloadedColor, 1, 1, type);
		}
	}
	else {
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, file_path.c_str(), resource_.GetAddressOf(), resourceView_.GetAddressOf());
		if (FAILED(hr)) {
			initializeColorTexture(device, &Colors::UnloadedColor, 1, 1, type);
		}
	}

	type_ = type;
}

Texture::Texture(ID3D11Device * device, const uint8_t * data, size_t size, aiTextureType type) {
	HRESULT hr = DirectX::CreateWICTextureFromMemory(device, data, size, resource_.GetAddressOf(), resourceView_.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, L"Falied to create texture from memory.");
	type_ = type;
}

Texture::~Texture() {
}

aiTextureType Texture::getType() {
	return type_;
}

ID3D11ShaderResourceView * Texture::getResourceView() {
	return resourceView_.Get();
}

ID3D11ShaderResourceView ** Texture::getResourceViewAddress() {
	return resourceView_.GetAddressOf();
}

void Texture::initializeColorTexture(ID3D11Device * device, const Color * color_data, UINT width, UINT height, aiTextureType type) {
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

	type_ = type;
}
