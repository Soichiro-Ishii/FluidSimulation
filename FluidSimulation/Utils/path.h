#pragma once
#include<string>
#include<utility>
#include<tuple>

std::pair<std::string, std::string> split(std::string str, size_t firstMaxPos);
std::pair<std::string, std::string> splitDirAndFile(std::string path);
std::pair<std::string, std::string> splitFileAndExt(std::string path);
std::tuple<std::string, std::string, std::string> splitPathAll(std::string path);