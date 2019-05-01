#include "StringHelper.h"
#include <algorithm>

std::wstring StringHelper::stringToWide(std::string str) {
	return std::wstring(str.begin(), str.end());
}

std::wstring StringHelper::getDirectoryFromPath(const std::wstring & path) {
	size_t off_back_slashes = path.find_last_of(L"\\");
	size_t off_forward_slash = path.find_last_of(L"/");

	if (off_back_slashes == std::string::npos && off_forward_slash == std::string::npos) {
		return {};
	}
	else if (off_back_slashes == std::string::npos) {
		return path.substr(0, off_forward_slash);
	}
	else if (off_forward_slash == std::string::npos) {
		return path.substr(0, off_back_slashes);
	}
	return path.substr(0, std::max(off_back_slashes, off_forward_slash));
}

std::wstring StringHelper::getFileExtension(const std::wstring & path) {
	size_t off = path.find_last_of(L".");
	if (off == std::string::npos) {
		return {};
	}
	return std::wstring(path.substr(off + 1));
}

std::string StringHelper::getDirectoryFromPath(const std::string & path) {
	size_t off_back_slashes = path.find_last_of("\\");
	size_t off_forward_slash = path.find_last_of("/");

	if (off_back_slashes == std::string::npos && off_forward_slash == std::string::npos) {
		return {};
	}
	else if (off_back_slashes == std::string::npos) {
		return path.substr(0, off_forward_slash);
	}
	else if (off_forward_slash == std::string::npos) {
		return path.substr(0, off_back_slashes);
	}
	return path.substr(0, std::max(off_back_slashes, off_forward_slash));
}

std::string StringHelper::getFileExtension(const std::string & path) {
	size_t off = path.find_last_of(".");
	if (off == std::string::npos) {
		return {};
	}
	return std::string(path.substr(off + 1));
}
