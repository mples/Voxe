#pragma once
#include "Object.h"
#include "Singleton.h"

#include <vector>

class ObjectManager : public Singleton<ObjectManager>{
public:
	ObjectManager();
	~ObjectManager();

	void addObject(Object* object);
	void removeObject(Object* object);

	void update(float dt);

		
private:
	std::vector<Object*> objects_;

	void destroyObject(std::vector<Object*>::iterator it);
};

