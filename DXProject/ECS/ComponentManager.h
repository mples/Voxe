#pragma once
#include <unordered_map>
#include <vector>
#include <mutex>

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
			this->freeMemory(dynamic_cast<T*>(component));
		}
	};
public:
	ComponentManager();
	~ComponentManager();


	template<class T, class ...ARGS>
	T* addComponent(const EntityId e_id, ARGS&& ...args) {
		std::lock_guard<std::mutex> lock(mutex_);
		T* component = getComponentPoolAllocator<T>()->allocateNotInitialized();

		component->owner_ = e_id;
		ComponentId c_id = acquireId<T>(component);

		component->id_ = c_id;
		component = new(component)T(std::forward<ARGS>(args)...);
		return component;
	}

	template<class T>
	void eraseComponent(const EntityId e_id) {
		std::lock_guard<std::mutex> lock(mutex_);
		IComponent* component = entityComponentMap_[e_id.getIndex()][T::COMPONENT_TYPE_ID];

		assert(component != nullptr && "Trying to remove component not in use by this entity");

		releaseId(component->id_);
		getComponentPoolAllocator<T>()->freeComponent(component);
	}

	void eraseAllComponents(EntityId e_id) {
		std::lock_guard<std::mutex> lock(mutex_);
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
	T* getComponentByEntityId(const EntityId e_id) {
		std::lock_guard<std::mutex> lock(mutex_);
		IComponent* component = entityComponentMap_[e_id.getIndex()][T::COMPONENT_TYPE_ID];
		return dynamic_cast<T*>(component);
	}

	template<class T>
	T* getComponentByComponentId(const ComponentId c_id) {
		std::lock_guard<std::mutex> lock(mutex_);
		IComponent* component = componentLookUpTable_[c_id];
		return dynamic_cast<T*>(component);
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

private:
	template <class T>
	ComponentId acquireId(IComponent * component) {
		EntityId e_id = component->getOwner();
		ComponentId result;
		auto found = entityComponentMap_.find(e_id.getIndex());
		if (found != entityComponentMap_.end()) {
			result = componentLookUpTable_.acquireHandle(component);

			found->second[T::COMPONENT_TYPE_ID] = component;
		}
		else {
			entityComponentMap_[e_id.getIndex()] = std::unordered_map<unsigned int, IComponent*>();
			result = componentLookUpTable_.acquireHandle(component);
			entityComponentMap_[e_id.getIndex()][T::COMPONENT_TYPE_ID] = component;
		}
		return result;
	}
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


	std::unordered_map<ComponentTypeId, IComponentAllocator*> componentsPools_;

	std::unordered_map<unsigned int, std::unordered_map<unsigned int, IComponent*>> entityComponentMap_;
	HandleTable<IComponent> componentLookUpTable_;

	std::mutex mutex_;
};