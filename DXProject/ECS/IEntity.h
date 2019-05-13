#pragma once
#include "Handle.h"

using EntityId = Handle;

using EntityTypeId = unsigned int;

class IEntity {
	friend class EntityManager;
public:
	IEntity() :active_(true) {}
	virtual ~IEntity() {}

	template<class T>
	T* getComponent() const {
		return nullptr; //TODO
	}

	template<class T, class ...P>
	T* addComponent(P&&... param) {
		return nullptr; //TODO
	}
	template<class T>
	void removeComponent() {
		//TODO
	}

	inline bool operator==(const IEntity& other) {
		return id_ == other.id_;
	}

	inline bool operator!=(const IEntity& other) {
		return id_ != other.id_;
	}

	inline bool operator==(const IEntity* other) {
		return id_ == other->id_;
	}

	inline bool operator!=(const IEntity* other) {
		return id_ == other->id_;
	}

	virtual const EntityTypeId getEntityTypeId() const = 0;

	inline const EntityId getEntityId() const {
		return id_;
	}

	void setActive(bool active) {
		if (active_ == active) {
			return;
		}

		if (active_ == false) {
			onDisable();
		}
		else {
			onEnable();
		}
		active_ = active;
	}

	virtual void onEnable() {};
	virtual void onDisable() {};
protected:
	EntityId id_;

	bool active_;
private:
	//TODO component manager pointer
};