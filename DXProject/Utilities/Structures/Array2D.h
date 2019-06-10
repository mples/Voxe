#pragma once
#include <vector>

template<typename T, size_t columns_count, size_t rows_count>
class Array2D {
public:
	Array2D() {
	}

	~Array2D() {
	}

	typename std::vector<T>::reference at(int r, int c) {
		return array_[(r * columns_count) + c];
	}

	void fillWith(T obj) {
		for (int i = 0; i < array_.size(); i++) {
			array_[i] = obj;
		}
	}

private:
	std::array<T, rows_count * columns_count> array_;
};