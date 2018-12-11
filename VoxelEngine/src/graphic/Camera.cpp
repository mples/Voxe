#include "Camera.h"
#include "GraphicEngine.h"


Camera::Camera() : position_(glm::vec3(0.0f, 0.0f, 0.0f)), lookAt_(glm::vec3(0.0f, 0.0f, 0.0f)), fieldOfView_(45.0f) {
	makeProjMatrix();
	makeViewMatrix();
}

Camera::Camera(glm::vec3 pos, glm::vec3 look_at, float fov) : position_(pos), lookAt_(look_at), fieldOfView_(fov){
	makeProjMatrix();
	makeViewMatrix();
}


Camera::~Camera()
{
}

void Camera::setPosition(glm::vec3 pos) {
	position_ = pos;
	makeProjMatrix();
	makeViewMatrix();
}

void Camera::setLookAt(glm::vec3 look_at) {
	lookAt_ = look_at;
	makeProjMatrix();
	makeViewMatrix();
}

glm::mat4 & Camera::getViewMatrix() {
	return viewMatrix_;
}

glm::mat4 & Camera::getProjMatrix() {
	return projectionMatrix_;
}

void Camera::makeProjMatrix() {
	projectionMatrix_ = glm::perspective(glm::radians(fieldOfView_), (float)GraphicEngine::getInstance().getWindowWidth() / GraphicEngine::getInstance().getWindowHeight(), 0.1f, 1000.0f);
}

void Camera::makeViewMatrix() {
	glm::vec3 camera_direction = glm::normalize(position_ - lookAt_);
	rightVec_ = glm::normalize(glm::cross(UP_VECTOR, camera_direction) );
	upVec_ = glm::cross(camera_direction, rightVec_);

	viewMatrix_ = glm::lookAt(position_, camera_direction, upVec_);
}
