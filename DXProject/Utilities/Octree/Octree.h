#pragma once
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
	Node<T> root_;
};

template<typename T>
inline Octree<T>::Octree(BoundingBox bounding_box) : root_(bounding_box, nullptr), size_(0) {
}

template<typename T>
Octree<T>::Octree(BoundingBox bounding_box, std::vector<T*> objects) : root_(bounding_box, nullptr), size_(0) {
	for (T* o : objects) {
		root_.insert(o);
	}
}

template<typename T>
Octree<T>::~Octree() {
}

template<typename T>
void Octree<T>::insert(T * object) {
	root_.insert(object);
	
}

template<typename T>
inline std::vector<T*> Octree<T>::collides(BoundingFrustum & frustum) {
	return root_.collides(frustum);
}
