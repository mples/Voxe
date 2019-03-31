#include "MouseEvent.h"



MouseEvent::MouseEvent(): type_(Type::INVALID), x_(0), y_(0) {
}


MouseEvent::~MouseEvent() {
}

MouseEvent::MouseEvent(const Type type, const int x, const int y) : type_(type), x_(x), y_(y) {
}

bool MouseEvent::isValid() {
	return type_ != Type::INVALID;
}

MouseEvent::Type MouseEvent::getType() {
	return type_;
}

MousePoint MouseEvent::getPos() {
	return { x_, y_ };
}

int MouseEvent::getPosX() {
	return x_;
}

int MouseEvent::getPosY() {
	return y_;
}