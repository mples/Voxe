#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::log(std::wstring message) {
	std::wstring error_message = L"Error: " + message;
	MessageBoxW(nullptr, error_message.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::log(HRESULT result, std::wstring message) {
	_com_error error(result);
	std::wstring error_message = L"Error: " + message + L"\n" + error.ErrorMessage();
	MessageBoxW(nullptr, error_message.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::log(std::string message) {
	log(StringHelper::stringToWide(message));
}

void ErrorLogger::log(HRESULT result, std::string message) {
	log(result, StringHelper::stringToWide(message));
}

void ErrorLogger::log(COMException & ex) {
	MessageBoxW(nullptr, ex.what(), L"Error", MB_ICONERROR);
}
