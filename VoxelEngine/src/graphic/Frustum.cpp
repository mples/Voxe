#include "Frustum.h"


Frustum::Frustum() {
	for (int i = 0; i < PLANE_COUNT; ++i) {
		planes_.push_back(Plane());
	}
}

Frustum::Frustum(glm::mat4 mat) {
	for (int i = 0; i < PLANE_COUNT; ++i) {
		planes_.push_back(Plane());
	}

	planes_[RIGHT_PLANE].normal_.x = mat[0][3] - mat[0][0];
	planes_[RIGHT_PLANE].normal_.y = mat[1][3] - mat[1][0];
	planes_[RIGHT_PLANE].normal_.z = mat[2][3] - mat[2][0];
	planes_[RIGHT_PLANE].distance_ = mat[3][3] - mat[3][0];

	planes_[LEFT_PLANE].normal_.x = mat[0][3] + mat[0][0];
	planes_[LEFT_PLANE].normal_.y = mat[1][3] + mat[1][0];
	planes_[LEFT_PLANE].normal_.z = mat[2][3] + mat[2][0];
	planes_[LEFT_PLANE].distance_ = mat[3][3] + mat[3][0];

	planes_[UP_PLANE].normal_.x = mat[0][3] - mat[0][1];
	planes_[UP_PLANE].normal_.y = mat[1][3] - mat[1][1];
	planes_[UP_PLANE].normal_.z = mat[2][3] - mat[2][1];
	planes_[UP_PLANE].distance_ = mat[3][3] - mat[3][1];

	planes_[DOWN_PLANE].normal_.x = mat[0][3] + mat[0][1];
	planes_[DOWN_PLANE].normal_.y = mat[1][3] + mat[1][1];
	planes_[DOWN_PLANE].normal_.z = mat[2][3] + mat[2][1];
	planes_[DOWN_PLANE].distance_ = mat[3][3] + mat[3][1];

	planes_[FAR_PLANE].normal_.x = mat[0][3] - mat[0][2];
	planes_[FAR_PLANE].normal_.y = mat[1][3] - mat[1][2];
	planes_[FAR_PLANE].normal_.z = mat[2][3] - mat[2][2];
	planes_[FAR_PLANE].distance_ = mat[3][3] - mat[3][2];

	planes_[NEAR_PLANE].normal_.x = mat[0][3] + mat[0][2];
	planes_[NEAR_PLANE].normal_.y = mat[1][3] + mat[1][2];
	planes_[NEAR_PLANE].normal_.z = mat[2][3] + mat[2][2];
	planes_[NEAR_PLANE].distance_ = mat[3][3] + mat[3][2];

	for (Plane p : planes_) {
		p.normalize();
	}
}


Frustum::~Frustum() {

}

void Frustum::update(glm::mat4 mat) {
	planes_[RIGHT_PLANE].normal_.x = mat[0][3] - mat[0][0];
	planes_[RIGHT_PLANE].normal_.y = mat[1][3] - mat[1][0];
	planes_[RIGHT_PLANE].normal_.z = mat[2][3] - mat[2][0];
	planes_[RIGHT_PLANE].distance_ = mat[3][3] - mat[3][0];

	planes_[LEFT_PLANE].normal_.x = mat[0][3] + mat[0][0];
	planes_[LEFT_PLANE].normal_.y = mat[1][3] + mat[1][0];
	planes_[LEFT_PLANE].normal_.z = mat[2][3] + mat[2][0];
	planes_[LEFT_PLANE].distance_ = mat[3][3] + mat[3][0];

	planes_[UP_PLANE].normal_.x = mat[0][3] - mat[0][1];
	planes_[UP_PLANE].normal_.y = mat[1][3] - mat[1][1];
	planes_[UP_PLANE].normal_.z = mat[2][3] - mat[2][1];
	planes_[UP_PLANE].distance_ = mat[3][3] - mat[3][1];

	planes_[DOWN_PLANE].normal_.x = mat[0][3] + mat[0][1];
	planes_[DOWN_PLANE].normal_.y = mat[1][3] + mat[1][1];
	planes_[DOWN_PLANE].normal_.z = mat[2][3] + mat[2][1];
	planes_[DOWN_PLANE].distance_ = mat[3][3] + mat[3][1];

	planes_[FAR_PLANE].normal_.x = mat[0][3] - mat[0][2];
	planes_[FAR_PLANE].normal_.y = mat[1][3] - mat[1][2];
	planes_[FAR_PLANE].normal_.z = mat[2][3] - mat[2][2];
	planes_[FAR_PLANE].distance_ = mat[3][3] - mat[3][2];

	planes_[NEAR_PLANE].normal_.x = mat[0][3] + mat[0][2];
	planes_[NEAR_PLANE].normal_.y = mat[1][3] + mat[1][2];
	planes_[NEAR_PLANE].normal_.z = mat[2][3] + mat[2][2];
	planes_[NEAR_PLANE].distance_ = mat[3][3] + mat[3][2];

	for (Plane p : planes_) {
		p.normalize();
	}
}

bool Frustum::pointIntersect(glm::vec3 point) {
	for (Plane p : planes_) {
		if (glm::dot(point, p.normal_) + p.distance_  <= 0)
			return false;
	}
	return true;
}

bool Frustum::sphereIntersect(glm::vec3 center, float radius) {
	for (Plane p : planes_) {
		if (glm::dot(center, p.normal_) + p.distance_ + radius <= 0)
			return false;
	}
	return true;
}

bool Frustum::cubeIntersect(glm::vec3 center, float x, float y, float z) {
	if (pointIntersect(center + glm::vec3(x, y, z)))
		return true;
	if (pointIntersect(center + glm::vec3(x, -y, z)))
		return true;
	if (pointIntersect(center + glm::vec3(x, y, -z)))
		return true;
	if (pointIntersect(center + glm::vec3(x, -y, -z)))
		return true;
	if (pointIntersect(center + glm::vec3(-x, y, z)))
		return true;
	if (pointIntersect(center + glm::vec3(-x, -y, z)))
		return true;
	if (pointIntersect(center + glm::vec3(-x, y, -z)))
		return true;
	if (pointIntersect(center + glm::vec3(-x, -y, -z)))
		return true;

	return false;
}
