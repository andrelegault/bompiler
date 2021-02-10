#include "Parser.h"
#include "Utils.h"
#include <sstream>
#include <fstream>
#include "Token.h"
#include <vector>
#include <string>

Parser::Parser(const std::string& src) {
    handler.open(src);
    std::string basename = Utils::get_before_ext(src);
    out_tokens.open(basename + ".outlextokens", std::ios_base::out);
    out_errors.open(basename + ".outlexerrors", std::ios_base::out);

    reserved_words = {
        "if", "then", "else", "integer", "float",
        "string", "void", "public", "private", "func",
        "var", "class", "while", "read", "write", "return",
        "main", "inherits", "break", "continue"
    };

    chars = {
        {'+', "plus"},
        {'-', "minus"},
        {'*', "mult"},
        {'|', "or"},
        {'&', "and"},
        {'!', "not"},
        {'?', "qmark"},
        {'(', "openpar"},
        {')', "closepar"},
        {'{', "opencubr"},
        {'}', "closecubr"},
        {'[', "opensqbr"},
        {']', "closesqbr"},
        {';', "semi"},
        {',', "comma"},
        {'.', "dot"}
    };
}

Parser::~Parser() {
    handler.close();
    out_tokens.close();
    out_errors.close();
}

bool Parser::is_blank(char& c, bool increase_line) {
    if (c == '\n' || c == '\r') { // assumes a \n comes after \r
        if (c == '\r')
            handler.get(c);
        if (increase_line)
            line++;
        return true;
    }
    else return c == ' ' || c == '\t';
}

bool Parser::is_alphanumeric(const int& d) {
    /// is it a letter, digit, or an underscore.
    return is_letter(d) || is_digit(d) || d == 95;
}

bool Parser::is_letter(const int& d) {
    /// is it a letter
    return (d >= 65 && d <= 90) || (d >= 97 && d <= 122);
}

bool Parser::is_digit(const int& d) {
    /// is it a digit
    return is_nonzero(d) || d == 48;
}

bool Parser::is_nonzero(const int& d) {
    return d >= 49 && d <= 57;
}

void Parser::process_remaining_digits(std::string& token, char& c) {
    while (is_digit((int)c)) {
        token += c;
        handler.get(c);
    }
    handler.unget();
}

void Parser::process_until_blank(std::string& token, char& c, bool save_changes) {
    while (!is_blank(c, save_changes)) {
        token += c;
        handler.get(c);
    }
    handler.unget();
}

Token* Parser::next_token() {
    /// holy shit
    Token* t{ nullptr };
    std::string token;
    std::ostringstream str_stream;
    char c;
    handler.get(c);
    while (!done && is_blank(c, true)) {
        std::cout << c;
        out_tokens << c;
        done = handler.eof();
        handler.get(c);
    }
    token += c;
    if (is_letter((int)c)) {
        handler.get(c);
        while (is_alphanumeric((int)c)) {
            token += c;
            handler.get(c);
        }
        handler.unget();
        if (reserved_words.find(token) != reserved_words.end())
            t = new Token(token, token, line);
        else
            t = new Token("id", token, line);
    }
    else if (is_digit((int)c)) { // 0..9
        // either .0 or .digit*nonzero
        if (c == '0') { // digit
            handler.get(c);
            if (c == '.') { // 0.
                token += c;
                handler.get(c);
                if (c == '0') { // 0.0
                    token += c;
                    handler.get(c);
                    if (c == 'e') { // 0.0e
                        token += c;
                        handler.get(c);
                        if (c == '-') { // 0.0e-
                            token += c;
                            handler.get(c);
                            if (c == '0') { // 0.0e-0
                                token += c;
                                t = new ErrorToken("invalidnum", token, line);
                            }
                            else if (is_nonzero((int)c)) { // 0.0e-<1..9>
                                token += c;
                                handler.get(c);
                                process_remaining_digits(token, c);
                                t = new Token("floatnum", token, line);
                            }
                            else { // 0.0e-<not-a-digit>
                                t = new ErrorToken("invalidnum", token, line);
                            }
                        }
                        else if (is_nonzero((int)c)) { // 0.0e<1..9>
                            token += c;
                            handler.get(c);
                            process_remaining_digits(token, c);
                            t = new Token("floatnum", token, line);
                        }
                        else {
                            t = new ErrorToken("invalidnum", token, line);
                        }
                    }
                    else if (is_digit((int)c)) { // 0.0<digit>
                        token += c;
                        handler.get(c);
                        process_remaining_digits(token, c);
                        handler.unget();
                        token.pop_back();
                        handler.get(c);
                        if (is_nonzero((int)c)) { // 0.0<digit>*<nonzero>
                            token += c;
                            handler.get(c);
                            if (c == 'e') { // 0.0e
                                token += c;
                                handler.get(c);
                                if (c == '-') {
                                    token += c;
                                    handler.get(c);
                                    if (c == '0') {
                                        token += c;
                                        t = new ErrorToken("invalidnum", token, line);
                                    }
                                    else if (is_nonzero((int)c)) { // 0.0<digit>*<nonzero>e[+/-]<nonzero><digit>*
                                        token += c;
                                        handler.get(c);
                                        process_remaining_digits(token, c);
                                        t = new Token("floatnum", token, line);
                                    }
                                }
                                else if (is_digit((int)c)) {
                                    token += c;
                                    handler.get(c);
                                    if (c == '0') {
                                        token += c;
                                        t = new Token("floatnum", token, line);
                                    }
                                    else {
                                        process_remaining_digits(token, c);
                                        handler.unget();
                                        handler.get(c);
                                        if (c == '0') {
                                            token.pop_back();
                                            handler.unget();
                                            t = new Token("floatnum", token, line);
                                        }
                                        else {
                                            t = new Token("floatnum", token, line);
                                        }
                                    }
                                }
                            }
                            else { // 0.0<digit>*<nonzero><something-not-e>
                                handler.unget();
                                t = new Token("floatnum", token, line);
                            }
                        } // 0.0<digit>*<!nonzero>
                        else {
                            handler.unget();
                            t = new Token("floatnum", token, line);
                        }
                    }
                }
                else if (is_digit((int)c)) { // 0.<digit>
                    while (is_digit((int)c)) {
                        token += c;
                        handler.get(c);
                    }
                    handler.unget();
                    handler.unget();
                    handler.get(c);
                    if (is_nonzero((int)c)) { // 0.digit*nonzero
                        token += c;
                        handler.get(c);
                        if (c == 'e') { // 0.<digit>*<nonzero>e
                            token += c;
                            handler.get(c);
                            if (c == '-') { // <digits>*<nonzero>.<digit>*<nonzero>e[+/-]
                                token += c;
                                handler.get(c);
                                if (is_nonzero((int)c)) { // 0.<digit>*<nonzero>e[+/-]<integer>
                                    process_remaining_digits(token, c);
                                    t = new Token("floatnum", token, line);
                                }
                                else {
                                    t = new ErrorToken("invalidnum", token, line);
                                }
                            }
                            else if (is_digit((int)c)) {
                                if (c == '0') {
                                    t = new Token("floatnum", token, line);
                                }
                                else if (is_nonzero((int)c)) {
                                    token += c;
                                    handler.get(c);
                                    process_remaining_digits(token, c);
                                    t = new Token("floatnum", token, line);
                                }
                            }
                        }
                        else {
                            handler.unget();
                            t = new Token("floatnum", token, line);
                        }
                    }
                    else {
                        handler.unget();
                        t = new Token("floatnum", token, line);
                    }
                }
                else {
                    process_until_blank(token, c, false);
                    t = new ErrorToken("invalidnum", token, line);
                }
            }
            else {
                handler.unget();
                t = new Token("integer", "0", line);
            }
        }
        else { // 1..9
            // we already know it's a nonzero digit
            handler.get(c);
            process_remaining_digits(token, c);
            handler.get(c);
            if (c == '.') { // [1..9].
                token += c;
                handler.get(c);
                if (is_digit((int)c)) { // [1..9].<digit>
                    token += c;
                    handler.get(c);
                    process_remaining_digits(token, c);
                    token.pop_back();
                    handler.unget();
                    handler.get(c);
                    if (is_nonzero((int)c)) { // [1..9].<digit>*<nonzero>
                        token += c;
                        handler.get(c);
                        if (c == 'e') { // <digits>*nonzero.<digit>*<nonzero>e
                            token += c;
                            handler.get(c);
                            if (c == '-') { // <digits>*<nonzero>.<digit>*<nonzero>e[+/-]
                                token += c;
                                handler.get(c);
                                if (is_nonzero((int)c)) { // 0.<digit>*<nonzero>e[+/-]<integer>
                                    process_remaining_digits(token, c);
                                    t = new Token("floatnum", token, line);
                                }
                                else {
                                    t = new ErrorToken("invalidnum", token, line);
                                }
                            }
                            else if (is_digit((int)c)) {
                                if (c == '0') {
                                    token += c;
                                    t = new Token("floatnum", token, line);
                                }
                                else if (is_nonzero((int)c)) {
                                    token += c;
                                    handler.get(c);
                                    process_remaining_digits(token, c);
                                    t = new Token("floatnum", token, line);
                                }
                            }
                        }
                        else if (c == '0') { // [1..9].<digit>*0
                            process_until_blank(token, c);
                            t = new ErrorToken("invalidnum", token, line);
                        }
                        else { // [1..9].<digit>*<not-0>
                            handler.unget();
                            t = new Token("floatnum", token, line);
                        }
                    }
                    else {
                        process_until_blank(token, c, false);
                        t = new ErrorToken("invalidnum", token, line);
                    }
                }
                else {
                    t = new ErrorToken("invalidnum", token, line);
                }
            }
            else {
                handler.unget();
                t = new Token("integer", token, line);
            }
        }
    }
    else if (c == '=') { // = or ==
        handler.get(c);
        if (c == '=') {
            t = new Token("eq", "==", line);
        }
        else {
            handler.unget();
            t = new Token("assign", "=", line);
        }
    }
    else if (c == '<') { // < or <> or <=
        handler.get(c);
        if (c == '>') {
            t = new Token("noteq", "<>", line);
        }
        else if (c == '=') {
            t = new Token("leq", "<=", line);
        }
        else {
            handler.unget();
            t = new Token("lt", "<", line);
        }
    }
    else if (c == '>') { // > or >=
        handler.get(c);
        if (c == '=') {
            t = new Token("geq", ">=", line);
        }
        else {
            handler.unget();
            t = new Token("gt", ">", line);
        }
    }
    else if (c == '/') {
        int starting_line = line;
        handler.get(c);
        if (c == '/') {
            token += c;
            handler.get(c);
            while (c != '\n' && !done) {
                token += c;
                handler.get(c);
                done = handler.eof();
            }
            handler.unget();
            t = new Token("inlinecmt", token, starting_line);
        }
        else if (c == '*') {
            token += c;
            handler.get(c);
            token += c;
            while (!done && c != '*') {
                handler.get(c);
                if (c == '\n') {
                    line++;
                    token += "\\n";
                }
                else
                    token += c;
                done = handler.eof();
            }
            while (!done && c != '/') {
                handler.get(c);
                if (c == '\n') {
                    line++;
                    token += "\\n";
                }
                else
                    token += c;
                done = handler.eof();
            }
            t = new Token("blockcmt", token, starting_line);
        }
        else {
            handler.unget(); // if this is part of another token, unget
            t = new Token("div", "/", line);
        }
    }
    else if (c == ':') {
        handler.get(c);
        if (c == ':') {
            t = new Token("coloncolon", "::", line);
        }
        else {
            handler.unget();
            t = new Token("colon", ":", line);
        }
    }
    else if (c == '"') {
        handler.get(c);
        if (is_blank(c, false)) { // "(\r|\t|\n|<space>)
            handler.unget();
            t = new Token("qmark", token, line);
        }
        else { // "<something that's not blank>
            while (c != '\n' && c != '"') { // until either end of line or another "
                token += c;
                handler.get(c);
            }
            handler.unget(); // check whatever was the last char
            handler.get(c);
            if (c == '"') { // "<someting>"
                token.erase(0, 1);
                t = new Token("stringlit", token, line);
            }
            else {
                while (token.size() > 1) {
                    token.pop_back();
                    handler.unget();
                }
                handler.unget();
                t = new Token("qmark", token, line);
            }
        }
    }
    else if (chars.find(c) != chars.end()) {
        auto test = chars.find(c);
        std::string temp{ c };
        t = new Token(test->second, temp, line);
    }
    else {
        done = handler.get() == -1;
        handler.unget();
        t = new ErrorToken("invalidchar", token, line);
    }
    if (t == nullptr)
        t = new ErrorToken("invalidword", token, line);

    t->process(out_tokens, out_errors);
    return t;
}