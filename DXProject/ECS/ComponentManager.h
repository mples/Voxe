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
		void freeComponent(IComponent * component) override {
			//component->~IComponent();
			free(dynamic_cast<T*>(component));
		}
	};
public:
	ComponentManager();
	~ComponentManager();

	ComponentId acquireId(IComponent * component);
	void releaseId(ComponentId id);

	template<class T, class ...ARGS>
	T* addComponent(const EntityId e_id, ARGS&& ...args) {
		T* component = getComponentPoolAllocator<T>()->allocate<T>(std::forward<ARGS>(args)...);
		ComponentId c_id = acquireId(component);

		component->id_ = c_id;
		component->owner_ = e_id;

		return c_id;
	}

	template<class T>
	void eraseComponent(const EntityId e_id) {
		IComponent* component = entityComponentMap_[e_id.getIndex()][T->getTypeId()];

		assert(component != nullptr && "Trying to remove component not in use by this entity");
		getComponentPoolAllocator<T>()->freeComponent(component);

		releaseId(component->id_);
	}

	void eraseAllComponents(EntityId e_id) {
		for (auto comp_it : entityComponentMap_[e_id.getIndex()]) {
			IComponentAllocator * comp_all = componentsPools_[comp_it.second->getTypeId()];
			releaseId(comp_it.second->getId());
			comp_all->freeComponent(comp_it.second);
		}
		entityComponentMap_[e_id.getIndex()].clear();
	}

	template<class T>
	T* getComponent(const EntityId e_id) {
		T* component = entityComponentMap_[e_id.getIndex()][T->getTypeId()];
		assert(component != nullptr);
		return component;
	}




private:
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
			return static_cast<PoolAllocator<T>*>(found->second);
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