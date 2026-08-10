#include "LoadBinaryFile.h"
LoadBinaryFile::LoadBinaryFile(const std::string& path)
	:Success(true), Size(0) {
	std::ifstream ifs(path, std::ios::binary);
	if (!ifs) {
		Success = false;
		return;
	}
	//読込サイズを調べる。
	ifs.seekg(0, std::ios::end);
	Size = ifs.tellg();
	ifs.seekg(0);
	//読み込んだデータをchar型に出力する
	Data.resize(Size);
	ifs.read(reinterpret_cast<char*>(Data.data()), Size);
}
bool LoadBinaryFile::succeeded() const {
	return Success;
}
const unsigned char* LoadBinaryFile::data() const {
	return Data.data();
}
size_t LoadBinaryFile::size() const {
	return Size;
}