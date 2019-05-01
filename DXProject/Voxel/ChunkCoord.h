#pragma once
#include <functional>
#include <DirectXMath.h>

struct ChunkCoord {
	ChunkCoord() : x_(0), y_(0), z_(0) {}
	ChunkCoord(DirectX::XMINT3 coord) : x_(coord.x), y_(coord.y), z_(coord.z) {}
	ChunkCoord(int x, int y, int z) : x_(x), y_(y), z_(z) {}
	ChunkCoord(const ChunkCoord& other) {
		x_ = other.x_;
		y_ = other.y_;
		z_ = other.z_;
	}


	//bool operator==(const ChunkCoord& other) {
	//	return (x_ == other.x_ && y_ == other.y_ && z_ == other.z_);
	//}
	//bool operator!=(const ChunkCoord& other) {
	//	return !(*this == other);
	//}

	int x_;
	int y_;
	int z_;
};

template<>
class std::hash<ChunkCoord> {
public:
	std::size_t operator()(const ChunkCoord& coord) const {
		return std::hash<int>()(coord.x_) ^ std::hash<int>()(coord.y_) ^ std::hash<int>()(coord.z_);
	}
};

inline bool operator==(const ChunkCoord& left, const ChunkCoord& right) {
	return (left.x_ == right.x_ && left.y_ == right.y_ && left.z_ == right.z_);
}

inline bool operator!=(const ChunkCoord& left, const ChunkCoord& right) {
	return !(left == right);
}