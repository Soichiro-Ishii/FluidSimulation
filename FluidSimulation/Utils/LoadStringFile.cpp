#include "LoadStringFile.h"
#include <fstream>

LoadStringFile::LoadStringFile(const std::string& path)
	: Success(true), Size(0) {
	std::ifstream ifs(path);
	if (!ifs) {
		Success = false;
		return;
	}
	// 読込サイズを調べる
	ifs.seekg(0, std::ios::end);
	auto pos = ifs.tellg();
	if (pos == -1) {
		// tellg failed
		Success = false;
		return;
	}
	Size = static_cast<size_t>(pos);
	ifs.seekg(0);
	Data.resize(Size);
	if (Size > 0)
		ifs.read(&Data[0], Size);
}

bool LoadStringFile::succeeded() const {
	return Success;
}
const char* LoadStringFile::data() const {
	return Data.c_str();
}
size_t LoadStringFile::size() const {
	return Size;
}
