#pragma once

using EntityId = unsigned int;

using EntityTypeId = unsigned int;

class IEntity {
public:
	IEntity();
	virtual ~IEntity();

	template<class T>
	T* getComponent() const {
		return nullptr //TODO
	}

	template<class T, class ...P>
	T* addComponent(P&&... param) {
		return nullptr //TODO
	}
	template<class T>
	void removeComponent() {

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