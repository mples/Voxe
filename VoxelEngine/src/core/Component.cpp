#include "Component.h"
#include "Object.h"

int Component::componentIdCount_ = 0;

Component::Component(ComponentType type) : toDelete(false), parent_(nullptr), type_(type), id_(++componentIdCount_) {
}

Component::~Component() {
}

void Component::setParent(Object * obj) {
	parent_ = obj;
}

