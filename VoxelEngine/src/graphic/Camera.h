#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();
private:
	glm::vec3 Position_;
	glm::vec3 LookAt_;
};

