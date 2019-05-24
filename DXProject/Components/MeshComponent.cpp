#include "MeshComponent.h"
#include "../Utilities/ErrorLogger.h"

MeshComponent::MeshComponent(ID3D11Device * device, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) : offset_(0u) {
	setMesh(device, vertices, indices);
}

MeshComponent::~MeshComponent() {
}

VertexBuffer<Vertex>& MeshComponent::getVertexBuffer() {
	return vertexBuffer_;
}

IndexBuffer & MeshComponent::getIndexBuffer() {
	return indexBuffer_;
}

void MeshComponent::setMesh(ID3D11Device * device, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	try {
		HRESULT hr = vertexBuffer_.initialize(device, vertices.data(), vertices.size());
		COM_ERROR_IF_FAILED(hr, L"Failed to initialize vertex buffer.");

		hr = indexBuffer_.initialize(device, indices.data(), indices.size());
		COM_ERROR_IF_FAILED(hr, L"Failed to initialize index buffer.");
	}
	catch (COMException e) {
		ErrorLogger::log(e);
	}
}

void MeshComponent::clearMesh() {
	vertexBuffer_.reset();
	indexBuffer_.reset();
}
