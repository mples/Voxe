#pragma once
#include <vector>
#include <array>

#include <DirectXCollision.h>
#include <Windows.h>

using namespace DirectX;

template <typename T>
class Node {
public:
	Node(BoundingBox bounding_box, Node * parent);
	Node(BoundingBox bounding_box, T* object, Node * parent);
	//Node(BoundingBox bounding_box, std::vector<T*> objects, Node * parent);
	~Node();
	bool insert(T* object);
	std::vector<T*> collides(BoundingFrustum& frustum);
private:
	const float MIN_DIMENSION = 16.0f;
	Node * parent_;
	std::array<Node<T>*, 8> childer_;
	std::vector<T*> objects_;
	BYTE activeChildren_;
	BoundingBox boundingBox_;
};


template<typename T>
inline Node<T>::Node(BoundingBox bounding_box, Node * parent) : activeChildren_(0) {
	boundingBox_ = bounding_box;
	parent_ = parent;
}

template<typename T>
Node<T>::Node(BoundingBox bounding_box, T * object, Node * parent) : activeChildren_(0) {
	boundingBox_ = bounding_box;
	parent_ = parent;
	insert(object);
}

//template<typename T>
//inline Node<T>::Node(BoundingBox bounding_box, std::vector<T*> objects, Node * parent) {
//	boundingBox_ = bounding_box;
//	parent_ = parent;
//	for (T* o : objects) {
//		insert(o);
//	}
//}

template<typename T>
inline Node<T>::~Node() {
}

template<typename T>
bool Node<T>::insert(T* object) {
	ContainmentType con = boundingBox_.Contains(object->getBoundingVolume());
	if (con == ContainmentType::CONTAINS) {
		if (activeChildren_ == 0) {
			XMFLOAT3 dimensions = boundingBox_.Extents;

			if (dimensions.x <= MIN_DIMENSION) {
				objects_.push_back(object);
				return true;
			}
			float half_dim = dimensions.x / 2.0f;

			std::array<BoundingBox, 8> octans = {
				BoundingBox(XMFLOAT3(-half_dim, -half_dim, -half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
				BoundingBox(XMFLOAT3(-half_dim, half_dim, -half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
				BoundingBox(XMFLOAT3(half_dim, half_dim, -half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
				BoundingBox(XMFLOAT3(half_dim, -half_dim, -half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
				BoundingBox(XMFLOAT3(-half_dim, -half_dim, half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
				BoundingBox(XMFLOAT3(-half_dim, half_dim, half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
				BoundingBox(XMFLOAT3(half_dim, half_dim, half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
				BoundingBox(XMFLOAT3(half_dim, -half_dim, half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
			};
			for (int i = 0; i < octans.size(); i++) {
				if (ContainmentType::CONTAINS == octans[i].Contains(object->getBoundingVolume())) {
					childer_[i] = new Node<T>(octans[i], object, this);
					BYTE mask = 1 << i;
					activeChildren_ |= mask;
					return true;
				}
			}

		}
		else {
			for (int i = 0; i < childer_.size(); i++) {
				if (activeChildren_ & (1 << i)) {
					if (childer_[i]->insert(object)) {
						return true;
					}
				}
			}
		}
	}
	else {
		if (parent_ == nullptr) {
			objects_.push_back(object);
				return true;
		}
		else {
			return false;
		}
	}
/*
	XMFLOAT3 dimensions = boundingBox_.Extents;

	if (dimensions.x <= MIN_DIMENSION) {
		return;
	}

	float half_dim = dimensions.x / 2.0f;

	std::array<BoundingBox, 8> octans = {
		BoundingBox(XMFLOAT3(-half_dim, -half_dim, -half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
		BoundingBox(XMFLOAT3(-half_dim, half_dim, -half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
		BoundingBox(XMFLOAT3(half_dim, half_dim, -half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
		BoundingBox(XMFLOAT3(half_dim, -half_dim, -half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
		BoundingBox(XMFLOAT3(-half_dim, -half_dim, half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
		BoundingBox(XMFLOAT3(-half_dim, half_dim, half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
		BoundingBox(XMFLOAT3(half_dim, half_dim, half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
		BoundingBox(XMFLOAT3(half_dim, -half_dim, half_dim), XMFLOAT3(half_dim, half_dim, half_dim)),
	};

	std::array<std::vector<T*>, 8> octan_objects = {
		{},{},{},{},{},{},{},{}
	};

	std::vector<T*> moved_objects;
	for (auto it = objects_.begin(); it != objects_.end(); it++) {
		for (int i = 0; i < 8; i++) {
			if (ContainmentType::CONTAINS == octans[i].Contains(it->getBoundingVolume())) {
				octan_objects[i].push_back(&it);
				objects_.erase(it);
			}

		}
	}

	for (int i = 0; i < childer_.size(), i++) {
		if(octan_objects[i].size == 0) {
			continue;
		}
		childer_[i] = Node<T>(octans[i], octan_objects[i]),
	}*/
}

template<typename T>
inline std::vector<T*> Node<T>::collides(BoundingFrustum & frustum) {
	if (activeChildren_ == 0) {
		return objects_;
	}
	else {
		std::vector<T*> objects;
		for (int i = 0; i < childer_.size(); i++) {
			if (activeChildren_ & (1 << i)) {
				std::vector<T*> o = childer_[i]->collides(frustum);
				objects.insert(objects.end(), o.begin(), o.end());
			}
		}
		objects.insert(objects.end(), objects_.begin(), objects_.end());
		return objects;
	}
}
