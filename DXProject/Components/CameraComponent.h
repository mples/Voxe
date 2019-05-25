#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "../ECS/Component.h"
#include "../Components/TransformComponent.h"

using namespace DirectX;

class CameraComponent : public TransformComponent {
public:
	CameraComponent();
	CameraComponent(float fov_degrees, float aspect_ratio, float near_plane, float far_plane, XMFLOAT3 look_at, XMFLOAT3 pos, XMFLOAT3 rot);
	~CameraComponent();

	virtual XMMATRIX & getViewMatrix();
	virtual XMMATRIX & getProjectionMatrix();

	BoundingFrustum & getLocalSpaceFrustum();
	BoundingFrustum & getWorldSpaceFrustum();

	void setProjData(float fov_degrees, float aspect_ratio, float near_plane, float far_plane);

	void setLookAt(XMFLOAT3 look_at);
private:
	void updateMatrix() override;
	void updateFrustum();

	XMMATRIX viewMatrix_;
	XMMATRIX projMatrix_;
	BoundingFrustum frustumLocalSpace_;
	BoundingFrustum frustumWorldSpace_;
};