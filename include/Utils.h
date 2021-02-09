#pragma once
#include <vector>
#include <string>

namespace Utils {
    std::vector<std::string> split_string(const std::string& str, const std::string &delim);
    std::string get_before_ext(const std::string &filename);
}