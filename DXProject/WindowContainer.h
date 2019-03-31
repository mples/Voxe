#pragma once
#include "RenderWindow.h"
#include "Input/Keyboard/KeyboardManager.h"
#include "Input/Mouse/MouseManager.h"

class WindowContainer {
public:
	WindowContainer();
	~WindowContainer();
	LRESULT WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	RenderWindow renderWindow_;
	KeyboardManager keyboardManager_;
	MouseManager mouseManager_;
private:
};

