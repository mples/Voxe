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
private:
};

