#pragma once
#include <functional>

class KeyboardEvent {
public:
	enum Type {
		INVALID,
		PRESS,
		RELEASE,
		REPEAT_PRESS
	};

	KeyboardEvent();
	KeyboardEvent(Type type, unsigned char key_code);
	~KeyboardEvent();
	bool operator==(const KeyboardEvent& other) const;
	bool isPressed();
	bool isReleased();
	bool isRepeatPress();
	bool isValid();
	unsigned char getKeyCode() const;
private:
	Type type_;
	unsigned char keyCode_;
};

template<>
class std::hash<KeyboardEvent> {
public:
	std::size_t operator()(const KeyboardEvent& button) const {
		return std::hash<int>()(button.getKeyCode());
	}
};