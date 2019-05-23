#include "GameCamera.h"
#include "../Engine.h"
#include "../Events/CameraCreated.h"
#include "../Events/CameraDestroyed.h"

GameCamera::GameCamera() : cameraComponent_(nullptr) {
	//addComponent<CameraComponent>();
	//ENGINE.sendEvent<CameraCreated>(id_);
	identity_ = XMMatrixIdentity();
}

GameCamera::GameCamera(float fov_degrees, float aspect_ratio, float near_plane, float far_plane, XMFLOAT3 look_at, XMFLOAT3 pos, XMFLOAT3 rot) : cameraComponent_(nullptr) {
	//addComponent<CameraComponent>(fov_degrees, aspect_ratio, near_plane, far_plane, look_at, pos, rot);
	//ENGINE.sendEvent<CameraCreated>(id_);
	identity_ = XMMatrixIdentity();
}

GameCamera::~GameCamera() {
	ENGINE.sendEvent<CameraDestroyed>(id_);
}

XMMATRIX & GameCamera::getViewMatrix() {
	if (cameraComponent_ != nullptr) {
		return cameraComponent_->getViewMatrix();
	}
	else {
		return identity_; //TODO remove
		assert(0 && "Camera Component not assigned");
	}
}

XMMATRIX & GameCamera::getProjectionMatrix() {
	if (cameraComponent_ != nullptr) {
		return cameraComponent_->getProjectionMatrix();
	}
	else {
		return identity_; //TODO remove
		assert(0 && "Camera Component not assigned");
	}
}
