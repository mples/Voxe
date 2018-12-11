#include "Object.h"
#include "Component.h"
#include <algorithm>
#include <iostream>

int Object::objectIdCount_ = 0;

Object::Object() : id_(++objectIdCount_), position_(0,0,0), components_() {
	components_.reserve(START_COMPVEC_SIZE);
}


Object::~Object() {
	removeAllComponents();
}

void Object::update(float dt) {
	CompIter end_it = components_.end();
	for (CompIter it = components_.begin(); it != end_it; ++it) {
		if ((*it)->toDelete) {
			delete (*it);
			components_.erase(it);
		}
		else {
			(*it)->update(dt);
		}
	}
}

void Object::addComponent(Component* component) {
	CompIter found = std::find(components_.begin(), components_.end(), component);

	if (found != components_.end()) {
		std::cout << "Warning: trying to add the same component twice." << std::endl;
	}

	component->setParent(this);
	components_.push_back(component);
}

void Object::removeComponent(Component* component) {
	CompIter found = std::find(components_.begin(), components_.end(), component);

	if (found != components_.end()) {
		return;
	}

	(*found)->toDelete = true;
}

glm::vec3 Object::getPosition() {
	return position_;
}

glm::vec3 Object::getRotation() {
	return rotation_;
}

void Object::setPosition(glm::vec3 pos) {
	position_ = pos;
}

void Object::setRotation(glm::vec3 rot) {
	rotation_ = rot;
}

void Object::removeAllComponents() {
	CompIter end_it = components_.end();
	for (CompIter it = components_.begin(); it != end_it; ++it) {
		delete (*it);
	}
	components_.clear();
}
