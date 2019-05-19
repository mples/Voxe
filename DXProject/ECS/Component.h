#pragma once

#include "IComponent.h"
#include "TypeId.h"

Id_t TypeId<IComponent>::count_ = 0u;

template<class T>
class Component : public IComponent {
public:
	Component() {}
	virtual ~Component() {};

	inline const ComponentTypeId getTypeId() const override {
		return COMPONENT_TYPE_ID;
	}
	static const ComponentTypeId COMPONENT_TYPE_ID;
private:
};

template<class T>
const ComponentTypeId Component<T>::COMPONENT_TYPE_ID = TypeId<IComponent>::get<T>();