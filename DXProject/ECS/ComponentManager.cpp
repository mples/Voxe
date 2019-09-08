#include "ComponentManager.h"
#include <cassert>

ComponentManager::ComponentManager() {
}

ComponentManager::~ComponentManager() {
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
