#include "KeyboardEvent.h"



KeyboardEvent::KeyboardEvent() : type_(Type::INVALID), keyCode_(0u){
}

KeyboardEvent::KeyboardEvent(Type type, unsigned char key_code) : type_(type), keyCode_(key_code) {
}


KeyboardEvent::~KeyboardEvent() {
}

bool KeyboardEvent::operator==(const KeyboardEvent & other) const {
	return (keyCode_ == other.keyCode_ && type_ == other.type_);
}

bool KeyboardEvent::isPressed() {
	return type_ == Type::PRESS;
}

bool KeyboardEvent::isReleased() {
	return type_ == Type::RELEASE;
}

bool KeyboardEvent::isRepeatPress() {
	return type_ == Type::REPEAT_PRESS;
}

bool KeyboardEvent::isValid() {
	return type_ == Type::INVALID;
}

unsigned char KeyboardEvent::getKeyCode() const {
	return keyCode_;
}
