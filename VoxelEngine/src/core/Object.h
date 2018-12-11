#pragma once
#include<vector>
#include <glm/glm.hpp>


class Component;

class Object
{
public:
	Object();
	~Object();

	virtual void update(float dt);
	void addComponent(Component* component);
	void removeComponent(Component* component);

	glm::vec3 getPosition();
	glm::vec3 getRotation();

	void setPosition(glm::vec3 pos);
	void setRotation(glm::vec3 rot);
private:
	void removeAllComponents();
	 
	glm::vec3 position_;
	glm::vec3 rotation_;
	using CompVec = std::vector<Component*>;
	using CompIter = CompVec::iterator;
	CompVec components_;
	const int START_COMPVEC_SIZE = 5;

	int id_;
	static int objectIdCount_;
};

