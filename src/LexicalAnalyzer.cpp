#include "LexicalAnalyzer.h"
#include "Utils.h"
#include <sstream>
#include <fstream>
#include "Token.h"
#include <vector>
#include <string>

LexicalAnalyzer::LexicalAnalyzer(const std::string& src) {
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

LexicalAnalyzer::~LexicalAnalyzer() {
    handler.close();
    out_tokens.close();
    out_errors.close();
}

void LexicalAnalyzer::process_remaining_digits(std::string& token, char& c) {
    while (!done && Utils::is_digit((int)c)) {
        token += c;
        handler.get(c);
        done = handler.eof();
    }
    handler.unget();
}

void LexicalAnalyzer::process_until_blank(std::string& token, char& c, bool save_changes) {
    while (!done && !Utils::is_blank(c, line, handler, save_changes)) {
        token += c;
        handler.get(c);
        done = handler.eof();
    }
    handler.unget();
}

Token* LexicalAnalyzer::next_token() {
    /// holy shit
    Token* t{ nullptr };
    std::string token;
    std::ostringstream str_stream;
    char c;
    handler.get(c);
    while (!done && Utils::is_blank(c, line, handler, true)) {
        out_tokens << c;
        done = handler.eof();
        handler.get(c);
    }
    token += c;
    if (Utils::is_letter((int)c)) {
        handler.get(c);
        while (!done && Utils::is_alphanumeric((int)c)) {
            token += c;
            handler.get(c);
            done = handler.eof();
        }
        handler.unget();
        if (reserved_words.find(token) != reserved_words.end())
            t = new Token(token, token, line);
        else
            t = new Token("id", token, line);
    }
    else if (Utils::is_digit((int)c)) { // 0..9
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
                                t = new ErrorToken("float", token, line);
                            }
                            else if (Utils::is_nonzero((int)c)) { // 0.0e-<1..9>
                                token += c;
                                handler.get(c);
                                process_remaining_digits(token, c);
                                t = new Token("float", token, line);
                            }
                            else { // 0.0e-<not-a-digit>
                                t = new ErrorToken("float", token, line);
                            }
                        }
                        else if (Utils::is_nonzero((int)c)) { // 0.0e<1..9>
                            token += c;
                            handler.get(c);
                            process_remaining_digits(token, c);
                            t = new Token("float", token, line);
                        }
                        else {
                            t = new ErrorToken("float", token, line);
                        }
                    }
                    else if (Utils::is_digit((int)c)) { // 0.0<digit>
                        token += c;
                        handler.get(c);
                        process_remaining_digits(token, c);
                        handler.unget();
                        token.pop_back();
                        handler.get(c);
                        if (Utils::is_nonzero((int)c)) { // 0.0<digit>*<nonzero>
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
                                        t = new ErrorToken("float", token, line);
                                    }
                                    else if (Utils::is_nonzero((int)c)) { // 0.0<digit>*<nonzero>e[+/-]<nonzero><digit>*
                                        token += c;
                                        handler.get(c);
                                        process_remaining_digits(token, c);
                                        t = new Token("float", token, line);
                                    }
                                }
                                else if (Utils::is_digit((int)c)) {
                                    token += c;
                                    handler.get(c);
                                    if (c == '0') {
                                        token += c;
                                        t = new Token("float", token, line);
                                    }
                                    else {
                                        process_remaining_digits(token, c);
                                        handler.unget();
                                        handler.get(c);
                                        if (c == '0') {
                                            token.pop_back();
                                            handler.unget();
                                            t = new Token("float", token, line);
                                        }
                                        else {
                                            t = new Token("float", token, line);
                                        }
                                    }
                                }
                            }
                            else { // 0.0<digit>*<nonzero><something-not-e>
                                handler.unget();
                                t = new Token("float", token, line);
                            }
                        } // 0.0<digit>*<!nonzero>
                        else {
                            handler.unget();
                            t = new Token("float", token, line);
                        }
                    }
                }
                else if (Utils::is_digit((int)c)) { // 0.<digit>
                    while (!done && Utils::is_digit((int)c)) {
                        token += c;
                        handler.get(c);
                        done = handler.eof();
                    }
                    handler.unget();
                    handler.unget();
                    handler.get(c);
                    if (Utils::is_nonzero((int)c)) { // 0.digit*nonzero
                        token += c;
                        handler.get(c);
                        if (c == 'e') { // 0.<digit>*<nonzero>e
                            token += c;
                            handler.get(c);
                            if (c == '-') { // <digits>*<nonzero>.<digit>*<nonzero>e[+/-]
                                token += c;
                                handler.get(c);
                                if (Utils::is_nonzero((int)c)) { // 0.<digit>*<nonzero>e[+/-]<integer>
                                    process_remaining_digits(token, c);
                                    t = new Token("float", token, line);
                                }
                                else {
                                    t = new ErrorToken("float", token, line);
                                }
                            }
                            else if (Utils::is_digit((int)c)) {
                                if (c == '0') {
                                    t = new Token("float", token, line);
                                }
                                else if (Utils::is_nonzero((int)c)) {
                                    token += c;
                                    handler.get(c);
                                    process_remaining_digits(token, c);
                                    t = new Token("float", token, line);
                                }
                            }
                        }
                        else {
                            handler.unget();
                            t = new Token("float", token, line);
                        }
                    }
                    else {
                        handler.unget();
                        t = new Token("float", token, line);
                    }
                }
                else {
                    process_until_blank(token, c, false);
                    t = new ErrorToken("float", token, line);
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
                if (Utils::is_digit((int)c)) { // [1..9].<digit>
                    token += c;
                    handler.get(c);
                    process_remaining_digits(token, c);
                    token.pop_back();
                    handler.unget();
                    handler.get(c);
                    if (Utils::is_nonzero((int)c)) { // [1..9].<digit>*<nonzero>
                        token += c;
                        handler.get(c);
                        if (c == 'e') { // <digits>*nonzero.<digit>*<nonzero>e
                            token += c;
                            handler.get(c);
                            if (c == '-') { // <digits>*<nonzero>.<digit>*<nonzero>e[+/-]
                                token += c;
                                handler.get(c);
                                if (Utils::is_nonzero((int)c)) { // 0.<digit>*<nonzero>e[+/-]<integer>
                                    process_remaining_digits(token, c);
                                    t = new Token("float", token, line);
                                }
                                else {
                                    t = new ErrorToken("float", token, line);
                                }
                            }
                            else if (Utils::is_digit((int)c)) {
                                if (c == '0') {
                                    token += c;
                                    t = new Token("float", token, line);
                                }
                                else if (Utils::is_nonzero((int)c)) {
                                    token += c;
                                    handler.get(c);
                                    process_remaining_digits(token, c);
                                    t = new Token("float", token, line);
                                }
                            }
                        }
                        else if (c == '0') { // [1..9].<digit>*0
                            process_until_blank(token, c);
                            t = new ErrorToken("float", token, line);
                        }
                        else { // [1..9].<digit>*<not-0>
                            handler.unget();
                            t = new Token("float", token, line);
                        }
                    }
                    else {
                        while (!done && Utils::is_alphanumeric(c)) {
                            token += c;
                            handler.get(c);
                            done = handler.eof();
                        }
                        handler.unget();
                        t = new ErrorToken("float", token, line);
                    }
                }
                else {
                    t = new ErrorToken("float", token, line);
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
        done = handler.eof();
        if (!done && c == '/') {
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
        else if (!done && c == '*') {
            token += c;
            int num_open = 1;
            bool prevent_double_check = false;
            while (!done && num_open > 0) {
                handler.get(c);
                done = handler.eof();
                while (!done && c != '*') {
                    if (c == '\n') {
                        line++;
                        token += "\\n";
                    }
                    else
                        token += c;
                    handler.get(c);
                    done = handler.eof();
                    prevent_double_check = false;
                }
                token += c;
                if (!done) {
                    if (!prevent_double_check && token.at(token.size() - 2) == '/')
                        num_open++;
                    else {
                        handler.get(c);
                        done = handler.eof();
                        if (c == '/') {
                            num_open--;
                            token += c;
                            prevent_double_check = true;
                        }
                        else
                            handler.unget();
                    }
                }
            }
            if (done)
                t = new ErrorToken("blockcmt reached EOF", token, starting_line);
            else
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
        if (Utils::is_blank(c, line, handler, false)) { // "(\r|\t|\n|<space>)
            handler.unget();
            t = new Token("qmark", token, line);
        }
        else { // "<something that's not blank>
            // TODO: match only alphanum or space
            while (c != '"' && (Utils::is_alphanumeric(c) || c == ' ')) { // until either end of line or another "
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
                if (c == '\n')
                    handler.unget();
                t = new ErrorToken("stringlit", token, line);
            }
        }
    }
    else if (chars.find(c) != chars.end()) {
        auto test = chars.find(c);
        std::string temp{ c };
        t = new Token(test->second, temp, line);
    }
    else {
        t = new ErrorToken("char", token, line);
    }
    done = handler.get() == -1;
    handler.unget();
    t->process(out_tokens, out_errors);
    return t;
}