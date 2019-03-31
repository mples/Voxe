#include "WindowContainer.h"



WindowContainer::WindowContainer() {
	static bool raw_input_initialized = false;
	if (raw_input_initialized == false) {
		RAWINPUTDEVICE rid;

		rid.usUsage = 0x02;
		rid.usUsagePage = 0x01;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
			ErrorLogger::log(GetLastError(), L"Error: Falied to register raw input device.");
			exit(-1);
		}
		raw_input_initialized = true;
	}
}


WindowContainer::~WindowContainer() {
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WindowContainer::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam)) {
		return true;
	}
	switch (uMsg) {
	//keyboard messages
	case WM_KEYDOWN:
	{
		unsigned char key_code = static_cast<unsigned char>(wParam);
		if (keyboardManager_.isAutoRepeatKeys()) {
			keyboardManager_.keyPressed(key_code);
		}
		else {
			const bool is_repeat = lParam & 0x4000000; // getting bit number 30 in lParam to check is key has already been pressed
			if (!is_repeat) {
				keyboardManager_.keyPressed(key_code);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char key_code = static_cast<unsigned char>(wParam);
		keyboardManager_.keyReleased(key_code);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char key_char = static_cast<unsigned char>(wParam);
		if (keyboardManager_.isAutoRepeatChars()) {
			keyboardManager_.charTyped(key_char);
		}
		else {
			const bool is_repeat = lParam & 0x4000000; // getting bit number 30 in lParam to check is key has already been pressed
			if (!is_repeat) {
				keyboardManager_.charTyped(key_char);
			}
		}
		return 0;
	}
	//mouse messages
	case WM_MOUSEMOVE:
	{
		mouseManager_.mouseMoved(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		mouseManager_.leftButtonPressed(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_LBUTTONUP:
	{
		mouseManager_.leftButtonReleased(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		mouseManager_.rightButtonPressed(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_RBUTTONUP:
	{
		mouseManager_.rightButtonReleased(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		mouseManager_.midButtonPressed(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_MBUTTONUP:
	{
		mouseManager_.midButtonReleased(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			mouseManager_.mouseWheelUp(LOWORD(lParam), HIWORD(lParam));
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
			mouseManager_.mouseWheelDown(LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	}
	case WM_INPUT:
	{
		UINT data_size;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &data_size, sizeof(RAWINPUTHEADER));

		if (data_size > 0) {
			std::unique_ptr<BYTE[]>  raw_data = std::make_unique<BYTE[]>(data_size);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, raw_data.get(), &data_size, sizeof(RAWINPUTHEADER)) == data_size) {
				RAWINPUT* rin = reinterpret_cast<RAWINPUT*>(raw_data.get());
				if (rin->header.dwType == RIM_TYPEMOUSE) {
					mouseManager_.mouseMovedRaw(rin->data.mouse.lLastX, rin->data.mouse.lLastY);
				}
			}

		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
