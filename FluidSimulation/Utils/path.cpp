#include "pch.h"
#include"path.h"

std::pair<std::string, std::string> split(std::string str, size_t firstMaxPos) {
	if (firstMaxPos >= str.size() - 1) {
		return std::pair(str, "\0");
	}
	else {
		std::string first = "";
		std::string second = "";
		for (size_t i = 0; i <= firstMaxPos; i++) {
			first += str[i];
		}
		for (size_t i = firstMaxPos + 1; i < str.size(); i++) {
			second += str[i];
		}
		return std::pair(first, second);
	}
}
std::pair<std::string, std::string> splitDirAndFile(std::string path) {
	auto pos = path.rfind('\\');
	if (pos == std::string::npos) {
		return std::pair("\0", path);
	}
	else {
		return split(path, pos);
	}
}
std::pair<std::string, std::string> splitFileAndExt(std::string path) {
	auto pos = path.rfind('.');
	if (pos == std::string::npos) {
		return std::pair("\0", path);
	}
	else {
		return split(path, pos);
	}
}
std::tuple<std::string, std::string, std::string> splitPathAll(std::string path) {
	auto tmpPair = splitDirAndFile(path);
	std::string first = tmpPair.first;
	tmpPair = splitFileAndExt(tmpPair.second);
	std::string second = tmpPair.first;
	std::string third = tmpPair.second;
	return std::tuple(first, second, third);
}