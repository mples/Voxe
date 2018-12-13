#include "ObjectManager.h"
#include <algorithm>


ObjectManager::ObjectManager() : Singleton<ObjectManager>() {

}


ObjectManager::~ObjectManager() {
}

void ObjectManager::addObject(Object * object) {
	auto found = std::find(objects_.begin(), objects_.end(), object);
	if (found == objects_.end()) {
		objects_.push_back(object);
	}
}

void ObjectManager::removeObject(Object * object) {
	auto found = std::find(objects_.begin(), objects_.end(), object);
	if (found != objects_.end()) {
		(*found)->toDelete_ = true;
	}
}

void ObjectManager::update(float dt) {
	auto end = objects_.end();
	for (auto it = objects_.begin(); it != end ; ++it) {
		if ((*it)->toDelete_) {
			destroyObject(it);
		}
		else {
			(*it)->update(dt);
		}
	}
}

void ObjectManager::destroyObject(std::vector<Object*>::iterator it) {
	Object* to_delete = *it;
	objects_.erase(it);
	delete to_delete;
}
