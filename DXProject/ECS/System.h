#pragma once
#include "ISystem.h"
#include "TypeId.h"

Id_t TypeId<ISystem>::count_ = 0u;

template<class T>
class System : public ISystem {
public:
	System() {}
	virtual ~System() {}

	// Inherited via ISystem
	virtual void preUpdate(float dt) override {
	}

	virtual void update(float dt) override {
	}

	virtual void postUpdate(float dt) override {
	}

	virtual SystemTypeId getTypeId() override {
		return SYSTEM_TYPE_ID;
	}

	static const SystemTypeId SYSTEM_TYPE_ID;

private:
};

template<class T>
const SystemTypeId System<T>::SYSTEM_TYPE_ID = TypeId<ISystem>::get<T>();

