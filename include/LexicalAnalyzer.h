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
    bool done{ false };
private:
    int line{ 1 };
    std::ofstream out_tokens;
    std::ofstream out_errors;
    std::ifstream handler;
    std::unordered_set<std::string> reserved_words;
    std::unordered_map<char, std::string> chars;
    void process_remaining_digits(std::string& token, char& c);
    void process_until_blank(std::string& token, char& c, bool save_changes = true);
};