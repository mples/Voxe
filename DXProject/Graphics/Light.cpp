#include "Light.h"



Light::Light() : color_(1.0f, 1.0f, 1.0f), strength_(1.0f), attenuationA_(1.0f), attenuationB_(0.1f), attenuationC_(0.1f) {
}


Light::~Light() {
}

bool Light::initialize(ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader) {
	if (!model_.initialize("Data/Objects/light.fbx", device, device_context, cb_vertex_shader)) {
		return false;
	}

	setPos(0.0f, 0.0f, 0.0f);
	setRot(0.0f, 0.0f, 0.0f);
	RenderableObject::updateMatrix();
	return true;
}
