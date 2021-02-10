#pragma once
#include <string>
#include <fstream>
#include <iostream>

class Token {
public:
    Token(const std::string& type = "undefined", const std::string& lexeme = "undefined", const int& line = -1);
    friend std::ostream& operator<<(std::ostream&, const Token& token);
    virtual void process(std::ofstream& out_tokens, std::ofstream& out_errors) const;
protected:
    const std::string type;
    const std::string lexeme;
    const int line;
};

class ErrorToken : public Token {
public:
    ErrorToken(const std::string& type = "undefined", const std::string& lexeme = "undefined", const int& line = -1);
    void process(std::ofstream& out_tokens, std::ofstream& out_errors) const override;
    friend std::ostream& operator<<(std::ostream&, const ErrorToken& token);
};

std::ostream& operator<<(std::ostream&, const ErrorToken& token);
std::ostream& operator<<(std::ostream& os, const Token& token);