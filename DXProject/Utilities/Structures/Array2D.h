#pragma once
#include <vector>

template<typename T, size_t columns_count, size_t rows_count>
class Array2D {
public:
	Array2D() {
		array_.resize(rows_count * columns_count);
	}

	~Array2D() {
		array_.clear();
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
	std::vector<T> array_;
};

//template< size_t columns_count, size_t rows_count>
//class Array2D<bool, columns_count, rows_count> {
//public:
//	Array2D() {
//		array_.resize(rows_count * columns_count);
//	}
//
//	~Array2D() {
//		array_.clear();
//	}
//
//	bool& at(int r, int c) {
//		return array_[(r * columns_count) + c];
//	}
//
//	void fillWith(bool obj) {
//		for (int i = 0; i < array_.size(); i++) {
//			array_[i] = obj;
//		}
//	}
//
//private:
//	std::vector<char> array_;
//};