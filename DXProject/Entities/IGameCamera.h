#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;
class IGameCamera {
public:
	virtual ~IGameCamera() {};

	virtual XMMATRIX & getViewMatrix() = 0;

	virtual XMMATRIX & getProjectionMatrix() = 0;

	virtual BoundingFrustum & getLocalSpaceFrustum() = 0;

	virtual BoundingFrustum & getWorldSpaceFrustum() = 0;

	virtual void setProjData(float fov_degrees, float aspect_ratio, float near_plane, float far_plane) = 0;

};
