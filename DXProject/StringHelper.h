#pragma once
#include<string>

class StringHelper {
public:
	static std::wstring stringToWide(std::string str);
	static std::wstring getDirectoryFromPath(const std::wstring & path);
	static std::wstring getFileExtension(const std::wstring & path);
	static std::string getDirectoryFromPath(const std::string & path);
	static std::string getFileExtension(const std::string & path);
};

