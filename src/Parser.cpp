#include "Parser.h"
#include "Utils.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

Parser::Parser(const std::string& src) {
    handler.open(src);

    reserved_words = {
        "if", "then", "else", "integer", "float",
        "string", "void", "public", "private", "func",
        "var", "class", "while", "read", "write", "return",
        "main", "inherits", "break", "continue"
    };
}

Parser::~Parser() {
    handler.close();
}

bool Parser::is_blank(char& c) {
    if (c == '\n' || c == '\r') { // assumes there's \n comes after \r
        if (c == '\r')
            handler.get(c);
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
    return d >= 65 && d <= 122;
}

bool Parser::is_digit(const int& d) {
    /// is it a digit
    return is_nonzero(d) || d == 48;
}

bool Parser::is_nonzero(const int& d) {
    return d >= 49 && d <= 57;
}

Token Parser::next_token() {
    /// holy shit
    std::string token;
    char c;
    handler.get(c);
    while (!done && is_blank(c)) {
        std::cout << c;
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
            return { token, token, line };
        else
            return { "id", token, line };
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
                                return { "invalidnum", token, line };
                            }
                            else if (is_nonzero((int)c)) { // 0.0e-<1..9>
                                token += c;
                                handler.get(c);
                                while (is_digit((int)c)) {
                                    token += c;
                                    handler.get(c);
                                }
                                handler.unget();
                                return { "floatnum", token, line };
                            }
                            else { // 0.0e-<not-a-digit>
                                return { "invalidnum", token, line };
                            }
                        }
                        else if (is_nonzero((int)c)) { // 0.0<1..9>
                            token += c;
                            handler.get(c);
                            while (is_digit((int)c)) { // 0.0<1..9><digit>
                                token += c;
                                handler.get(c);
                            }
                            handler.unget();
                            return { "floatnum", token, line };
                        }
                    }
                    else if (is_digit((int)c)) { // 0.0<digit>
                        token += c;
                        handler.get(c);
                        while (is_digit((int)c)) { // 0.0<digit>*
                            token += c;
                            handler.get(c);
                        }
                        handler.unget();
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
                                        return { "invalidnum", token, line };
                                    }
                                    else if (is_nonzero((int)c)) { // 0.0<digit>*<nonzero>e[+/-]<nonzero><digit>*
                                        token += c;
                                        handler.get(c);
                                        while (is_digit((int)c)) {
                                            token += c;
                                            handler.get(c);
                                        }
                                        handler.unget();
                                        return { "floatnum", token, line };
                                    }
                                }
                                else if (is_digit((int)c)) {
                                    token += c;
                                    handler.get(c);
                                    if (c == '0') {
                                        token += c;
                                        return { "floatnum", token, line };
                                    }
                                    else {
                                        while (is_digit((int)c)) {
                                            token += c;
                                            handler.get(c);
                                        }
                                        handler.unget();
                                        handler.unget();
                                        handler.get(c);
                                        if (c == '0') {
                                            token.pop_back();
                                            handler.unget();
                                            return { "floatnum", token, line };
                                        }
                                        else {
                                            return { "floatnum", token, line };
                                        }
                                    }
                                }
                            }
                            else { // 0.0<digit>*<nonzero><something-not-e>
                                handler.unget();
                                return { "floatnum", token, line };
                            }
                        }
                        else {
                            handler.unget();
                            return { "floatnum", token, line };
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
                                    while (is_digit((int)c)) {
                                        token += c;
                                        handler.get(c);
                                    }
                                    handler.unget();
                                    return { "floatnum", token, line };
                                }
                                else {
                                    return { "invalidnum", token, line };
                                }
                            }
                            else if (is_digit((int)c)) {
                                if (c == '0') {
                                    return { "floatnum", token, line };
                                }
                                else if (is_nonzero((int)c)) {
                                    token += c;
                                    handler.get(c);
                                    while (is_digit((int)c)) {
                                        token += c;
                                        handler.get(c);
                                    }
                                    handler.unget();
                                    return { "floatnum", token, line };
                                }
                            }
                        }
                        else {
                            handler.unget();
                            return { "floatnum", token, line };
                        }
                    }
                    else {
                        handler.unget();
                        return { "floatnum", token, line };
                    }
                }
            }
            else {
                handler.unget();
                return { "integer", "0", line };
            }
        }
        else {
            // we already know it's a nonzero digit
            handler.get(c);
            while (is_digit((int)c)) {
                token += c;
                handler.get(c);
            }
            handler.unget();
            handler.get(c);
            if (c == '.') { // <integer>
                token += c;
                handler.get(c);
                if (is_digit((int)c)) {
                    token += c;
                    handler.get(c);
                    while (is_digit((int)c)) {
                        token += c;
                        handler.get(c);
                    }
                    handler.unget();
                    token.pop_back();
                    handler.unget();
                    handler.get(c);
                    if (is_nonzero((int)c)) {
                        token += c;
                        handler.get(c);
                        if (c == 'e') { // <digits>*nonzero.<digit>*<nonzero>e
                            token += c;
                            handler.get(c);
                            if (c == '-') { // <digits>*<nonzero>.<digit>*<nonzero>e[+/-]
                                token += c;
                                handler.get(c);
                                if (is_nonzero((int)c)) { // 0.<digit>*<nonzero>e[+/-]<integer>
                                    while (is_digit((int)c)) {
                                        token += c;
                                        handler.get(c);
                                    }
                                    handler.unget();
                                    return { "floatnum", token, line };
                                }
                                else {
                                    return { "invalidnum", token, line };
                                }
                            }
                            else if (is_digit((int)c)) {
                                if (c == '0') {
                                    return { "floatnum", token, line };
                                }
                                else if (is_nonzero((int)c)) {
                                    token += c;
                                    handler.get(c);
                                    while (is_digit((int)c)) {
                                        token += c;
                                        handler.get(c);
                                    }
                                    handler.unget();
                                    return { "floatnum", token, line };
                                }
                            }
                        }
                        else {
                            while (!is_blank(c)) {
                                token += c;
                                handler.get(c);
                            }
                            return { "invalidnumok", token, line };
                        }
                    }
                    else {
                        while (!is_blank(c)) {
                            token += c;
                            handler.get(c);
                        }
                        return { "invalidnumtest", token, line };
                    }
                }
                else {
                    while (!is_blank(c)) {
                        token += c;
                        handler.get(c);
                    }
                    handler.unget();
                    return { "invalidnum", token, line };
                }
            }
        }
    }
    else if (c == '=') { // = or ==
        handler.get(c);
        done = handler.eof();
        if (c == '=') {
            done = handler.eof();
            return { "eq", "==", line };
        }
        else {
            handler.unget();
            done = handler.eof();
            return { "assign", "=", line };
        }
    }
    else if (c == '<') { // < or <> or <=
        handler.get(c);
        done = handler.eof();
        if (c == '>') {
            return { "noteq", "<>", line };
        }
        else if (c == '=') {
            return { "leq", "<=", line };
        }
        else {
            handler.unget();
            done = handler.eof();
            return { "lt", "<", line };
        }
    }
    else if (c == '>') { // > or >=
        handler.get(c);
        done = handler.eof();
        if (c == '=') {
            return { "geq", ">=", line };
        }
        else {
            handler.unget();
            done = handler.eof();
            return { "gt", ">", line };
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
            line++;
            return { "inlinecmt", token, starting_line };
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
            token += c;
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
            return { "blockcmt", token, starting_line };
        }
        else {
            handler.unget(); // if this is part of another token, unget
            return { "div", "/", line };
        }
    }
    else if (c == ':') {
        handler.get(c);
        if (c == ':') {
            return { "coloncolon", "::", line };
        }
        else {
            handler.unget();
            return { "colon", ":", line };
        }
    }
    // maybe refactor to use a hashmap from char to string to remove those below
    else if (c == '+') {
        return { "plus", "+", line };
    }
    else if (c == '-') {
        return { "minus", "-", line };
    }
    else if (c == '*') { // this is for sure a mult, otherwise comment block check failed
        return { "mult", "*", line };
    }
    else if (c == '|') {
        return { "or", "|", line };
    }
    else if (c == '&') {
        return { "and", "&", line };
    }
    else if (c == '!') {
        return { "not", "!", line };
    }
    else if (c == '?') {
        return { "qmark", "?", line };
    }
    else if (c == '(') {
        return { "openpar", "(", line };
    }
    else if (c == ')') {
        return { "closepar", ")", line };
    }
    else if (c == '{') {
        return { "opencubr", "{", line };
    }
    else if (c == '}') {
        return { "closecubr", "}", line };
    }
    else if (c == '[') {
        return { "opensqbr", "[", line };
    }
    else if (c == ']') {
        return { "closesqbr", "]", line };
    }
    else if (c == ';') {
        return { "semi", ";", line };
    }
    else if (c == ',') {
        return { "comma", ",", line };
    }
    else if (c == '"') {
        return { "comma", ",", line };
    }
    done = handler.get() == -1;
    return { "invalidword", token, line };
}

Token Parser::next_token_regex() {
    // TODO: skip comments (keep in mind multi-line comments)
    std::string current;
    // skip empty lines
    while (std::getline(handler, current)) {
        std::cout << current << std::endl;
        if (current == "\n" || (current.size() >= 2 && current.substr(0, 2) == "//")) {
            ++line;
            continue;
        }
        else {
            // assuming the start is at 0
            int comment_start = current.find("/*");
            if (comment_start != std::string::npos) {
                int start_line_count = line;
                std::stringstream container;
                container << current;
                while (current.find("*/") == std::string::npos) {
                    std::cout << "ye" << std::endl;
                    if (std::getline(handler, current)) {
                        container << current;
                        ++line;
                    }
                    else {
                        done = handler.eof();
                        break;
                    }
                }
                return { "blockcmt", container.str(), start_line_count };
            }
            else {
                // this wont support if there's more than one word in a line
                current.pop_back();
                std::vector<std::string> words = Utils::split_string(current, " ");
                std::smatch matches;
                if (std::regex_match(current, matches, RGX)) {
                    for (auto match : matches) {
                        std::cout << match.str() << std::endl;
                    }
                    std::cout << line << std::endl;
                    return { "integernum", matches[0].str(), line++ };
                }
            }
            return { "integernum", "1", 0 };
        }
    }
    return { "integernum", "2", 0 };
}

Token process(const std::string& str) {
    return { "integernum", "0", 0 };
}