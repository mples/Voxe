#pragma once
#include <comdef.h>
#include <string>

#define COM_ERROR_IF_FAILED( hr, msg) if( FAILED(hr)) throw COMException(hr, msg, __FILEW__, __FUNCTIONW__, __LINE__);
class COMException {
public:
	COMException(HRESULT hr, const std::wstring& msg, const std::wstring& file, const std::wstring& func, int line) {
		_com_error error(hr);
		msg_ = L"Msg: " + msg + L"\n";
		msg_ += error.ErrorMessage();
		msg_ += L"\nFunction: " + func;
		msg_ += L"\nFile: " + file;
		msg_ += L"\nLine: " + std::to_wstring(line);
	}

	const wchar_t * what() const{
		return msg_.c_str();
	}

private:
	std::wstring msg_;
};
