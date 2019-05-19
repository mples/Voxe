#pragma once
#include <unordered_map>
#include <vector>

#include "API.h"
#include "IComponent.h"
#include "Handle.h"
#include "Allocator/PoolAllocator.h"

class ComponentManager {
	class IComponentAllocator {
	public:
		virtual void freeComponent(IComponent * component) = 0;
	};

	template<class T>
	class ComponentPoolAllocator : public PoolAllocator<T>, public IComponentAllocator {
	public:
		ComponentPoolAllocator() : PoolAllocator<T>() {}

		void freeComponent(IComponent * component) override {
			//component->~IComponent();
			this->freeMemory(dynamic_cast<T*>(component));
		}
	};
public:
	ComponentManager();
	~ComponentManager();


	template<class T, class ...ARGS>
	T* addComponent(const EntityId e_id, ARGS&& ...args) {
		T* component = getComponentPoolAllocator<T>()->allocate<T>(std::forward<ARGS>(args)...);

		component->owner_ = e_id;
		ComponentId c_id = acquireId(component);

		component->id_ = c_id;

		return component;
	}

	template<class T>
	void eraseComponent(const EntityId e_id) {
		IComponent* component = entityComponentMap_[e_id.getIndex()][T::COMPONENT_TYPE_ID];

		assert(component != nullptr && "Trying to remove component not in use by this entity");

		releaseId(component->id_);
		getComponentPoolAllocator<T>()->freeComponent(component);
	}

	void eraseAllComponents(EntityId e_id) {
		for (auto comp_it : entityComponentMap_[e_id.getIndex()]) {
			if (comp_it.second != nullptr) {
				IComponentAllocator * comp_all = componentsPools_[comp_it.second->getTypeId()];
				releaseId(comp_it.second->getId());
				comp_all->freeComponent(comp_it.second);
			}
		}
		entityComponentMap_[e_id.getIndex()].clear();
	}

	template<class T>
	T* getComponent(const EntityId e_id) {
		IComponent* component = entityComponentMap_[e_id.getIndex()][T::COMPONENT_TYPE_ID];
		assert(component != nullptr);
		return dynamic_cast<T*>(component);
	}




private:
	ComponentId acquireId(IComponent * component);
	void releaseId(ComponentId id);

	template<class T>
	inline ComponentPoolAllocator<T>* getComponentPoolAllocator() {
		ComponentTypeId id = T::COMPONENT_TYPE_ID;

		auto found = componentsPools_.find(id);
		if (found == componentsPools_.end()) {
			ComponentPoolAllocator<T>* pool_ptr = new ComponentPoolAllocator<T>();
			componentsPools_[id] = pool_ptr;
			return pool_ptr;
		}
		else {
			return static_cast<ComponentPoolAllocator<T>*>(found->second);
		}
	}

	template<class T>
	using TComponentIterator = typename ComponentPoolAllocator<T>::iterator;

	template<class T>
	inline TComponentIterator<T> begin() {
		return getComponentPoolAllocator<T>()->begin();
	}

	template<class T>
	inline TComponentIterator<T> end() {
		return getComponentPoolAllocator<T>()->end();
	}

	std::unordered_map<ComponentTypeId, IComponentAllocator*> componentsPools_;

	std::unordered_map<unsigned int, std::unordered_map<unsigned int, IComponent*>> entityComponentMap_;
	HandleTable<IComponent> componentLookUpTable_;
};