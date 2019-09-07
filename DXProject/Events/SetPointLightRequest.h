#pragma once

#include <DirectXMath.h>
#include "../ECS/Event/Event.h"

struct SetPointLightRequest : public Event<SetPointLightRequest> {
	SetPointLightRequest(DirectX::XMFLOAT3 color, float a_strg, float diff_strg, float spec_strg, DirectX::XMFLOAT3 pos, float range) : 
		color_(color), ambStrenght_(a_strg), diffStrenght_(diff_strg), specStrenght_(spec_strg), position_(pos), range_(range) {}

	DirectX::XMFLOAT3 color_;
	float ambStrenght_;
	float diffStrenght_;
	float specStrenght_;

	DirectX::XMFLOAT3 position_;
	float range_;
};