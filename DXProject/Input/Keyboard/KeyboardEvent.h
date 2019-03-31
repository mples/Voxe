#pragma once
class KeyboardEvent {
public:
	enum Type {
		INVALID,
		PRESS,
		RELEASE
	};

	KeyboardEvent();
	KeyboardEvent(Type type, unsigned char key_code);
	~KeyboardEvent();
	bool isPressed();
	bool isReleased();
	bool isValid();
	unsigned char getKeyCode();
private:
	Type type_;
	unsigned char keyCode_;
};

