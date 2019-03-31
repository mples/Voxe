#include "MouseManager.h"



MouseManager::MouseManager() {
}


MouseManager::~MouseManager() {
}

void MouseManager::leftButtonPressed(int x, int y) {
	leftButtonDown_ = true;
	eventBuffer_.push(MouseEvent(MouseEvent::Type::LMB_PRESS, x, y));
}

void MouseManager::leftButtonReleased(int x, int y) {
	leftButtonDown_ = false;
	eventBuffer_.push(MouseEvent(MouseEvent::Type::LMP_RELEASE, x, y));
}

void MouseManager::rightButtonPressed(int x, int y) {
	rightButtonDown_ = true;
	eventBuffer_.push(MouseEvent(MouseEvent::Type::RMB_PRESS, x, y));
}

void MouseManager::rightButtonReleased(int x, int y) {
	rightButtonDown_ = false;
	eventBuffer_.push(MouseEvent(MouseEvent::Type::RMB_RELEASE, x, y));
}

void MouseManager::midButtonPressed(int x, int y) {
	leftButtonDown_ = true;
	eventBuffer_.push(MouseEvent(MouseEvent::Type::MMB_PRESS, x, y));
}

void MouseManager::midButtonReleased(int x, int y) {
	leftButtonDown_ = false;
	eventBuffer_.push(MouseEvent(MouseEvent::Type::MMB_RELEASE, x, y));
}

void MouseManager::mouseWheelDown(int x, int y) {
	eventBuffer_.push(MouseEvent(MouseEvent::Type::MWHEEL_DOWN, x, y));
}

void MouseManager::mouseWheelUp(int x, int y) {
	eventBuffer_.push(MouseEvent(MouseEvent::Type::MWHEEL_UP, x, y));
}

void MouseManager::mouseMoved(int x, int y) {
	eventBuffer_.push(MouseEvent(MouseEvent::Type::MOVE, x, y));
}

void MouseManager::mouseMovedRaw(int x, int y) {
	eventBuffer_.push(MouseEvent(MouseEvent::Type::RAW_MOVE, x, y));
}

bool MouseManager::isLeftButtonDown() {
	return leftButtonDown_;
}

bool MouseManager::isRightButtonDown() {
	return rightButtonDown_;
}

bool MouseManager::isMidButtonDown() {
	return midButtonDown_;
}

int MouseManager::getPosX() {
	return x_;
}

int MouseManager::getPosY() {
	return y_;
}

MousePoint MouseManager::getPos() {
	return {x_, y_};
}

bool MouseManager::isEventBufferEmpty() {
	return eventBuffer_.empty();
}

MouseEvent MouseManager::readEvent() {
	if (eventBuffer_.empty()) {
		return MouseEvent();
	}
	else {
		MouseEvent ev = eventBuffer_.front();
		eventBuffer_.pop();
		return ev;
	}
}
