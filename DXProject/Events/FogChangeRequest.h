#pragma once
#include <DirectXMath.h>
#include "../ECS/Event/Event.h"

struct FogChangeRequest : public Event<FogChangeRequest> {
	FogChangeRequest(DirectX::XMFLOAT3 color, float start, float end) : fogColor_(color), fogStart_(start), fogEnd_(end) {}

	DirectX::XMFLOAT3 fogColor_;
	float fogStart_;
	float fogEnd_;
};