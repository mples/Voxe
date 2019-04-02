#include "KeyboardManager.h"



KeyboardManager::KeyboardManager() {
	for (int i = 0; i < 256; i++) {
		keyStates_[i] = false;
	}
}


KeyboardManager::~KeyboardManager() {
}

bool KeyboardManager::keyIsPressed(const unsigned char key) {
	return keyStates_[key];
}

bool KeyboardManager::keyBufferIsEmpty() {
	return keyBuffer_.empty();
}

bool KeyboardManager::charBufferIsEmpty() {
	return charBuffer_.empty();
}

KeyboardEvent KeyboardManager::readKey() {
	if (keyBuffer_.empty()) {
		return KeyboardEvent();
	}
	else {
		KeyboardEvent e = keyBuffer_.front();
		keyBuffer_.pop();
		return e;
	}
}

unsigned char KeyboardManager::readChar() {
	if (charBuffer_.empty()) {
		return 0u;
	}
	else {
		unsigned char c = charBuffer_.front();
		charBuffer_.pop();
		return c;
	}
}

void KeyboardManager::keyPressed(const unsigned char key) {
	keyStates_[key] = true;
	keyBuffer_.push(KeyboardEvent(KeyboardEvent::Type::PRESS, key));
}

void KeyboardManager::keyReleased(const unsigned char key) {
	keyStates_[key] = false;
	keyBuffer_.push(KeyboardEvent(KeyboardEvent::Type::RELEASE, key));
}

void KeyboardManager::keyRepeated(const unsigned char key) {
	keyBuffer_.push(KeyboardEvent(KeyboardEvent::Type::REPEAT_PRESS, key));
}

void KeyboardManager::charTyped(const unsigned char key) {
	charBuffer_.push(key);
}

void KeyboardManager::enableAutoRepeatKeys() {
	autoRepeatKeys_ = true;
}

void KeyboardManager::disableAutoRepeatKeys() {
	autoRepeatKeys_ = false;
}

void KeyboardManager::enableAutoRepeatChars() {
	autoRepeatChars_ = true;
}

void KeyboardManager::disableAutoRepeatChars() {
	autoRepeatChars_ = false;
}

bool KeyboardManager::isAutoRepeatKeys() {
	return autoRepeatKeys_;
}

bool KeyboardManager::isAutoRepeatChars() {
	return autoRepeatChars_;
}
