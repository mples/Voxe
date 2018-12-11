#pragma once
#include "ComponentType.h"

class Object;

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();
	virtual void update(float dt) = 0;
	virtual void activate();
	void setParent(Object * obj);

	bool toDelete;
protected:
	Object* parent_;
private:
	int id_;
	static int componentIdCount_;

	ComponentType type_;
};

