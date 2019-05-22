#pragma once
#include <functional>
#include <DirectXMath.h>

struct TerrainCoord {
	TerrainCoord() : x_(0), y_(0), z_(0) {}
	TerrainCoord(DirectX::XMINT3 coord) : x_(coord.x), y_(coord.y), z_(coord.z) {}
	TerrainCoord(int x, int y, int z) : x_(x), y_(y), z_(z) {}
	TerrainCoord(const TerrainCoord& other) {
		x_ = other.x_;
		y_ = other.y_;
		z_ = other.z_;
	}

	DirectX::XMINT3 asXMINT3() {
		return DirectX::XMINT3(x_, y_, z_);
	}

	int x_;
	int y_;
	int z_;
};

template<>
class std::hash<TerrainCoord> {
public:
	std::size_t operator()(const TerrainCoord& coord) const {
		return std::hash<int>()(coord.x_) ^ std::hash<int>()(coord.y_) ^ std::hash<int>()(coord.z_);
	}
};

inline bool operator==(const TerrainCoord& left, const TerrainCoord& right) {
	return (left.x_ == right.x_ && left.y_ == right.y_ && left.z_ == right.z_);
}

inline bool operator!=(const TerrainCoord& left, const TerrainCoord& right) {
	return !(left == right);
}