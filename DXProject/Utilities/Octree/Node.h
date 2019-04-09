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
	bool insert(T* object);
	std::vector<T*> collide(BoundingFrustum& frustum);
	ContainmentType contains(BoundingBox bounding_box);
	Node * parent_;
	std::array<Node*, 8> children_;
	bool isLeaf_;
	std::vector<T*> objects_;
	BoundingBox boundingBox_;
	const unsigned int MAX_OBJ_COUNT = 16;
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

template<typename T>
bool Node<T>::insert(T* object) {
	objects_.push_back(object);
	if (objects_.size() > MAX_OBJ_COUNT && isLeaf_ ) {
		float dim = boundingBox_.Extents.x / 2.0f;
		if (dim <= MIN_DIMENSION) {
			return true;
		}
		children_[0] = new Node<T>(BoundingBox(XMFLOAT3(boundingBox_.Center.x + dim, boundingBox_.Center.y + dim, boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), this);
		children_[1] = new Node<T>(BoundingBox(XMFLOAT3(boundingBox_.Center.x - dim, boundingBox_.Center.y + dim, boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), this);
		children_[2] = new Node<T>(BoundingBox(XMFLOAT3(boundingBox_.Center.x - dim, boundingBox_.Center.y - dim, boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), this);
		children_[3] = new Node<T>(BoundingBox(XMFLOAT3(boundingBox_.Center.x + dim, boundingBox_.Center.y - dim, boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), this);
		children_[4] = new Node<T>(BoundingBox(XMFLOAT3(boundingBox_.Center.x + dim, boundingBox_.Center.y + dim, boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), this);
		children_[5] = new Node<T>(BoundingBox(XMFLOAT3(boundingBox_.Center.x - dim, boundingBox_.Center.y + dim, boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), this);
		children_[6] = new Node<T>(BoundingBox(XMFLOAT3(boundingBox_.Center.x - dim, boundingBox_.Center.y - dim, boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), this);
		children_[7] = new Node<T>(BoundingBox(XMFLOAT3(boundingBox_.Center.x + dim, boundingBox_.Center.y - dim, boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), this);

		isLeaf_ = false;
		std::vector<T*> new_objects;

		for (int i = 0; i < objects_.size(); i++) {
			bool added = false;
			for (int j = 0; j < children_.size(); j++) {
				if (ContainmentType::CONTAINS == children_[j]->contains(objects_[i]->getBoundingVolume()) ) {
					children_[j]->insert(objects_[i]);
						added = true;
				}
			}
			if (!added) {
				new_objects.push_back(objects_[i]);
			}
		}
		objects_ = new_objects;
	}
	return true;
}

template<typename T>
std::vector<T*> Node<T>::collide(BoundingFrustum & frustum) {

}

template<typename T>
ContainmentType Node<T>::contains(BoundingBox bounding_box) {
	return boundingBox_.Contains(bounding_box);
}
