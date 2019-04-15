#include "WorldChunkModel.h"



WorldChunkModel::WorldChunkModel() {
}


WorldChunkModel::~WorldChunkModel() {
}

bool WorldChunkModel::initialize(ID3D11Device* device, ID3D11DeviceContext* device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, Texture * texture) {
	device_ = device;
	deviceContext_ = device_context;
	CBVertexShader_ = &cb_vertex_shader;
	texture_ = texture;
	mesh_.initialize(device_, deviceContext_, vertices, indices, texture_);	
	boundingBox_.CreateFromPoints(boundingBox_, vertices.size(), &(vertices.data()->pos_), sizeof(Vertex));
	return false;
}

void WorldChunkModel::draw(DirectX::XMMATRIX model_matrix, DirectX::XMMATRIX view_proj_matrix) {

	deviceContext_->VSSetConstantBuffers(0, 1, CBVertexShader_->getAddressOf());
	CBVertexShader_->data_.modelMatrix_ = model_matrix;
	CBVertexShader_->data_.mvpMatrix_ = model_matrix * view_proj_matrix;
	CBVertexShader_->applyChanges();
	mesh_.draw();
}

void WorldChunkModel::loadData(std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	mesh_.initialize(device_, deviceContext_, vertices, indices, texture_);
}

BoundingBox & WorldChunkModel::getBoundingBox() {
	return boundingBox_;
}

void WorldChunkModel::reset() {
	device_ = nullptr;
	deviceContext_ = nullptr;
	CBVertexShader_= nullptr;
	texture_ = nullptr;
	mesh_.reset();
}
