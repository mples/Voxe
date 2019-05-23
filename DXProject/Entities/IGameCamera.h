#pragma once
#include <DirectXMath.h>

using namespace DirectX;
class IGameCamera {
public:
	virtual ~IGameCamera() {};

	virtual XMMATRIX & getViewMatrix() = 0;

	virtual XMMATRIX & getProjectionMatrix() = 0;

};
