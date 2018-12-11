#include "CameraComponent.h"
#include <iostream>
#include "Object.h"
#include "GraphicEngine.h"

CameraComponent::CameraComponent(Object* parent) : Component(ComponentType::CameraComponent) {
	setParent(parent);
	if (!parent_) {
		std::cout << "Error: trying to activate camera component without setting parent object." << std::endl;
	}

	camera_.setPosition(parent_->getPosition());
	GraphicEngine::getInstance().setActiveCamera(&camera_);
}


CameraComponent::~CameraComponent() {
}

void CameraComponent::activate() {
	/*
	if (!parent_) {
		std::cout << "Error: trying to activate camera component without setting parent object." << std::endl;
	}

	camera_.setPosition(parent_->getPosition());
	GraphicEngine::getInstance().setActiveCamera(&camera_);*/
}

void CameraComponent::update(float dt) {
	camera_.setPosition(parent_->getPosition());
}

