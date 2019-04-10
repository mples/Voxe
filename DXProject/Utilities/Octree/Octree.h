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
	void remove(T* object);
	std::vector<T*> collides(BoundingFrustum& frustum);
	int size_;
private:
	void insertIntoNode(T* object, Node<T>* node);
	bool removeFromNode(T* object, Node<T>* node);
	ContainmentType nodeContains(BoundingFrustum & frustum, Node<T> * node);
	int countOfChildren(Node<T> * node);
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
		ContainmentType contains = node->boundingBox_.Contains(object->getBoundingVolume());
		switch (contains) {
		case ContainmentType::DISJOINT:
			if (node->parent_ == nullptr) {
				insertIntoNode(object, node);
				return;
			}
			continue;
		case ContainmentType::INTERSECTS:
			if (node->parent_ != nullptr) {
				insertIntoNode(object, node->parent_);
				return;
			}
			else {
				insertIntoNode(object, node);
				return;
			}
			break;
		case ContainmentType::CONTAINS:
			if (node->isLeaf_) {
				insertIntoNode(object, node);
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
void Octree<T>::remove(T * object) {
	std::queue<Node<T>*> queue;
	queue.push(root_);
	while (!queue.empty()) {
		Node<T>* node = queue.front();
		queue.pop();
		if (ContainmentType::DISJOINT != node->boundingBox_.Contains(object->getBoundingVolume() )) {
			if (removeFromNode(object, node)) {
				return;
			}
			else {
				if (!node->isLeaf_) {
					for (int i = 0; i < node->children_.size(); i++) {
						queue.push(node->children_[i]);
					}
				}
			}
		}
	}
	return;
}

template<typename T>
std::vector<T*> Octree<T>::collides(BoundingFrustum & frustum) {
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

template<typename T>
void Octree<T>::insertIntoNode(T * object, Node<T>* node) {
	node->objects_.push_back(object);
	if (node->objects_.size() > node->MAX_OBJ_COUNT && node->isLeaf_) {
		float dim = node->boundingBox_.Extents.x / 2.0f;
		if (dim <= node->MIN_DIMENSION) {
			return;
		}
		node->children_[0] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x + dim, node->boundingBox_.Center.y + dim, node->boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), node);
		node->children_[1] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x - dim, node->boundingBox_.Center.y + dim, node->boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), node);
		node->children_[2] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x - dim, node->boundingBox_.Center.y - dim, node->boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), node);
		node->children_[3] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x + dim, node->boundingBox_.Center.y - dim, node->boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), node);
		node->children_[4] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x + dim, node->boundingBox_.Center.y + dim, node->boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), node);
		node->children_[5] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x - dim, node->boundingBox_.Center.y + dim, node->boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), node);
		node->children_[6] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x - dim, node->boundingBox_.Center.y - dim, node->boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), node);
		node->children_[7] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x + dim, node->boundingBox_.Center.y - dim, node->boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), node);

		node->isLeaf_ = false;
		std::vector<T*> new_objects;

		for (int i = 0; i < node->objects_.size(); i++) {
			bool added = false;
			for (int j = 0; j < node->children_.size(); j++) {
				if (ContainmentType::CONTAINS == node->children_[j]->boundingBox_.Contains(node->objects_[i]->getBoundingVolume())) {
					//node->children_[j]-> insertI (objects_[i]);
					insertIntoNode(node->objects_[i], node->children_[j]);
					added = true;
					continue;
				}
			}
			if (!added) {
				new_objects.push_back(node->objects_[i]);
			}
		}
		node->objects_ = new_objects;
	}
	return;
}

template<typename T>
bool Octree<T>::removeFromNode(T * object, Node<T>* node) {
	auto del_obj = std::find(node->objects_.begin(), node->objects_.end(), object);
	if (del_obj != node->objects_.end()) {
		node->objects_.erase(del_obj);
		return true;
	}
	return false;
}

template<typename T>
ContainmentType Octree<T>::nodeContains(BoundingFrustum & frustum, Node<T>* node) {
	return node->boundingBox_.Contains(frustum);
}

template<typename T>
int Octree<T>::countOfChildren(Node<T>* node) {
	std::queue<Node<T>*> queue;
	queue.push(root_);
	int children_count = 0;
	while (!queue.empty()) {
		Node<T>* node = queue.front();
		queue.pop();
		children_count += node->objects_.size();
		if (!node->isLeaf_) {
			for (int i = 0; i < node->objects_.size(); i++) {
				queue.push(node->objects_[i]);
			}
		}
	}
	return children_count;
}
