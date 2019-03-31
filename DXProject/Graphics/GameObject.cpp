#include "GameObject.h"


GameObject::GameObject() {
	setPos(0.0f, 0.0f, 0.0f);
	setRot(0.0f, 0.0f, 0.0f);
}

GameObject::~GameObject() {
}

void GameObject::updateMatrix() {
	XMMATRIX vector_rotation = XMMatrixRotationRollPitchYaw(rot_.x, rot_.y, 0.0f);
	forwardVector_ = XMVector3TransformCoord(DEFAULT_FRONT_VEC, vector_rotation);
	backwardVector_ = XMVector3TransformCoord(DEFAULT_BACKWARD_VEC, vector_rotation);
	rightVector_ = XMVector3TransformCoord(DEFAULT_RIGHT_VEC, vector_rotation);
	leftVector_ = XMVector3TransformCoord(DEFAULT_LEFT_VEC, vector_rotation);
}


void GameObject::setPos(const XMVECTOR & pos) {
	XMStoreFloat3(&pos_, pos);
	posVector_ = pos;
	updateMatrix();
}

void GameObject::setPos(const XMFLOAT3 & pos) {
	pos_ = pos;
	posVector_ = XMLoadFloat3(&pos_);
	updateMatrix();
}

void GameObject::setPos(float x, float y, float z) {
	pos_ = XMFLOAT3(x, y, z);
	posVector_ = XMLoadFloat3(&pos_);
	updateMatrix();
}

void GameObject::adjustPos(const XMVECTOR & pos) {
	posVector_ += pos;
	XMStoreFloat3(&pos_, posVector_);
	updateMatrix();
}

void GameObject::adjustPos(const XMFLOAT3 & pos) {
	pos_.x += pos.x;
	pos_.y += pos.y;
	pos_.z += pos.z;
	posVector_ = XMLoadFloat3(&pos_);
	updateMatrix();
}

void GameObject::adjustPos(float x, float y, float z) {
	pos_.x += x;
	pos_.y += y;
	pos_.z += z;
	posVector_ = XMLoadFloat3(&pos_);
	updateMatrix();
}

void GameObject::setRot(const XMVECTOR & rot) {
	XMStoreFloat3(&rot_, rot);
	rotVector_ = rot;
	updateMatrix();
}

void GameObject::setRot(const XMFLOAT3 & rot) {
	rot_ = rot;
	rotVector_ = XMLoadFloat3(&rot_);
	updateMatrix();
}

void GameObject::setRot(float x, float y, float z) {
	rot_ = XMFLOAT3(x, y, z);
	rotVector_ = XMLoadFloat3(&rot_);
	updateMatrix();
}

void GameObject::adjustRot(const XMVECTOR & rot) {
	rotVector_ += rot;
	XMStoreFloat3(&rot_, rotVector_);
	updateMatrix();
}

void GameObject::adjustRot(const XMFLOAT3 & rot) {
	rot_.x += rot.x;
	rot_.y += rot.y;
	rot_.z += rot.z;
	rotVector_ = XMLoadFloat3(&rot_);
	updateMatrix();
}

void GameObject::adjustRot(float x, float y, float z) {
	rot_.x += x;
	rot_.y += y;
	rot_.z += z;
	rotVector_ = XMLoadFloat3(&rot_);
	updateMatrix();
}

const XMFLOAT3 & GameObject::getPos() {
	return pos_;
}

const XMVECTOR & GameObject::getPosVector() {
	return posVector_;
}

const XMFLOAT3 & GameObject::getRot() {
	return rot_;
}

const XMVECTOR & GameObject::getRotVector() {
	return rotVector_;
}

const XMVECTOR & GameObject::getForwardVector() {
	return forwardVector_;
}

const XMVECTOR & GameObject::getBackwardVector() {
	return backwardVector_;
}

const XMVECTOR & GameObject::getLeftVector() {
	return leftVector_;
}

const XMVECTOR & GameObject::getRightVector() {
	return rightVector_;
}