#pragma once
#include "ISystem.h"
#include "TypeId.h"

Id_t TypeId<ISystem>::count_ = 0u;

template<class T>
class System : public ISystem {
	friend class SystemManager;
public:
	System() {}
	~System() {}

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

	template<class... Dependencies>
	void addDependencies(Dependencies&&... dependencies) {
		// TODO add
	}

private:
	static const SystemTypeId SYSTEM_TYPE_ID;


	//TODO add system manager instance
};

template<class T>
const SystemTypeId System<T>::SYSTEM_TYPE_ID = TypeId<IEntity>::get<T>();

