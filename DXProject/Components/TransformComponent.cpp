#include "TransformComponent.h"

const XMVECTOR TransformComponent::DEFAULT_FRONT_VEC = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
const XMVECTOR TransformComponent::DEFAULT_UP_VEC = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const XMVECTOR TransformComponent::DEFAULT_BACKWARD_VEC = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
const XMVECTOR TransformComponent::DEFAULT_RIGHT_VEC = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const XMVECTOR TransformComponent::DEFAULT_LEFT_VEC = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);


TransformComponent::TransformComponent() {
	setPos(0.0f, 0.0f, 0.0f);
	setRot(0.0f, 0.0f, 0.0f);
}

TransformComponent::TransformComponent(float x, float y, float z, float pitch, float yaw, float roll) {
	setPos(x, y, z);
	setRot(pitch, yaw, roll);
}

TransformComponent::TransformComponent(XMFLOAT3 pos, XMFLOAT3 rot) {
	setPos(pos);
	setRot(rot);
	
}

TransformComponent::~TransformComponent() {
}

XMMATRIX & TransformComponent::getWorldMatrix() {
	return worldMatrix_;
}


void TransformComponent::updateMatrix() {
	XMMATRIX vector_rotation = XMMatrixRotationRollPitchYaw(rot_.x, rot_.y, 0.0f);
	forwardVector_ = XMVector3TransformCoord(DEFAULT_FRONT_VEC, vector_rotation);
	backwardVector_ = XMVector3TransformCoord(DEFAULT_BACKWARD_VEC, vector_rotation);
	rightVector_ = XMVector3TransformCoord(DEFAULT_RIGHT_VEC, vector_rotation);
	leftVector_ = XMVector3TransformCoord(DEFAULT_LEFT_VEC, vector_rotation);

	worldMatrix_ = XMMatrixRotationRollPitchYaw(rot_.x, rot_.y, rot_.z) * XMMatrixTranslation(pos_.x, pos_.y, pos_.z);
}


void TransformComponent::setPos(const XMVECTOR & pos) {
	XMStoreFloat3(&pos_, pos);
	posVector_ = pos;
	updateMatrix();
}

void TransformComponent::setPos(const XMFLOAT3 & pos) {
	pos_ = pos;
	posVector_ = XMLoadFloat3(&pos_);
	updateMatrix();
}

void TransformComponent::setPos(float x, float y, float z) {
	pos_ = XMFLOAT3(x, y, z);
	posVector_ = XMLoadFloat3(&pos_);
	updateMatrix();
}

void TransformComponent::adjustPos(const XMVECTOR & pos) {
	posVector_ += pos;
	XMStoreFloat3(&pos_, posVector_);
	updateMatrix();
}

void TransformComponent::adjustPos(const XMFLOAT3 & pos) {
	pos_.x += pos.x;
	pos_.y += pos.y;
	pos_.z += pos.z;
	posVector_ = XMLoadFloat3(&pos_);
	updateMatrix();
}

void TransformComponent::adjustPos(float x, float y, float z) {
	pos_.x += x;
	pos_.y += y;
	pos_.z += z;
	posVector_ = XMLoadFloat3(&pos_);
	updateMatrix();
}

void TransformComponent::setRot(const XMVECTOR & rot) {
	XMStoreFloat3(&rot_, rot);
	rotVector_ = rot;
	updateMatrix();
}

void TransformComponent::setRot(const XMFLOAT3 & rot) {
	rot_ = rot;
	rotVector_ = XMLoadFloat3(&rot_);
	updateMatrix();
}

void TransformComponent::setRot(float x, float y, float z) {
	rot_ = XMFLOAT3(x, y, z);
	rotVector_ = XMLoadFloat3(&rot_);
	updateMatrix();
}

void TransformComponent::adjustRot(const XMVECTOR & rot) {
	rotVector_ += rot;
	XMStoreFloat3(&rot_, rotVector_);
	updateMatrix();
}

void TransformComponent::adjustRot(const XMFLOAT3 & rot) {
	rot_.x += rot.x;
	rot_.y += rot.y;
	rot_.z += rot.z;
	rotVector_ = XMLoadFloat3(&rot_);
	updateMatrix();
}

void TransformComponent::adjustRot(float x, float y, float z) {
	rot_.x += x;
	rot_.y += y;
	rot_.z += z;
	rotVector_ = XMLoadFloat3(&rot_);
	updateMatrix();
}

const XMFLOAT3 & TransformComponent::getPos() {
	return pos_;
}

const XMVECTOR & TransformComponent::getPosVector() {
	return posVector_;
}

const XMFLOAT3 & TransformComponent::getRot() {
	return rot_;
}

const XMVECTOR & TransformComponent::getRotVector() {
	return rotVector_;
}

const XMVECTOR & TransformComponent::getForwardVector() {
	return forwardVector_;
}

const XMVECTOR & TransformComponent::getBackwardVector() {
	return backwardVector_;
}

const XMVECTOR & TransformComponent::getLeftVector() {
	return leftVector_;
}

const XMVECTOR & TransformComponent::getRightVector() {
	return rightVector_;
}