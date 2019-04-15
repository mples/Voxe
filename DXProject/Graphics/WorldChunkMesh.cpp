#include "WorldChunkMesh.h"

#include "../ErrorLogger.h"

WorldChunkMesh::WorldChunkMesh() {
}


WorldChunkMesh::~WorldChunkMesh() {
}

bool WorldChunkMesh::initialize(ID3D11Device * device, ID3D11DeviceContext * device_context, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, Texture* texture) {
	deviceContext_ = device_context;
	texture_ = texture;

	HRESULT hr = vertexBuffer_.initialize(device, vertices.data(), vertices.size());
	COM_ERROR_IF_FAILED(hr, L"Failed to initialize vertex buffer.");

	hr = indexBuffer_.initialize(device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(hr, L"Failed to initialize index buffer.");

	return true;
}

void WorldChunkMesh::draw() {

	deviceContext_->PSSetShaderResources(0, 1, texture_->getResourceViewAddress());
	UINT offset = 0;
	deviceContext_->IASetVertexBuffers(0, 1, vertexBuffer_.getAddressOf(), vertexBuffer_.stridePtr(), &offset);
	deviceContext_->IASetIndexBuffer(indexBuffer_.get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	deviceContext_->DrawIndexed(indexBuffer_.indicesCount(), 0, 0);
}

void WorldChunkMesh::reset() {
	texture_ = nullptr;
	deviceContext_ = nullptr;
	vertexBuffer_.reset();
	indexBuffer_.reset();
}
