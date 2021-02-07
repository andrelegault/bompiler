#include "Token.h"

Token::Token(const std::string& type, const std::string& lexeme, const int& line) : type(type), lexeme(lexeme), line(line) { }

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "[" << token.type << ", " << token.lexeme << ", " << token.line << "]";
    return os;
}