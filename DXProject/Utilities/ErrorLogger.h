#pragma once
#include <Windows.h>
#include "StringHelper.h"
#include "ComException.h"

class ErrorLogger {
public:
	static void log(std::wstring message);
	static void log(HRESULT result, std::wstring message);
	static void log(std::string message);
	static void log(HRESULT result, std::string message);
	static void log(COMException& ex);
};

