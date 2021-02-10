#pragma once
#include "Token.h"
#include <string>
#include <fstream>
#include <regex>
#include <unordered_set>
#include <unordered_map>

class LexicalAnalyzer {
public:
    LexicalAnalyzer(const std::string& src);
    ~LexicalAnalyzer();
    Token* next_token();
    Token next_token_regex();
    bool done{ false };
private:
    bool is_blank(char& c, bool increase_line);
    int line{ 1 };
    std::ofstream out_tokens;
    std::ofstream out_errors;
    // std::regex RGX{ "([a-zA-Z]([a-zA-Z]|\\d|_)*)|^([1-9]\\d*|0)$)|(^([1-9]\\d*|0).((\\d*[1-9])|0)((e[+|-])?([1-9]\\d*|0))*$)" };
    std::regex RGX{ "[a-zA-Z]([a-zA-Z]|\\d|_)*" };
    std::ifstream handler;
    std::unordered_set<std::string> reserved_words;
    std::unordered_map<char, std::string> chars;
    bool is_alphanumeric(const int& d);
    bool is_letter(const int& d);
    bool is_digit(const int& d);
    bool is_nonzero(const int& d);
    void process_remaining_digits(std::string& token, char& c);
    void process_until_blank(std::string& token, char& c, bool save_changes = true);
};