#pragma once
#include "Token.h"
#include <string>
#include <fstream>
#include <regex>

class Parser {
public:
    Parser(const std::string& src);
    ~Parser();
    Token next_token();
    Token next_token_regex();
    bool done{ false };
private:
    bool is_blank(char &c);
    int line{ 1 };
    // std::regex RGX{ "([a-zA-Z]([a-zA-Z]|\\d|_)*)|^([1-9]\\d*|0)$)|(^([1-9]\\d*|0).((\\d*[1-9])|0)((e[+|-])?([1-9]\\d*|0))*$)" };
    std::regex RGX{ "[a-zA-Z]([a-zA-Z]|\\d|_)*" };
    std::ifstream handler;
    Token process(const std::string& str);
};