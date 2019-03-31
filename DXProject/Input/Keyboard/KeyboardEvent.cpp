#include "KeyboardEvent.h"



KeyboardEvent::KeyboardEvent() : type_(Type::INVALID), keyCode_(0u){
}

KeyboardEvent::KeyboardEvent(Type type, unsigned char key_code) : type_(type), keyCode_(key_code) {
}


KeyboardEvent::~KeyboardEvent() {
}

bool KeyboardEvent::isPressed() {
	return type_ == Type::PRESS;
}

bool KeyboardEvent::isReleased() {
	return type_ == Type::RELEASE;
}

bool KeyboardEvent::isValid() {
	return type_ == Type::INVALID;
}

unsigned char KeyboardEvent::getKeyCode() {
	return keyCode_;
}
