#pragma once
#include "Object.h"
#include "components/InputComponent.h"
#include "components/CameraComponent.h"

class Player : public Object {
public:
	Player();
	~Player();

	virtual void update(float dt) override;
	void handleInput(MappedInput& input);
	virtual void setPosition(glm::vec3& pos) override;
	virtual void setRotation(glm::vec3& rot) override;

private:
	glm::vec3 frontVec_;
	glm::vec3 rightVec_;

	glm::vec3 velocity_;

	void updateVectors();
};

