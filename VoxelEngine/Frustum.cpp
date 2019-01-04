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
	
	//mat = glm::transpose(mat);
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
	//planes_.clear();
	//planes_.reserve(PLANE_COUNT);
	//mat = glm::transpose(mat);
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

bool Frustum::sphereIntersect(glm::vec3 center, float radius) {
	for (Plane p : planes_) {
		if (glm::dot(center, p.normal_) + p.distance_ + radius <= 0)
			return false;
	}
	return true;
}

bool Frustum::cubeIntersect(glm::vec3 center, float x, float y, float z) {
	int inside_count = 0;
	for (Plane p : planes_) {
		int outside_count = 0;
		float a = glm::dot(center + glm::vec3(x, y, z), p.normal_);
		if (glm::dot(center + glm::vec3(x, y, z), p.normal_) + p.distance_ <= 0)
			++outside_count;
		if (glm::dot(center + glm::vec3(x, -y, z), p.normal_) + p.distance_ <= 0)
			++outside_count;
		if (glm::dot(center + glm::vec3(x, y, -z), p.normal_) + p.distance_ <= 0)
			++outside_count;
		if (glm::dot(center + glm::vec3(x, -y, -z), p.normal_) + p.distance_ <= 0)
			++outside_count;
		if (glm::dot(center + glm::vec3(-x, y, z), p.normal_) + p.distance_ <= 0)
			++outside_count;
		if (glm::dot(center + glm::vec3(-x, -y, z), p.normal_) + p.distance_ <= 0)
			++outside_count;
		if (glm::dot(center + glm::vec3(-x, y, -z), p.normal_) + p.distance_ <= 0)
			++outside_count;
		if (glm::dot(center + glm::vec3(-x, -y, -z), p.normal_) + p.distance_ <= 0)
			++outside_count;

		if (outside_count < 8)
			++inside_count;
		else if (outside_count == 8)
			int a = 0 + 34 * 2;
	}
	if (inside_count == 0)
		int b = 0 + 34 * 5;
	return (inside_count > 0) ;
}

bool Frustum::cubeIntersect(glm::ivec3 center, int dim) {
	center = glm::ivec3(center.x + 8, center.y + 8, center.z + 8);
	//dim /= 2;
	/*if (sphereIntersect(center, 0.0f))
		return true;
	if (sphereIntersect(center + glm::ivec3(dim, 0, 0) , 0.0f))
		return true;
	if (sphereIntersect(center + glm::ivec3(dim, dim, 0), 0.0f))
		return true;
	if (sphereIntersect(center + glm::ivec3(dim, 0, dim), 0.0f))
		return true;
	if (sphereIntersect(center + glm::ivec3(dim, dim, dim), 0.0f))
		return true;
	if (sphereIntersect(center + glm::ivec3(0, dim, 0), 0.0f))
		return true;
	if (sphereIntersect(center + glm::ivec3(0, 0, dim), 0.0f))
		return true;
	if (sphereIntersect(center + glm::ivec3(0, dim, dim), 0.0f))
		return true;

	return false;*/
	if (sphereIntersect(center, 32))
		return true;
	return false;

}
