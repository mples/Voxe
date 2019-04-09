#pragma once
#include <queue>
#include "Node.h"

template <typename T>
class Octree {
public:
	Octree(BoundingBox bounding_box);
	Octree(BoundingBox bounding_box, std::vector<T*> objects);
	~Octree();
	void insert(T* object);
	std::vector<T*> collides(BoundingFrustum& frustum);
	int size_;
private:
	Node<T>* root_;
};

template<typename T>
inline Octree<T>::Octree(BoundingBox bounding_box) : size_(0) {
	root_ = new Node<T>(bounding_box, nullptr);
}

template<typename T>
Octree<T>::Octree(BoundingBox bounding_box, std::vector<T*> objects) : size_(0) {
	root_ = new Node<T>(bounding_box, nullptr);
	for (T* o : objects) {
		root_.insert(o);
	}
}

template<typename T>
Octree<T>::~Octree() {
}

template<typename T>
void Octree<T>::insert(T * object) {
	std::queue<Node<T>*> queue;
	queue.push(root_);

	while (!queue.empty()) {
		Node<T>* node = queue.front();
		queue.pop();
		ContainmentType contains = node->contains(object->getBoundingVolume());
		switch (contains) {
		case ContainmentType::DISJOINT:
			if (node->parent_ == nullptr) {
				node->insert(object);
				return;
			}
			continue;
		case ContainmentType::INTERSECTS:
			if (node->parent_ != nullptr) {
				node->parent_->insert(object);
				return;
			}
			else {
				node->insert(object);
				return;
			}
			break;
		case ContainmentType::CONTAINS:
			if (node->isLeaf_) {
				node->insert(object);
				return;
			}
			else {
				for (int i = 0; i < node->children_.size(); i++) {
					queue.push(node->children_[i]);
				}
			}
			break;
		default:
			break;
		}

	}
	
}

template<typename T>
inline std::vector<T*> Octree<T>::collides(BoundingFrustum & frustum) {
	std::queue<Node<T>*> queue;
	queue.push(root_);
	std::vector<T*> result;
	while (!queue.empty()) {
		Node<T>* node = queue.front();
		queue.pop();
		if (ContainmentType::DISJOINT != frustum.Contains(node->boundingBox_)) {
			result.insert(result.end(), node->objects_.begin(), node->objects_.end());
			if (!node->isLeaf_) {
				for (int i = 0; i < node->children_.size(); i++) {
					queue.push(node->children_[i]);
				}
			}
		}
	}
	return result;
}
