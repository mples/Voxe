#pragma once
#include "IEntity.h"
#include "TypeId.h"

Id_t TypeId<IEntity>::count_ = 0u;

template<class T>
class Entity : public IEntity {
public:
	Entity() {}
	virtual ~Entity() {}

	//Destruction is handled by entity manager
	void operator delete(void*) = delete;
	void operator delete[](void*) = delete;
	virtual const EntityTypeId getEntityTypeId() const override {
		return ENTITY_TYPE_ID;
	}

	static const EntityTypeId ENTITY_TYPE_ID;
};

template<class T>
const EntityTypeId Entity<T>::ENTITY_TYPE_ID = TypeId<IEntity>::get<T>(); 



	