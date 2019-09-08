#pragma once
#include <functional>

#include "Keyboard/KeyboardEvent.h"

enum class AxisType {
	RAW_INPUT_MOUSE_X,
	RAW_INPUT_MOUSE_Y
};

class RawAxis {
public:
	RawAxis(AxisType type, double v = 0.0) : type_(type), value_(v) {};
	~RawAxis() = default;
	RawAxis(const RawAxis& other) = default;
	RawAxis& operator=(const RawAxis& other) = default;

	bool operator==(const RawAxis& other) const {
		return type_ == other.type_;
	}

	AxisType type_;
	double value_;
};

template<>
class std::hash<RawAxis> {
public:
	std::size_t operator()(const RawAxis& axis) const {
		return std::hash<int>()(static_cast<int>(axis.type_));
	}
};