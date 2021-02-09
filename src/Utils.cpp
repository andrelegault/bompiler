#include "Utils.h"

// from https://stackoverflow.com/a/57346888/7705786
std::vector<std::string> Utils::split_string(const std::string& i_str, const std::string& i_delim) {
    std::vector<std::string> result;

    size_t found = i_str.find(i_delim);
    size_t startIndex = 0;

    while (found != std::string::npos) {
        result.push_back(std::string(i_str.begin() + startIndex, i_str.begin() + found));
        startIndex = found + i_delim.size();
        found = i_str.find(i_delim, startIndex);
    }
    if (startIndex != i_str.size())
        result.push_back(std::string(i_str.begin() + startIndex, i_str.end()));
    return result;
}

std::string Utils::get_before_ext(const std::string &filename) {
    const int dot = filename.rfind('.');
    if (dot != std::string::npos) {
        return filename.substr(0, dot);
    } else {
        return filename;
    }
}