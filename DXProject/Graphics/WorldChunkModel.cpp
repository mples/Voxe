#include "WorldChunkModel.h"



WorldChunkModel::WorldChunkModel() {
}


WorldChunkModel::~WorldChunkModel() {
}

bool WorldChunkModel::initialize(ID3D11Device* device, ID3D11DeviceContext* device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, Texture * texture) {
	device_ = device;
	deviceContext_ = device_context;
	CBVertexShader_ = &cb_vertex_shader;

	loadData(vertices, indices, texture);
	return false;
}

void WorldChunkModel::loadData(std::vector<Vertex>& vertices, std::vector<DWORD>& indices, Texture * texture) {
	mesh_.initialize(device_, deviceContext_, vertices, indices, texture);
}

void WorldChunkModel::draw(DirectX::XMMATRIX model_matrix, DirectX::XMMATRIX view_proj_matrix) {
	CBVertexShader_->data_.modelMatrix_ = model_matrix;
	CBVertexShader_->data_.mvpMatrix_ = model_matrix * view_proj_matrix;
	CBVertexShader_->applyChanges();
	mesh_.draw();
}
