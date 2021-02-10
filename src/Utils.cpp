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

std::string Utils::get_before_ext(const std::string& filename) {
    const int dot = filename.rfind('.');
    if (dot != std::string::npos) {
        return filename.substr(0, dot);
    }
    else {
        return filename;
    }
}

bool Utils::is_blank(char& c, int& line, std::ifstream& handler, bool increase_line) {
    if (c == '\n' || c == '\r') { // assumes a \n comes after \r
        if (c == '\r')
            handler.get(c);
        if (increase_line)
            line++;
        return true;
    }
    else return c == ' ' || c == '\t';
}

bool Utils::is_alphanumeric(const int& d) {
    /// is it a letter, digit, or an underscore.
    return is_letter(d) || is_digit(d) || d == 95;
}

bool Utils::is_letter(const int& d) {
    /// is it a letter
    return (d >= 65 && d <= 90) || (d >= 97 && d <= 122);
}

bool Utils::is_digit(const int& d) {
    /// is it a digit
    return is_nonzero(d) || d == 48;
}

bool Utils::is_nonzero(const int& d) {
    return d >= 49 && d <= 57;
}
