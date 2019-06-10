#pragma once
#include <limits>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>

class Handle {
public:
	Handle() {
	/*	index_ = std::numeric_limits<unsigned int>::max();
		version_ = std::numeric_limits<unsigned int>::max();*/
	}
	Handle(unsigned int index, unsigned int vers) : index_(index), version_(vers) {}

	bool operator==(const Handle& other) {
		return index_ == other.index_ && version_ == other.version_;
	}
	bool operator!=(const Handle& other) {
		return index_ != other.index_ || version_ != other.version_;
	}
	inline const unsigned int getIndex() const {
		return index_;
	}
	inline const unsigned int getVersion() const {
		return version_;
	}
	static unsigned int maxIndex() {
		return std::numeric_limits<unsigned int>::max() - 1;
	}
	static unsigned int minVersion() {
		return std::numeric_limits<unsigned int>::min();
	}
	static unsigned int maxVersion() {
		return std::numeric_limits<unsigned int>::max() - 1;
	}
	static unsigned int minIndex() {
		return std::numeric_limits<unsigned int>::min();
	}
	static Handle getInvalidHandle() {
		return Handle(std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max());
	}

private:
	unsigned int index_;
	unsigned int version_;	//storing version counter to make sure handle is valid 
};


template<class T, size_t size = 1024>
class HandleTable {
public:
	HandleTable() : firstEmptyIndex_(0) {
		extendTable();
	}

	~HandleTable() {}

	Handle acquireHandle(T* object) {
		if (!emptyIndexes_.empty()) {
			unsigned int index = emptyIndexes_.front();
			table_[index].second = object;
			table_[index].first = (table_[index].first + 1) % Handle::maxVersion();
			emptyIndexes_.pop_front();
			return Handle(index, table_[index].first);
		}
		else {
			if (firstEmptyIndex_ == table_.size()) {
				extendTable();
			}
			table_[firstEmptyIndex_].second = object;
			table_[firstEmptyIndex_].first = (table_[firstEmptyIndex_].first + 1) % Handle::maxVersion();
			Handle created_handle (firstEmptyIndex_, table_[firstEmptyIndex_].first);
			firstEmptyIndex_++;
			return created_handle;
		}
/*
		unsigned int i = 0;
		for (; i < table_.size(); i++) {
			if (table_[i].second == nullptr) {
				table_[i].second = object;
				table_[i].first = (table_[i].first + 1) % Handle::maxVersion();
				return Handle(i, table_[i].first);
			}
		}
		extendTable();
		
		table_[i].second = object;
		table_[i].first = (table_[i].first + 1) % Handle::maxVersion();
		return Handle(i, table_[i].first);*/
	}

	void releaseHandle(Handle handle) {
		assert(handle.getIndex() < table_.size() && handle.getVersion() == table_[handle.getIndex()].first && "Invalid handle");
		table_[handle.getIndex()].second = nullptr;
		emptyIndexes_.push_back(handle.getIndex());
	}

	inline bool isHandleValid(Handle handle) {
		return handle.getVersion() == table_[handle.getIndex().first];
	}

	inline T* operator[](const Handle& handle) {
		if (handle.getIndex() < table_.size() && handle.getVersion() == table_[handle.getIndex()].first && "Invalid handle") {
			return (table_[handle.getIndex()].first == handle.getVersion()) ? table_[handle.getIndex()].second : nullptr;
		}
		else {
			return nullptr;
		}
	}

private:
	void extendTable() {
		size_t old_size = table_.size();

		size_t new_size = std::min(old_size + size, static_cast<size_t>(Handle::maxIndex()) ); //unless max index of handle was reach double the table size

		table_.resize(new_size);

		for (unsigned int i = old_size; i < new_size; i++) {
			table_[i] = Entry(Handle::minVersion(), nullptr);
		}
	}

	using Entry = std::pair<unsigned int, T*>;
	std::vector<Entry> table_;
	std::list<unsigned int> emptyIndexes_;
	unsigned int firstEmptyIndex_;
};