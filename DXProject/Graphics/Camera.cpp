#include "Camera.h"



Camera::Camera() {
	pos_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
	posVector_ = XMLoadFloat3(&pos_);
	rot_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotVector_ = XMLoadFloat3(&rot_);
	updateMatrix();
}


Camera::~Camera() {
}

const XMMATRIX & Camera::getViewMatrix() const {
	return viewMatrix_;
}

const XMMATRIX & Camera::getProjMatrix() const {
	return projMatrix_;
}

const XMMATRIX Camera::getViewProjMatrix() {
	return viewMatrix_ * projMatrix_;
}

void Camera::setProjData(float fov_degrees, float aspect_ratio, float near_plane, float far_plane) {
	projMatrix_ = XMMatrixPerspectiveFovLH((fov_degrees / 360.0f) * XM_2PI, aspect_ratio, near_plane, far_plane);
}

void Camera::setLookAt(XMFLOAT3 look_at) {
	if (look_at.x == pos_.x && look_at.y == pos_.y && look_at.z == pos_.z) {
		return;
	}
	
	look_at.x = pos_.x - look_at.x;
	look_at.y = pos_.y - look_at.y;
	look_at.z = pos_.z - look_at.z;

	float pitch = 0.0f;
	if (look_at.y != 0.0f) {
		const float distance = sqrt(look_at.x * look_at.x + look_at.z * look_at.z);
		pitch = atan(look_at.y / distance);
	}

	float yaw = 0.0f;
	if (look_at.x != 0.0f) {
		yaw = atan(look_at.x / look_at.z);
	}
	if (look_at.z > 0.0f) {
		yaw += XM_PI;
	}

	setRot(pitch, yaw, 0.0f);
}

void Camera::updateMatrix() {
	XMMATRIX camera_rot = XMMatrixRotationRollPitchYaw(rot_.x, rot_.y, rot_.z);
	XMVECTOR target_vector = XMVector3TransformCoord(DEFAULT_FRONT_VEC, camera_rot);
	target_vector += posVector_;
	XMVECTOR up_directory = XMVector3TransformCoord(DEFAULT_UP_VEC, camera_rot);
	viewMatrix_ = XMMatrixLookAtLH(posVector_, target_vector, up_directory);

	GameObject::updateMatrix();
	/*XMMATRIX vector_rot = XMMatrixRotationRollPitchYaw(0.0f, rot_.y, 0.0f);
	forwardVector_ = XMVector3TransformCoord(DEFAULT_FRONT_VEC, vector_rot);
	backwardVector_ = XMVector3TransformCoord(DEFAULT_BACKWARD_VEC, vector_rot);
	leftVector_ = XMVector3TransformCoord(DEFAULT_LEFT_VEC, vector_rot);
	rightVector_ = XMVector3TransformCoord(DEFAULT_RIGHT_VEC, vector_rot);*/
}
