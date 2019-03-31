#pragma once
#include "MouseEvent.h"
#include <queue>

class MouseManager {
public:
	MouseManager();
	~MouseManager();

	void leftButtonPressed(int x, int y);
	void leftButtonReleased(int x, int y);
	void rightButtonPressed(int x, int y);
	void rightButtonReleased(int x, int y);
	void midButtonPressed(int x, int y);
	void midButtonReleased(int x, int y);

	void mouseWheelDown(int x, int y);
	void mouseWheelUp(int x, int y);

	void mouseMoved(int x, int y);
	void mouseMovedRaw(int x, int y);

	bool isLeftButtonDown();
	bool isRightButtonDown();
	bool isMidButtonDown();

	int getPosX();
	int getPosY();
	MousePoint getPos();

	bool isEventBufferEmpty();
	MouseEvent readEvent();
private:
	bool leftButtonDown_ = false;
	bool rightButtonDown_ = false;
	bool midButtonDown_ = false;
	std::queue<MouseEvent> eventBuffer_;
	int x_ = 0;
	int y_ = 0;
};

