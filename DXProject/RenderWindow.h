#pragma once
#include "Utilities/ErrorLogger.h"

class WindowContainer;

class RenderWindow {
public:
	RenderWindow();
	~RenderWindow();

	HWND getHWND();
	bool initialize(WindowContainer* window_container, HINSTANCE instance, std::wstring title, std::wstring class_name, int width, int height);
	bool procesMessages();

private:
	void registerWindowClass();

	HWND windowHandle_;
	HINSTANCE instanceHandle_;

	std::wstring windowTitle_;
	std::wstring windowClassName_;
	int width_;
	int height_;
};

