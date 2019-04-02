#pragma once
#include <functional>

#include "Keyboard/KeyboardEvent.h"

//class RawButton {
//public:
//	RawButton(int key) : key_(key), scancode_(0), action_(0), mods_(0) {}
//	RawButton(KeyboardEvent ke) : key_(ke.getKeyCode()){}
//	~RawButton() = default;
//	RawButton(const RawButton& other) = default;
//	RawButton& operator=(const RawButton& other) = default;
//
//	bool operator==(const RawButton& other) const {
//		return key_ == other.key_;
//		//&& scancode_ == other.scancode_ && action_ == other.action_ && mods_ == other.mods_;
//	}
//
//	unsigned char keyboardKey_;
//	unsigned char mouseKey_;
//	int scancode_;
//	int action_;
//	int mods_;
//
//};
//
//template<>
//class std::hash<RawButton> {
//public:
//	std::size_t operator()(const RawButton& button) const {
//		return std::hash<int>()(button.key_);
//	}
//};

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
		//return std::hash<int>()(axis.value_) ^ std::hash<int>()(static_cast<int>(axis.type_));
		return std::hash<int>()(static_cast<int>(axis.type_));
	}
};