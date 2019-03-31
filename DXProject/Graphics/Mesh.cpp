#include "Mesh.h"

Mesh::Mesh(ID3D11Device * device, ID3D11DeviceContext * device_context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture> & textures, const DirectX::XMMATRIX & transform) {
	deviceContext_ = device_context;
	textures_ = textures;
	transformMatrix_ = transform;

	HRESULT hr = vertexBuffer_.initialize(device, vertices.data(), vertices.size());
	COM_ERROR_IF_FAILED(hr, L"Falied to create vertex buffer from mesh.");

	hr = indexBuffer_.initialize(device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(hr, L"Falied to create index buffer from mesh.");
}

Mesh::Mesh(const Mesh & other) {
	deviceContext_ = other.deviceContext_;
	indexBuffer_ = other.indexBuffer_;
	vertexBuffer_ = other.vertexBuffer_;
	textures_ = other.textures_;
	transformMatrix_ = other.transformMatrix_;
}

Mesh::~Mesh() {
}

void Mesh::draw() {
	for (auto texture : textures_) {
		if (texture.getType() == aiTextureType::aiTextureType_DIFFUSE) {
			deviceContext_->PSSetShaderResources(0, 1, texture.getResourceViewAddress());
			break;
		}
	}

	UINT offset = 0;
	deviceContext_->IASetVertexBuffers(0, 1, vertexBuffer_.getAddressOf(), vertexBuffer_.stridePtr(), &offset);
	deviceContext_->IASetIndexBuffer(indexBuffer_.get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	deviceContext_->DrawIndexed(indexBuffer_.indicesCount(), 0, 0);
}

const DirectX::XMMATRIX & Mesh::getTransform() {
	return transformMatrix_;
}
