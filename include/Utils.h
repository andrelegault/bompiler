#pragma once
#include <vector>
#include <string>
#include <fstream>

namespace Utils {
    std::vector<std::string> split_string(const std::string& str, const std::string& delim);
    std::string get_before_ext(const std::string& filename);
    bool is_blank(char& c, int& line, std::ifstream& handler, bool increase_line);
    bool is_alphanum_or_underscore(const int& d);
    bool is_letter(const int& d);
    bool is_nonzero(const int& d);
}