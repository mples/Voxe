#pragma once

#include <array>

template<typename T, size_t x_count, size_t y_count, size_t z_count>
class Array3D {
public:
	Array3D() {
	}

	Array3D(T obj) {
		fillWith(obj);
	}

	~Array3D() {
	}

	typename std::vector<T>::reference at(int x, int y, int z) {
		return array_[(z * x_count * y_count) + (y * x_count) + x];
	}

	void fillWith(T obj) {
		for (int i = 0; i < array_.size(); i++) {
			array_[i] = obj;
		}
	}

private:
	std::array<T, x_count * y_count * z_count> array_;
};
