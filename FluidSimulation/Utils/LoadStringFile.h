#pragma once
#include <string>

class LoadStringFile
{
private:
	std::string Data;
	size_t Size;
	bool Success;
public:
	LoadStringFile(const std::string& path);
	bool succeeded() const;
	const char* data() const;
	size_t size() const;
};
