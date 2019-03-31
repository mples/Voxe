#include "Camera.h"
#include "GraphicEngine.h"
#include <cmath>

Camera::Camera(glm::vec3 pos, glm::vec3 front, float fov) : position_(pos), frontVec_(front), fieldOfView_(fov), rotation_(glm::vec3 (0.0f, 90.0f, 0.0f)){
	makeProjMatrix();
	makeViewMatrix();
	frustum_.update(projectionMatrix_ * viewMatrix_);
}

void Camera::setPosition(glm::vec3& pos) {
	if (pos != position_) {
		position_ = pos;
		makeProjMatrix();
		makeViewMatrix();
		frustum_.update(projectionMatrix_ * viewMatrix_);
	}
}

void Camera::setRotation(glm::vec3& rot) {
	if (rot.x > 89.0) {
		rot.x = 89.0;
	}
	else if (rot.x < -89.0) {
		rot.x = -89.0;
	}
	
	if (rot.y < 0 || rot.y > 360) {
		rot.y = fmod(rot.y, 360.0f);
	}

	if (rotation_ != rot) {
		rotation_ = rot;
		makeProjMatrix();
		makeViewMatrix();
		frustum_.update(projectionMatrix_ * viewMatrix_);
	}
}

glm::vec3 & Camera::getPosition() {
	return position_;
}

glm::vec3 & Camera::getRotation() {
	return rotation_;
}

glm::vec3 & Camera::getFront() {
	return frontVec_;
}

glm::mat4 & Camera::getViewMatrix() {
	return viewMatrix_;
}

glm::mat4 & Camera::getProjMatrix() {
	return projectionMatrix_;
}

Frustum & Camera::getFrustum() {
	return frustum_;
}

void Camera::makeProjMatrix() {
	projectionMatrix_ = glm::perspective(glm::radians(fieldOfView_), (float)GraphicEngine::getInstance().getWindowWidth() / GraphicEngine::getInstance().getWindowHeight(), 0.1f, 100.0f);
}

void Camera::makeViewMatrix() {

	glm::vec3 front;
	front.x = cos(glm::radians(rotation_.y)) * cos(glm::radians(rotation_.x));
	front.y = sin(glm::radians(rotation_.x));
	front.z = sin(glm::radians(rotation_.y)) * cos(glm::radians(rotation_.x));
	frontVec_ = glm::normalize(front);
	rightVec_ = glm::normalize(glm::cross(frontVec_, UP_VECTOR) );
	upVec_ = glm::cross(rightVec_, frontVec_);

	viewMatrix_ = glm::lookAt(position_, position_ + frontVec_, upVec_);
}
