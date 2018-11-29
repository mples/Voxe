#pragma once
#include <functional>

class RawButton {
public:
	RawButton(int key) : key_(key), scancode_(0), action_(0), mods_(0) {}
	RawButton(int key, int scancode, int action, int mods) : key_(key), scancode_(scancode), action_(action), mods_(mods) {}
	~RawButton() = default;
	RawButton(const RawButton& other) = default;
	RawButton& operator=(const RawButton& other) = default;

	bool operator==(const RawButton& other) const {
		return key_ == other.key_;
		//&& scancode_ == other.scancode_ && action_ == other.action_ && mods_ == other.mods_;
	}

	int key_;
	int scancode_;
	int action_;
	int mods_;

};

class RawAxis {
public:
	RawAxis() = default;
	RawAxis(int xpos, int ypos) : mouseX_(xpos), mouseY_(ypos) {};
	~RawAxis() = default;
	RawAxis(const RawAxis& other) = default;
	RawAxis& operator=(const RawAxis& other) = default;

	bool operator==(const RawAxis& other) const {
		return mouseX_ == other.mouseX_ && mouseY_ == other.mouseY_;
	}

	int mouseX_;
	int mouseY_;
};

template<>
class std::hash<RawButton> {
public:
	std::size_t operator()(const RawButton& button) const {
		return std::hash<int>()(button.key_);
	}
};

template<>
class std::hash<RawAxis> {
public:
	std::size_t operator()(const RawAxis& axis) const {
		return std::hash<int>()(axis.mouseX_) ^ std::hash<int>()(axis.mouseY_);
	}
};