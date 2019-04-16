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
	static bool equalsBoundingBox(BoundingBox a, BoundingBox b);
	BoundingBox & getBoundingBox();

	int size_;
private:
	void insertIntoNode(T* object, Node<T>* node);
	void splitNode(Node<T>* node);
	bool removeFromNode(T* object, Node<T>* node);
	ContainmentType nodeContains(BoundingFrustum & frustum, Node<T> * node);
	void extend();
	Node<T> * calculateNewRoot();
	void shrink();
	int countOfObjects(Node<T> * node);

	const int MAX_OUTSIDE_POINT_COUNT = 128;
	const int MIN_OUTSIDE_POINT_COUNT = 16;
	Node<T>* root_;
	std::vector<T*> outsideObjects_;
};

template<typename T>
Octree<T>::Octree(BoundingBox bounding_box) : size_(0) {
	root_ = new Node<T>(bounding_box);
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
	delete root_;
}

template<typename T>
void Octree<T>::insert(T * object) {

	if (ContainmentType::DISJOINT == root_->boundingBox_.Contains(object->getBoundingVolume())) {
		outsideObjects_.push_back(object);
		if (outsideObjects_.size() > MAX_OUTSIDE_POINT_COUNT) {
			extend();
		}
		return;
	}

	std::queue<Node<T>*> queue;
	queue.push(root_);

	while (!queue.empty()) {
		Node<T>* node = queue.front();
		queue.pop();
		ContainmentType contains = node->boundingBox_.Contains(object->getBoundingVolume());
		switch (contains) {
		case ContainmentType::DISJOINT:
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
	auto it = std::find(outsideObjects_.begin(), outsideObjects_.end(), object);
	if (it != outsideObjects_.end()) {
		outsideObjects_.erase(it);
		return;
	}

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
	for (T* o : outsideObjects_) {
		if (ContainmentType::DISJOINT != frustum.Contains(o->getBoundingVolume())) {
			result.push_back(o);
		}
	}
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
inline bool Octree<T>::equalsBoundingBox(BoundingBox a, BoundingBox b) {
	return a.Center.x == b.Center.x && a.Center.y == b.Center.y && a.Center.z == b.Center.z &&
				a.Extents.x == b.Extents.x && a.Extents.y == b.Extents.y && a.Extents.z == b.Extents.z;
}

template<typename T>
inline BoundingBox & Octree<T>::getBoundingBox() {
	return root_->boundingBox_;
}

template<typename T>
void Octree<T>::insertIntoNode(T * object, Node<T>* node) {
	node->objects_.push_back(object);
	if (node->objects_.size() > node->MAX_OBJ_COUNT && node->isLeaf_) {
		if (node->boundingBox_.Extents.x / 2.0f <= node->MIN_DIMENSION) {
			return;
		}
		splitNode(node);

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
void Octree<T>::splitNode(Node<T>* node) {
	float dim = node->boundingBox_.Extents.x / 2.0f;
	node->children_[0] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x + dim, node->boundingBox_.Center.y + dim, node->boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), node);
	node->children_[1] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x - dim, node->boundingBox_.Center.y + dim, node->boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), node);
	node->children_[2] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x - dim, node->boundingBox_.Center.y - dim, node->boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), node);
	node->children_[3] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x + dim, node->boundingBox_.Center.y - dim, node->boundingBox_.Center.z + dim), XMFLOAT3(dim, dim, dim)), node);
	node->children_[4] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x + dim, node->boundingBox_.Center.y + dim, node->boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), node);
	node->children_[5] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x - dim, node->boundingBox_.Center.y + dim, node->boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), node);
	node->children_[6] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x - dim, node->boundingBox_.Center.y - dim, node->boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), node);
	node->children_[7] = new Node<T>(BoundingBox(XMFLOAT3(node->boundingBox_.Center.x + dim, node->boundingBox_.Center.y - dim, node->boundingBox_.Center.z - dim), XMFLOAT3(dim, dim, dim)), node);

	node->isLeaf_ = false;
}

template<typename T>
bool Octree<T>::removeFromNode(T * object, Node<T>* node) {
	auto del_obj = std::find(node->objects_.begin(), node->objects_.end(), object);
	if (del_obj != node->objects_.end()) {
		node->objects_.erase(del_obj);
		shrink();
		return true;
	}
	return false;
}

template<typename T>
ContainmentType Octree<T>::nodeContains(BoundingFrustum & frustum, Node<T>* node) {
	return node->boundingBox_.Contains(frustum);
}

template<typename T>
void Octree<T>::extend() {
	Node<T>* new_root = calculateNewRoot();
	splitNode(new_root);
	for (int i = 0; i < new_root->children_.size(); i++) {
		if (equalsBoundingBox(new_root->children_[i]->boundingBox_, root_->boundingBox_)) {
			new_root->children_[i] = root_;
			root_->parent_ = new_root;
			root_ = new_root;
			break;
		}
	}

	std::vector<T*> objects_to_insert = outsideObjects_;
	outsideObjects_.clear();
	for (T* obj : objects_to_insert) {
		insert(obj);
	}
}

template<typename T>
inline Node<T>* Octree<T>::calculateNewRoot() {
	float dim = root_->boundingBox_.Extents.x;
	float ext_dim = root_->boundingBox_.Extents.x * 2.0f;
	XMFLOAT3 corners[8];
	root_->boundingBox_.GetCorners(corners);
	int corners_count = 8;
		//ARRAYSIZE(corners);
	assert(corners_count == 8);
	std::array<Node<T>*, 8> potential_roots;
	std::array<std::vector<T*>, 8> pot_roots_elements = {};
	for (int i = 0; i < corners_count; i++) {
		potential_roots[i] = new Node<T>(BoundingBox(corners[i], XMFLOAT3(ext_dim, ext_dim, ext_dim)));
	}

	int new_root_index = 0;
	for (T* obj : outsideObjects_) {
		for (int i = 0; i < potential_roots.size(); i++) {
			if (ContainmentType::DISJOINT != potential_roots[i]->boundingBox_.Contains(obj->getBoundingVolume())) {
				pot_roots_elements[i].push_back(obj);
				if (new_root_index != i && pot_roots_elements[i].size() > pot_roots_elements[new_root_index].size()) {
					new_root_index = i;
				}
			}
		}
	}
	Node<T> * new_root = potential_roots[new_root_index];

	potential_roots[new_root_index] = nullptr;
	for (Node<T>* node : potential_roots) {
		if (node != nullptr) {
			delete node;
		}
	}

	return new_root;
}

template<typename T>
void Octree<T>::shrink() {
	if (root_->isLeaf_) {
		return;
	}
	int max_count = 0;
	int sum;
	Node<T> * new_root;
	auto it = std::for_each(root_->children_.begin(), root_->children_.end(), [&](Node<T>* node) {
		int count_of_objects = countOfObjects(node);
		if (count_of_objects > max_count) {
			max_count = count_of_objects;
			new_root = node;
		}
		sum += count_of_objects;
	});

	sum -= max_count;
	sum += outsideObjects_.size();

	if (sum >= MIN_OUTSIDE_POINT_COUNT) {
		return ;
	}
	
	std::queue<Node<T>*> queue;
	queue.push(root_);

	while (!queue.empty()) {
		Node<T>* node = queue.front();
		queue.pop();
		if (node != new_root) {
			for (T* obj : node->objects_) {
				outsideObjects_.push_back(obj);
			}
		}
	}
	for (Node<T>* n : root_->children_) {
		if (!root_->isLeaf_) {
			if (n != new_root) {
				delete n;
			}
		}
	}
	//setting this to delete root properly
	root_->isLeaf_ = true;
	delete root_;

	root_ = new_root;
	new_root->parent_ = nullptr;
}

template<typename T>
int Octree<T>::countOfObjects(Node<T>* node) {
	std::queue<Node<T>*> queue;
	queue.push(node);
	int children_count = 0;
	while (!queue.empty()) {
		Node<T>* node = queue.front();
		queue.pop();
		children_count += node->objects_.size();
		if (!node->isLeaf_) {
			for (int i = 0; i < node->children_.size(); i++) {
				queue.push(node->children_[i]);
			}
		}
	}
	return children_count;
}
