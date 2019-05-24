#include "ComponentManager.h"
#include <cassert>

ComponentManager::ComponentManager() {
}

ComponentManager::~ComponentManager() {
}

ComponentId ComponentManager::acquireId(IComponent * component) {
	EntityId e_id = component->getOwner();
	ComponentId result;
	auto found = entityComponentMap_.find(e_id.getIndex());
	if (found != entityComponentMap_.end()) {
		result = componentLookUpTable_.acquireHandle(component);

		//assert( (found->second.count(component->getTypeId()) == 0) && "Cannot add two components of the same type to one entity");
		found->second[component->getTypeId()] = component;
	}
	else {
		entityComponentMap_[e_id.getIndex()] = std::unordered_map<unsigned int, IComponent*>();
		result = componentLookUpTable_.acquireHandle(component);
		entityComponentMap_[e_id.getIndex()][component->getTypeId()] = component;
	}
	return result;
}

void ComponentManager::releaseId(ComponentId id) {
	EntityId e_id = componentLookUpTable_[id]->getOwner();

	auto found = entityComponentMap_.find(e_id.getIndex());
	if (found != entityComponentMap_.end()) {
		unsigned int a = componentLookUpTable_[id]->getTypeId();
		found->second[componentLookUpTable_[id]->getTypeId()] = nullptr;
		
		componentLookUpTable_.releaseHandle(id);
	}
	else {
		assert(0 && "Trying to release component which is not acquired correctly.");
	}
}
