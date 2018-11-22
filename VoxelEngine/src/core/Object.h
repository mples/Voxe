#pragma once
#include<vector>
#include <glm/glm.hpp>


class Component;

class Object
{
public:
	Object();
	~Object();

	void update(float dt);
	void addComponent(Component* component);
	void removeComponent(Component* component);
private:
	void removeAllComponents();
	 
	glm::vec3 position_;
	using CompVec = std::vector<Component*>;
	using CompIter = CompVec::iterator;
	CompVec components_;
	const int START_COMPVEC_SIZE = 5;

	int id_;
	static int objectIdCount_;
};

