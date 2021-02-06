#pragma once
#include <string>
#include <iostream>

class Token {
public:
    Token(const std::string& type, const std::string& lexeme, const int& line);
    friend std::ostream& operator<<(std::ostream&, const Token& token);
private:
    const std::string type;
    const std::string lexeme;
    const int line;
};

std::ostream& operator<<(std::ostream& os, const Token& token);