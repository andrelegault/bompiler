#include "Token.h"

Token::Token(const std::string& type, const std::string& lexeme, const int& line) : type(type), lexeme(lexeme), line(line) { }

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "[" << token.type << ", " << token.lexeme << ", " << token.line << "]";
    return os;
}

void Token::process(std::ofstream& out_tokens, std::ofstream& out_errors) const {
    out_tokens << *this;
}

ErrorToken::ErrorToken(const std::string& type, const std::string& lexeme, const int& line) : Token(type, lexeme, line) {}

std::ostream& operator<<(std::ostream& os, const ErrorToken& token) {
    os << "[invalid" << token.type << ", " << token.lexeme << ", " << token.line << "]";
    return os;
}

void ErrorToken::process(std::ofstream& out_tokens, std::ofstream& out_errors) const {
    out_errors << "Lexical error: Invalid " + type + ": \"" + lexeme + "\": line " + std::to_string(line) << std::endl;
    out_tokens << *this;
}