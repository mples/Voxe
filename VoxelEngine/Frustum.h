#pragma once
#include "Plane.h"
#include <vector>
#include <glm/ext.hpp>

class Frustum {
public:
	enum {
		NEAR_PLANE = 0,
		FAR_PLANE = 1,
		LEFT_PLANE = 2,
		RIGHT_PLANE = 3,
		DOWN_PLANE = 4,
		UP_PLANE = 5
	};
	static const int PLANE_COUNT = 6;
	Frustum();
	Frustum(glm::mat4 mat);
	~Frustum();
	void update(glm::mat4 mat);
	bool sphereIntersect(glm::vec3 center, float radius);
	bool cubeIntersect(glm::vec3 center, float x, float y, float z);
	bool cubeIntersect(glm::ivec3 center, int dim);
private:
	std::vector<Plane> planes_;

};

