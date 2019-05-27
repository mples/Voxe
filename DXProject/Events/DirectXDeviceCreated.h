#pragma once
#include "../ECS/Event/Event.h"
#include <d3d11.h>

struct DirectXDeviceCreated : public Event<DirectXDeviceCreated> {
	DirectXDeviceCreated(ID3D11Device * device, ID3D11DeviceContext * device_context) : device_(device), deviceContext_(device_context) {}

	ID3D11Device * device_;
	ID3D11DeviceContext * deviceContext_;
};