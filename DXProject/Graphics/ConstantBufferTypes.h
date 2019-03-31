#pragma once
#include <DirectXMath.h>

struct CB_VS_object_buffer {
	DirectX::XMMATRIX mvpMatrix_;
	DirectX::XMMATRIX modelMatrix_;
};

struct CB_PS_point_light{
	DirectX::XMFLOAT3 ambientColor_;
	float ambientStrength_;

	DirectX::XMFLOAT3 diffuseColor_;
	float diffuseStrength_;

	DirectX::XMFLOAT3 specularColor_;
	float specularStrength_;

	DirectX::XMFLOAT3 pos_;
	float range_;

	float lightAttenuationA_;
	float lightAttenuationB_;
	float lightAttenuationC_;
	float pad_;
};

struct CB_PS_frame_buffer {
	DirectX::XMFLOAT3 eyePos_;
};