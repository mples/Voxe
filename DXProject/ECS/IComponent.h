#pragma once

#include "IEntity.h"

using ComponentId = unsigned int;
using ComponentTypeId = unsigned int;

class IComponent {
	friend class ComponentManager;
public:
	IComponent() : active_(true) {}
	virtual ~IComponent() {}

	inline bool operator==(const IComponent& other) {
		return id_ == other.id_ && owner_ == other.owner_;
	}

	inline bool operator!=(const IComponent& other) {
		return id_ != other.id_ || owner_ != other.owner_;
	}

	inline const ComponentId getId() const {
		return id_;
	}

	inline const EntityId getOwner() const {
		return owner_;
	}

	inline void setActive(bool a) {
		active_ = a;
	}

	inline bool isActive() {
		return active_;
	}
protected:
	ComponentId id_;
	EntityId owner_;
	bool active_;

};