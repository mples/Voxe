#pragma once
#include <vector>
#include <array>

#include <DirectXCollision.h>

using namespace DirectX;

template <typename T>
class Node {
public:
	Node(BoundingBox bounding_box, Node * parent);
	Node(BoundingBox bounding_box, T* object, Node * parent);
	Node(BoundingBox bounding_box, std::vector<T*> objects, Node * parent);
	~Node();
	Node * parent_;
	std::array<Node*, 8> children_;
	bool isLeaf_;
	std::vector<T*> objects_;
	BoundingBox boundingBox_;
	const unsigned int MAX_OBJ_COUNT = 16;
	const unsigned int MIN_OBJ_COUNT = 3;
	const float MIN_DIMENSION = 16.0f;
private:
};


template<typename T>
Node<T>::Node(BoundingBox bounding_box, Node * parent) : isLeaf_(true) {
	boundingBox_ = bounding_box;
	parent_ = parent;
}

template<typename T>
Node<T>::Node(BoundingBox bounding_box, T * object, Node * parent) : isLeaf_(true) {
	boundingBox_ = bounding_box;
	parent_ = parent;
	insert(object);
}

template<typename T>
Node<T>::Node(BoundingBox bounding_box, std::vector<T*> objects, Node * parent) : isLeaf_(true) {
	boundingBox_ = bounding_box;
	parent_ = parent;
	objects_.insert(objects_.end(), objects.begin(), objects.end());
}

template<typename T>
Node<T>::~Node() {
}
