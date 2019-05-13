#pragma once

using Id_t = unsigned int;

template<class T>
class TypeId {
	static  Id_t count_;

public:
	template<class T>
	static const Id_t get() {
		static const Id_t STATIC_TYPE_ID{ count_++ };
		return STATIC_TYPE_ID;
	}

	static const Id_t get() {
		return count_;
	}
};