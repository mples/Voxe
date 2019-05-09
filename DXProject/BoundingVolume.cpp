#include "BoundingVolume.h"
#include "Utilities/ErrorLogger.h"
#include "Voxel/Chunk.h"
BoundingVolume::BoundingVolume() : coord_(0.0f, 0.0f, 0.0f), boundingBox_() {
}

BoundingVolume::BoundingVolume(XMINT3 coord, BoundingBox bounding_box) : coord_(coord), boundingBox_(bounding_box) {
}

BoundingVolume::BoundingVolume(int x, int y, int z, BoundingBox bounding_box) : coord_(x,y,z), boundingBox_(bounding_box) {
}

BoundingVolume::~BoundingVolume() {
}

bool BoundingVolume::initialize(ID3D11Device * device, ID3D11DeviceContext * device_context) {
	deviceContext_ = device_context;

	std::vector<SimpleVertex> vertices;
	std::vector<DWORD> indices;
	
	vertices.push_back(SimpleVertex(XMFLOAT3(boundingBox_.Center.x - boundingBox_.Extents.x,
												boundingBox_.Center.y - boundingBox_.Extents.y,
												boundingBox_.Center.z - boundingBox_.Extents.z) ));
	vertices.push_back(SimpleVertex(XMFLOAT3(boundingBox_.Center.x - boundingBox_.Extents.x,
												boundingBox_.Center.y + boundingBox_.Extents.y,
												boundingBox_.Center.z - boundingBox_.Extents.z)));
	vertices.push_back(SimpleVertex(XMFLOAT3(boundingBox_.Center.x + boundingBox_.Extents.x,
												boundingBox_.Center.y + boundingBox_.Extents.y,
												boundingBox_.Center.z - boundingBox_.Extents.z)));
	vertices.push_back(SimpleVertex(XMFLOAT3(boundingBox_.Center.x + boundingBox_.Extents.x,
												boundingBox_.Center.y - boundingBox_.Extents.y,
												boundingBox_.Center.z - boundingBox_.Extents.z)));
	vertices.push_back(SimpleVertex(XMFLOAT3(boundingBox_.Center.x - boundingBox_.Extents.x,
												boundingBox_.Center.y - boundingBox_.Extents.y,
												boundingBox_.Center.z + boundingBox_.Extents.z)));
	vertices.push_back(SimpleVertex(XMFLOAT3(boundingBox_.Center.x - boundingBox_.Extents.x,
												boundingBox_.Center.y + boundingBox_.Extents.y,
												boundingBox_.Center.z + boundingBox_.Extents.z)));
	vertices.push_back(SimpleVertex(XMFLOAT3(boundingBox_.Center.x + boundingBox_.Extents.x,
												boundingBox_.Center.y + boundingBox_.Extents.y,
												boundingBox_.Center.z + boundingBox_.Extents.z)));
	vertices.push_back(SimpleVertex(XMFLOAT3(boundingBox_.Center.x + boundingBox_.Extents.x,
												boundingBox_.Center.y - boundingBox_.Extents.y,
												boundingBox_.Center.z + boundingBox_.Extents.z)));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(5);

	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(6);

	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(1);
	indices.push_back(6);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(7);

	indices.push_back(0);
	indices.push_back(7);
	indices.push_back(4);

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(5);

	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(1);

	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(6);

	indices.push_back(3);
	indices.push_back(6);
	indices.push_back(7);
	
	HRESULT hr = vertexBuffer_.initialize(device, vertices.data(), vertices.size());
	COM_ERROR_IF_FAILED(hr, L"Failed to initizlize vertex buffer if bounding volume");

	hr = indexBuffer_.initialize(device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(hr, L"Failed to initizlize vertex buffer if bounding volume");

	return true;

}

void BoundingVolume::draw() {
	UINT offset = 0;

	deviceContext_->IASetVertexBuffers(0, 1, vertexBuffer_.getAddressOf(), vertexBuffer_.stridePtr(), &offset);
	deviceContext_->IASetIndexBuffer(indexBuffer_.get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	deviceContext_->DrawIndexed(indexBuffer_.indicesCount(), 0, 0);
}

BoundingBox& BoundingVolume::getBoundingVolume() {
	return boundingBox_;
}

XMINT3 & BoundingVolume::getCoord() {
	return coord_;
}

XMFLOAT3 BoundingVolume::getPos() {
	return boundingBox_.Center;
}
