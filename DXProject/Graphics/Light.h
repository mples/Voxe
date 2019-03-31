#pragma once
#include "RenderableObject.h"

class Light : public RenderableObject {
public:
	Light();
	~Light();

	bool initialize(ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader);

	DirectX::XMFLOAT3 color_;
	float strength_;
	float attenuationA_;
	float attenuationB_;
	float attenuationC_;
};

