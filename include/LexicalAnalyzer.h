#pragma once
#include "Token.h"
#include <string>
#include <fstream>
#include <regex>
#include <unordered_set>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::ifstream;
using std::ofstream;

class LexicalAnalyzer {
public:
    LexicalAnalyzer(const string& src);
    ~LexicalAnalyzer();
    Token* next_token();
    bool done = false;
private:
    int line = 1;
    ofstream out_tokens;
    ofstream out_errors;
    ifstream handler;
    unordered_set<string> reserved_words;
    unordered_map<char, string> chars;
    void process_remaining_digits(string& token, char& c);
    void process_until_blank(string& token, char& c, bool save_changes = true);
};