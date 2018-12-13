#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), float fov = 45.0f);
	
	void setPosition(glm::vec3& pos);
	void setRotation(glm::vec3& rot);

	glm::vec3 & getPosition();
	glm::vec3 & getRotation();

	glm::mat4 & getViewMatrix();
	glm::mat4 & getProjMatrix();
private:
	const glm::vec3 UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 position_;
	glm::vec3 rotation_;
	glm::vec3 frontVec_;
	glm::vec3 upVec_;
	glm::vec3 rightVec_;

	glm::mat4 projectionMatrix_;
	glm::mat4 viewMatrix_;

	float fieldOfView_;
	void makeProjMatrix();
	void makeViewMatrix();
};

