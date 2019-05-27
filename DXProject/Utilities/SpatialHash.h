#pragma once
#include <unordered_map>
#include <vector>

#include <DirectXMath.h>

using namespace DirectX;

struct Point {
	Point(float x, float y, float z) : x_(x), y_(y), z_(z) {}
	int x_;
	int y_;
	int z_;
};

template<>
class std::hash<Point> {
public:
	std::size_t operator()(const Point& coord) const {
		return std::hash<int>()(coord.x_) ^ std::hash<int>()(coord.y_) ^ std::hash<int>()(coord.z_);
	}
};

inline bool operator==(const Point& left, const Point& right) {
	return (left.x_ == right.x_ && left.y_ == right.y_ && left.z_ == right.z_);
}

inline bool operator!=(const Point& left, const Point& right) {
	return !(left == right);
}

template <typename T>
class SpatialHash {
public:
	SpatialHash(float cell_size);
	~SpatialHash();
	Point hash(float x, float y, float z);
	Point hash(XMFLOAT3 point);
	void insert(T* obj);
	void remove(T* obj);
	void clear();
	std::vector<T*>* getBucket(int x, int y, int z);
	std::vector<T*>* getBucket(Point p);
private:
	float cellSize_;
	std::unordered_map<Point, std::vector<T*>> map_;
};

template<typename T>
SpatialHash<T>::SpatialHash(float cell_size) : cellSize_(cell_size) {
}

template<typename T>
 SpatialHash<T>::~SpatialHash() {
}

template<typename T>
Point SpatialHash<T>::hash(float x, float y, float z) {

	return Point(static_cast<int>(x/cellSize_), static_cast<int>(y / cellSize_), static_cast<int>(z / cellSize_));
}

template<typename T>
Point SpatialHash<T>::hash(XMFLOAT3 point) {
	return Point(static_cast<int>(point.x / cellSize_), static_cast<int>(point.y / cellSize_), static_cast<int>(point.z / cellSize_));
}

template<typename T>
void SpatialHash<T>::insert(T* obj) {
	Point p = hash(obj->getPos());

	auto found = map_.find(p);

	if (found == map_.end()) {
		std::vector<T*> bucket;
		bucket.push_back(obj);
		map_.insert(std::make_pair(p, bucket));
	}
	else {
		found->second.push_back(obj);
	}
}

template<typename T>
void SpatialHash<T>::remove(T * obj) {
	Point p = hash(obj->getPos());
	auto found = map_.find(p);

	if (found != map_.end()) {
		std::vector<T*> bucket = found->second;
		auto it = std::find(bucket.begin(), bucket.end(), obj);
		if (it != bucket.end()) {
			bucket.erase(it);
		}
	}
}

template<typename T>
void SpatialHash<T>::clear() {
	map_.clear();
}

template<typename T>
std::vector<T*>* SpatialHash<T>::getBucket(int x, int y, int z) {
	Point p(x, y, z);
	auto found = map_.find(p);
	if (found != map_.end()) {
		return &found->second;
	}
	return nullptr;
}

template<typename T>
inline std::vector<T*>* SpatialHash<T>::getBucket(Point p) {
	auto found = map_.find(p);
	if (found != map_.end()) {
		return &found->second;
	}
	return nullptr;
}

