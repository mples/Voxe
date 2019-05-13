#pragma once
#include <unordered_map>

#include "IEntity.h"
#include "Allocator/PoolAllocator.h"
#include "Handle.h"



class EntityManager {
public:
	EntityManager();	//TODO add component manager pointer
	~EntityManager();

	template<class T, class ... ARGS>
	EntityId createEntity(ARGS&&... args) {
		T* entity = getEntityPoolAllocator<T>()->allocate<T>(std::forward<ARGS>(args)...);
		EntityId id = handleTable_.acquireHandle(entity);

		entity->id_ = id;
		//insert comopnent manager instance

		return id;
	}

	template<class T>
	void eraseEntity(EntityId id) {
		IEntity * entity = handleTable_[id];
		EntityTypeId type_id = entity->getEntityTypeId();

		auto found = entitiesPools_.find(type_id);
		if (found != entitiesPools_.end()) {

			//TODO delete components

			dynamic_cast<PoolAllocator<T>*>(found->second).free(dynamic_cast<T*>(entity));
		}

		releaseEntityId(id);
	}
	template<class T>
	inline T* getEntity(EntityId id) {
		return dynamic_cast<T*>(handleTable_[id]);
	}

private:
	EntityManager(const EntityManager& other) = delete;
	EntityManager& operator=(const EntityManager& other) = delete;

	std::unordered_map<EntityTypeId, void*> entitiesPools_;

	HandleTable<IEntity> handleTable_;
	//TODO component manager

	EntityId acquireId(IEntity* entity);

	void releaseEntityId(EntityId id);

	template<class T> 
	inline PoolAllocator<T>* getEntityPoolAllocator() {
		EntityTypeId id = T::ENTITY_TYPE_ID;

		auto found = entitiesPools_.find(id);
		if (found == entitiesPools_.end()) {
			PoolAllocator<T>* pool_ptr = new PoolAllocator<T>();
			entitiesPools_[id] = static_cast<void*>(pool_ptr);
			return pool_ptr;
		}
		else {
			return static_cast<PoolAllocator<T>*>(found->second);
		}
	}

};