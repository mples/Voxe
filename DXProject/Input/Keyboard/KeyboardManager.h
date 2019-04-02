#pragma once
#include <queue>

#include "KeyboardEvent.h"
#include "../../Singleton.h"

//auto& KEBOARD = KeyboardManager::getInstance;

class KeyboardManager : public Singleton<KeyboardManager> {
public:
	KeyboardManager();
	~KeyboardManager();

	bool keyIsPressed(const unsigned char key);

	bool keyBufferIsEmpty();
	bool charBufferIsEmpty();

	KeyboardEvent readKey();
	unsigned char readChar();

	void keyPressed(const unsigned char key);
	void keyReleased(const unsigned char key);
	void keyRepeated(const unsigned char key);
	void charTyped(const unsigned char key);

	void enableAutoRepeatKeys();
	void disableAutoRepeatKeys();
	void enableAutoRepeatChars();
	void disableAutoRepeatChars();

	bool isAutoRepeatKeys();
	bool isAutoRepeatChars();
private:
	bool autoRepeatKeys_ = false;
	bool autoRepeatChars_ = false;
	bool keyStates_[256];
	std::queue<KeyboardEvent> keyBuffer_;
	std::queue<unsigned char> charBuffer_;
};

#define KEYBOARD KeyboardManager::getInstance()