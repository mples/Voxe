#pragma once
#include "Component.h"
#include "Camera.h"

class CameraComponent : public Component {
public:
	CameraComponent(Object* parent);
	~CameraComponent();

	// Inherited via Component
	virtual void update(float dt) override;

	// Inherited via Component
	virtual void activate() override;
private:
	Camera camera_;

};

