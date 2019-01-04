#pragma once
#include "glm/glm.hpp"

class Plane {
public:
	Plane();
	Plane(glm::vec3& norm, float dist);
	~Plane();
	void normalize();

	glm::vec3 normal_;
	float distance_;
};

