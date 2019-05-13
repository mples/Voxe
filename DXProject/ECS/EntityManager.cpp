#include "EntityManager.h"

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
	for (auto pa : entitiesPools_) {
		delete pa.second;
		pa.second = nullptr;
	}
}

EntityId EntityManager::acquireId(IEntity * entity) {
	return handleTable_.acquireHandle(entity);
}

void EntityManager::releaseEntityId(EntityId id) {
	handleTable_.releaseHandle(id);
}
