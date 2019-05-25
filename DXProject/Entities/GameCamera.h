#pragma once
#include <DirectXMath.h>

#include "../ECS/Entity.h"
#include "../Components/CameraComponent.h"

#include "IGameCamera.h"

#include "../Input/InputManager.h"


class GameCamera : public Entity<GameCamera>, public IGameCamera {
public:
	GameCamera();
	GameCamera(float fov_degrees, float aspect_ratio, float near_plane, float far_plane, XMFLOAT3 look_at, XMFLOAT3 pos, XMFLOAT3 rot);
	~GameCamera();

	// Inherited via IGameCamera
	virtual XMMATRIX & getViewMatrix() override;
	virtual XMMATRIX & getProjectionMatrix() override;
	virtual BoundingFrustum & getLocalSpaceFrustum() override;
	virtual BoundingFrustum & getWorldSpaceFrustum() override;
private:
	CameraComponent * cameraComponent_;

	static XMMATRIX IDENTITY_MATRIX;

	static BoundingFrustum NOT_INITIALIZED_FRUSTUM;

	//TODO refector that
	InputContext context_;

	// Inherited via IGameCamera
};


