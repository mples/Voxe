#include "Plane.h"



Plane::Plane() {
}

Plane::Plane(glm::vec3& norm, float dist) : normal_(norm), distance_(dist) {
}


Plane::~Plane() {
}

void Plane::normalize() {
	float scale = 1 / normal_.length();
	normal_.x *= scale;
	normal_.y *= scale;
	normal_.z *= scale;
	distance_ *= scale;
}
