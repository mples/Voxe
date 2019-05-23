#pragma once
#include "../ECS/Event/Event.h"
#include <d3d11.h>

struct DirectXDeviceCreated : public Event<DirectXDeviceCreated> {
	DirectXDeviceCreated(ID3D11Device * device) : device_(device) {}

	ID3D11Device * device_;
};