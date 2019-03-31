#include "RenderableObject.h"



RenderableObject::RenderableObject() {
}


RenderableObject::~RenderableObject() {
}

bool RenderableObject::initialize(const std::string & file_path, ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader) {
	if (!model_.initialize(file_path, device, device_context, cb_vertex_shader)) {
		return false;
	}

	setPos(0.0f, 0.0f, 0.0f);
	setRot(0.0f, 0.0f, 0.0f);
	updateMatrix();
	return true;
}


void RenderableObject::draw(const XMMATRIX & view_proj_matrix) {
	model_.draw(modelMatrix_, view_proj_matrix);
}

void RenderableObject::updateMatrix() {
	GameObject::updateMatrix();
	modelMatrix_ = XMMatrixRotationRollPitchYaw(rot_.x, rot_.y, rot_.z) * XMMatrixTranslation(pos_.x, pos_.y, pos_.z);
}