#pragma once
#include <unordered_map>
#include "IComponent.h"

#include "Allocator/PoolAllocator.h"

class ComponentManager {
public:
	ComponentManager();
	~ComponentManager();
private:
	template<class T>
	inline PoolAllocator<T>* getComponentPoolAllocator() {
		ComponentTypeId id = T::COMPONENT_TYPE_ID;

		auto found = componentsPools_.find(id);
		if (found == componentsPools_.end()) {
			PoolAllocator<T>* pool_ptr = new PoolAllocator<T>();
			componentsPools_[id] = static_cast<void*>(pool_ptr);
			return pool_ptr;
		}
		else {
			return static_cast<PoolAllocator<T>*>(found->second);
		}
	}

	std::unordered_map<ComponentTypeId, void*> componentsPools_;


};