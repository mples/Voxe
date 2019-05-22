#include "MeshComponent.h"
#include "../Utilities/ErrorLogger.h"

MeshComponent::MeshComponent(ID3D11Device * device, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) : offset_(0u) {
	HRESULT hr = vertexBuffer_.initialize(device, vertices.data(), vertices.size());
	COM_ERROR_IF_FAILED(hr, L"Failed to initialize vertex buffer.");

	hr = indexBuffer_.initialize(device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(hr, L"Failed to initialize index buffer.");
}

MeshComponent::~MeshComponent() {
}

VertexBuffer<Vertex>& MeshComponent::getVertexBuffer() {
	return vertexBuffer_;
}

IndexBuffer & MeshComponent::getIndexBuffer() {
	return indexBuffer_;
}
