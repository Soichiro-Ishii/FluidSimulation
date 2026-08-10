#pragma once
#include<fstream>
#include<string>
#include<vector>
class LoadBinaryFile
{
private:
	std::vector<unsigned char> Data;
	size_t Size;
	bool Success;
public:
	LoadBinaryFile(const std::string& path);
	bool succeeded() const;
	const unsigned char* data() const;
	size_t size() const;
};

