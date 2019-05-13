#pragma once
#include <limits>
#include <vector>

class Handle {
public:
	Handle(unsigned int index, unsigned int vers) : index_(index), version_(vers) {}

	inline const unsigned int getIndex() const {
		return index_;
	}
	inline const unsigned int getVersion() const {
		return version_;
	}
	static unsigned int maxIndex() {
		return std::numeric_limits<unsigned int>::max();
	}
	static unsigned int minVersion() {
		return std::numeric_limits<unsigned int>::min();
	}
	static unsigned int maxVersion() {
		return std::numeric_limits<unsigned int>::max();
	}
	static unsigned int minIndex() {
		return std::numeric_limits<unsigned int>::min();
	}
private:
	unsigned int index_;
	unsigned int version_;	//storing version counter to make sure handle is valid 
};





template<class T, size_t size = 1024>
class HandleTable {
public:
	HandleTable() {
		extendTable();
	}

	~HandleTable() {}

	Handle<T> acquireHandle(T* object) {
		for (unsigned int i = 0; i < table_.size(); i++) {
			if (table_[i].second == nullptr) {
				table_[i].second = object;
				table_[i].first = (table_[i].first + 1) % Handle::maxVersion();
				return Handle(i, table_[i].first);
			}
		}
		extendTable();
		assert(table[i].second != nullptr); //table is full
		
		table_[i].second = object;
		table_[i].first = (table_[i].first + 1) % Handle::maxVersion();
		return Handle(i, table_[i].first);
	}

	void releaseHandle(Handle handle) {
		assert(handle.getIndex() < table_.size() && handle.getVersion() == table_[handle.getIndex()].first && "Invalid handle");
		table_[handle.getIndex()] = nullptr;
	}

	inline bool isHandleValid(Handle handle) {
		return handle.getVersion() == table_[handle.getIndex().first];
	}

	inline T* operator[](Handle& handle) {
		assert(handle.getIndex() < table_.size() &&  handle.getVersion() == table_[handle.getIndex()].first && "Invalid handle");
		return (table_[handle.getIndex()].first == table_.getVersion()) ? table_[handle.getIndex()].second : nullptr;
	}

private:
	using Entry = std::pair<unsigned int, T*>
	void extendTable() {
		size_t old_size = table_.size();

		size_t new_size = std::min(old_size + size, Handle<T>::MAX_INDEX); //unless max index of handle was reach double the table size

		table_.resize(new_size);

		for (unsigned int i = old_size; i < new_size; i++) {
			table_[i] = Entry(Handle::minVersion(), nullptr));
		}
	}

	std::vector<Entry> table_;
};