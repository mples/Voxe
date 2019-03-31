#include "WindowContainer.h"



RenderWindow::RenderWindow() : windowHandle_(nullptr), instanceHandle_(nullptr), windowTitle_(L""), windowClassName_(L""), height_(0), width_(0) {
}


RenderWindow::~RenderWindow() {
	if (windowHandle_ != nullptr) {
		UnregisterClass(windowClassName_.c_str(), instanceHandle_);
		DestroyWindow(windowHandle_);
	}
}

HWND RenderWindow::getHWND() {
	return windowHandle_;
}

bool RenderWindow::initialize(WindowContainer* window_container, HINSTANCE instance, std::wstring title, std::wstring class_name, int width, int height) {
	instanceHandle_ = instance;
	width_ = width;
	height_ = height;
	windowTitle_ = title;
	windowClassName_ = class_name;

	registerWindowClass();

	int screen_center_x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	int screen_center_y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	RECT rec;
	rec.left = screen_center_x;
	rec.top = screen_center_y;
	rec.bottom = rec.top + height;
	rec.right = rec.left + width;
	AdjustWindowRect(&rec, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

	windowHandle_ = CreateWindowEx(0, 
		windowClassName_.c_str(), 
		windowTitle_.c_str(), 
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 
		rec.left, 
		rec.top, 
		rec.right - rec.left, 
		rec.bottom - rec.top, 
		NULL, 
		NULL, 
		instanceHandle_,
		window_container);

	if (windowHandle_ == NULL) {
		ErrorLogger::log(GetLastError(), L"CreateWindowEx falied." + windowTitle_);
		return false;
	}

	ShowWindow(windowHandle_, SW_SHOW);
	SetForegroundWindow(windowHandle_);
	SetFocus(windowHandle_);

	return true;
}


bool RenderWindow::procesMessages() {

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (PeekMessage(&msg, windowHandle_, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL) {
		if (!IsWindow(this->windowHandle_)) {
			this->windowHandle_ = NULL; //Message processing loop takes care of destroying this window
			UnregisterClass(this->windowClassName_.c_str(), this->instanceHandle_);
			return false;
		}
	}
	return true;
}

LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	default:
	{
		WindowContainer * const w_container = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return w_container->WinProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const p_create = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* w_container = reinterpret_cast<WindowContainer*>(p_create->lpCreateParams);
		if (w_container == nullptr) {
			ErrorLogger::log(L"Error: Pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(w_container));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));
		return w_container->WinProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
void RenderWindow::registerWindowClass() {
	WNDCLASSEX win_class;

	win_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	win_class.lpfnWndProc = HandleMessageSetup;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = instanceHandle_;
	win_class.hIcon = LoadIcon(0, IDI_APPLICATION);
	win_class.hIconSm = NULL;
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.hbrBackground = NULL;
	win_class.lpszMenuName = NULL;
	win_class.lpszClassName = windowClassName_.c_str();
	win_class.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&win_class)) {
		ErrorLogger::log(GetLastError(), L"RegisterClassEx falied.");
	}
}
