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
}

void CameraComponent::update(float dt) {
	std::cout << "Camera position " << camera_.getPosition().x << " " << camera_.getPosition().y << " " << camera_.getPosition().z << std::endl;
	std::cout << "Camera roattion " << camera_.getRotation().x << " " << camera_.getRotation().y << " " << camera_.getRotation().z << std::endl;

	std::cout << "Player position " << parent_->getPosition().x << " " << parent_->getPosition().y << " " << parent_->getPosition().z << std::endl;
	std::cout << "Plater roattion " << parent_->getRotation().x << " " << parent_->getRotation().y << " " << parent_->getRotation().z << std::endl;

	camera_.setPosition(parent_->getPosition());
	camera_.setRotation(parent_->getRotation());
}

