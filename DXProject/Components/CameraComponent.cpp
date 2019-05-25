#include "CameraComponent.h"

CameraComponent::CameraComponent() {
	viewMatrix_ = XMMatrixIdentity();
	projMatrix_ = XMMatrixIdentity();
}

CameraComponent::CameraComponent(float fov_degrees, float aspect_ratio, float near_plane, float far_plane, XMFLOAT3 look_at, XMFLOAT3 pos, XMFLOAT3 rot) : TransformComponent(pos, rot) {
	setProjData(fov_degrees, aspect_ratio, near_plane, far_plane);
	setLookAt(look_at);
}

CameraComponent::~CameraComponent() {
}

XMMATRIX & CameraComponent::getViewMatrix() {
	return viewMatrix_;
}

XMMATRIX & CameraComponent::getProjectionMatrix() {
	return projMatrix_;
}

BoundingFrustum & CameraComponent::getLocalSpaceFrustum() {
	return frustumLocalSpace_;
}

BoundingFrustum & CameraComponent::getWorldSpaceFrustum() {
	return frustumWorldSpace_;
}

void CameraComponent::setProjData(float fov_degrees, float aspect_ratio, float near_plane, float far_plane) {
	projMatrix_ = XMMatrixPerspectiveFovLH((fov_degrees / 360.0f) * XM_2PI, aspect_ratio, near_plane, far_plane);
	frustumLocalSpace_.CreateFromMatrix(frustumLocalSpace_, projMatrix_);
}

void CameraComponent::setLookAt(XMFLOAT3 look_at) {
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

void CameraComponent::updateMatrix() {
	XMMATRIX camera_rot = XMMatrixRotationRollPitchYaw(rot_.x, rot_.y, rot_.z);
	XMVECTOR target_vector = XMVector3TransformCoord(DEFAULT_FRONT_VEC, camera_rot);
	target_vector += posVector_;
	XMVECTOR up_directory = XMVector3TransformCoord(DEFAULT_UP_VEC, camera_rot);
	viewMatrix_ = XMMatrixLookAtLH(posVector_, target_vector, up_directory);

	updateFrustum();
	TransformComponent::updateMatrix();
}

void CameraComponent::updateFrustum() {
	XMVECTOR det = XMMatrixDeterminant(viewMatrix_);
	XMMATRIX inv_view = XMMatrixInverse(&det, viewMatrix_);

	XMVECTOR scale;
	XMVECTOR rotation;
	XMVECTOR translation;

	XMMatrixDecompose(&scale, &rotation, &translation, inv_view);
	frustumLocalSpace_.Transform(frustumWorldSpace_, XMVectorGetX(scale), rotation, translation);
}
