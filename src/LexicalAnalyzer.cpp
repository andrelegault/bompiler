#include "LexicalAnalyzer.h"
#include "Token.h"
#include "Utils.h"
#include <locale>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using std::string;
using std::ostringstream;
using std::ios_base;

LexicalAnalyzer::LexicalAnalyzer(const string& src) {
    handler.open(src, ios_base::in);
    string basename = Utils::get_before_ext(src);
    out_tokens.open(basename + ".outlextokens", ios_base::out);
    out_errors.open(basename + ".outlexerrors", ios_base::out);

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
        {'?', "qm"},
        {'(', "lpar"},
        {')', "rpat"},
        {'{', "lcurbr"},
        {'}', "rcurbr"},
        {'[', "lsqbr"},
        {']', "rsqbr"},
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

void LexicalAnalyzer::process_remaining_digits(string& lexeme, char& c) {
    while (!done && std::isdigit((int)c)) {
        lexeme += c;
        handler.get(c);
        done = handler.eof();
    }
    handler.unget();
}

void LexicalAnalyzer::process_until_blank(string& lexeme, char& c, bool save_changes) {
    while (!done && !Utils::is_blank(c, line, handler, save_changes)) {
        lexeme += c;
        handler.get(c);
        done = handler.eof();
    }
    handler.unget();
}

Token* LexicalAnalyzer::next_token() {
    if (done)
        return new Token("EOF", "$", line);
    Token* t{ nullptr };
    string lexeme;
    ostringstream str_stream;
    char c;
    handler.get(c);
    while (!done && Utils::is_blank(c, line, handler, true)) {
        out_tokens << c;
        done = handler.eof();
        handler.get(c);
    }
    lexeme += c;
    if (std::isalpha((int)c)) {
        handler.get(c);
        while (!done && Utils::is_alphanum_or_underscore((int)c)) {
            lexeme += c;
            handler.get(c);
            done = handler.eof();
        }
        handler.unget();
        if (reserved_words.find(lexeme) != reserved_words.end())
            t = new Token(lexeme, lexeme, line);
        else
            t = new Token("id", lexeme, line);
    }
    else if (std::isdigit((int)c)) { // 0..9
        // either .0 or .digit*nonzero
        if (c == '0') { // digit
            handler.get(c);
            if (c == '.') { // 0.
                lexeme += c;
                handler.get(c);
                if (c == '0') { // 0.0
                    lexeme += c;
                    handler.get(c);
                    if (c == 'e') { // 0.0e
                        lexeme += c;
                        handler.get(c);
                        if (c == '-') { // 0.0e-
                            lexeme += c;
                            handler.get(c);
                            if (c == '0') { // 0.0e-0
                                lexeme += c;
                                t = new ErrorToken("floatLit", lexeme, line);
                            }
                            else if (Utils::is_nonzero((int)c)) { // 0.0e-<1..9>
                                lexeme += c;
                                handler.get(c);
                                process_remaining_digits(lexeme, c);
                                t = new Token("floatLit", lexeme, line);
                            }
                            else { // 0.0e-<not-a-digit>
                                t = new ErrorToken("floatLit", lexeme, line);
                            }
                        }
                        else if (Utils::is_nonzero((int)c)) { // 0.0e<1..9>
                            lexeme += c;
                            handler.get(c);
                            process_remaining_digits(lexeme, c);
                            t = new Token("floatLit", lexeme, line);
                        }
                        else {
                            t = new ErrorToken("floatLit", lexeme, line);
                        }
                    }
                    else if (std::isdigit((int)c)) { // 0.0<digit>
                        lexeme += c;
                        handler.get(c);
                        process_remaining_digits(lexeme, c);
                        handler.unget();
                        lexeme.pop_back();
                        handler.get(c);
                        if (Utils::is_nonzero((int)c)) { // 0.0<digit>*<nonzero>
                            lexeme += c;
                            handler.get(c);
                            if (c == 'e') { // 0.0e
                                lexeme += c;
                                handler.get(c);
                                if (c == '-') {
                                    lexeme += c;
                                    handler.get(c);
                                    if (c == '0') {
                                        lexeme += c;
                                        t = new ErrorToken("floatLit", lexeme, line);
                                    }
                                    else if (Utils::is_nonzero((int)c)) { // 0.0<digit>*<nonzero>e[+/-]<nonzero><digit>*
                                        lexeme += c;
                                        handler.get(c);
                                        process_remaining_digits(lexeme, c);
                                        t = new Token("floatLit", lexeme, line);
                                    }
                                }
                                else if (std::isdigit((int)c)) {
                                    lexeme += c;
                                    handler.get(c);
                                    if (c == '0') {
                                        lexeme += c;
                                        t = new Token("floatLit", lexeme, line);
                                    }
                                    else {
                                        process_remaining_digits(lexeme, c);
                                        handler.unget();
                                        handler.get(c);
                                        if (c == '0') {
                                            lexeme.pop_back();
                                            handler.unget();
                                            t = new Token("floatLit", lexeme, line);
                                        }
                                        else {
                                            t = new Token("floatLit", lexeme, line);
                                        }
                                    }
                                }
                            }
                            else { // 0.0<digit>*<nonzero><something-not-e>
                                handler.unget();
                                t = new Token("floatLit", lexeme, line);
                            }
                        } // 0.0<digit>*<!nonzero>
                        else {
                            handler.unget();
                            t = new Token("floatLit", lexeme, line);
                        }
                    }
                    else {
                        handler.unget();
                        t = new Token("floatLit", lexeme, line);
                    }
                }
                else if (std::isdigit((int)c)) { // 0.<digit>
                    while (!done && std::isdigit((int)c)) {
                        lexeme += c;
                        handler.get(c);
                        done = handler.eof();
                    }
                    handler.unget();
                    handler.unget();
                    handler.get(c);
                    if (Utils::is_nonzero((int)c)) { // 0.digit*nonzero
                        lexeme += c;
                        handler.get(c);
                        if (c == 'e') { // 0.<digit>*<nonzero>e
                            lexeme += c;
                            handler.get(c);
                            if (c == '-') { // <digits>*<nonzero>.<digit>*<nonzero>e[+/-]
                                lexeme += c;
                                handler.get(c);
                                if (Utils::is_nonzero((int)c)) { // 0.<digit>*<nonzero>e[+/-]<integer>
                                    process_remaining_digits(lexeme, c);
                                    t = new Token("floatLit", lexeme, line);
                                }
                                else {
                                    t = new ErrorToken("floatLit", lexeme, line);
                                }
                            }
                            else if (std::isdigit((int)c)) {
                                if (c == '0') {
                                    t = new Token("floatLit", lexeme, line);
                                }
                                else if (Utils::is_nonzero((int)c)) {
                                    lexeme += c;
                                    handler.get(c);
                                    process_remaining_digits(lexeme, c);
                                    t = new Token("floatLit", lexeme, line);
                                }
                            }
                        }
                        else {
                            handler.unget();
                            t = new Token("floatLit", lexeme, line);
                        }
                    }
                    else {
                        handler.unget();
                        t = new Token("floatLit", lexeme, line);
                    }
                }
                else {
                    process_until_blank(lexeme, c, false);
                    t = new ErrorToken("floatLit", lexeme, line);
                }
            }
            else {
                handler.unget();
                t = new Token("intLit", "0", line);
            }
        }
        else { // 1..9
            // we already know it's a nonzero digit
            handler.get(c);
            process_remaining_digits(lexeme, c);
            handler.get(c);
            if (c == '.') { // [1..9].
                lexeme += c;
                handler.get(c);
                if (std::isdigit((int)c)) { // [1..9].<digit>
                    lexeme += c;
                    handler.get(c);
                    process_remaining_digits(lexeme, c);
                    lexeme.pop_back();
                    handler.unget();
                    handler.get(c);
                    if (Utils::is_nonzero((int)c)) { // [1..9].<digit>*<nonzero>
                        lexeme += c;
                        handler.get(c);
                        if (c == 'e') { // <digits>*nonzero.<digit>*<nonzero>e
                            lexeme += c;
                            handler.get(c);
                            if (c == '-') { // <digits>*<nonzero>.<digit>*<nonzero>e[+/-]
                                lexeme += c;
                                handler.get(c);
                                if (Utils::is_nonzero((int)c)) { // 0.<digit>*<nonzero>e[+/-]<integer>
                                    process_remaining_digits(lexeme, c);
                                    t = new Token("floatLit", lexeme, line);
                                }
                                else {
                                    t = new ErrorToken("floatLit", lexeme, line);
                                }
                            }
                            else if (std::isdigit((int)c)) {
                                if (c == '0') {
                                    lexeme += c;
                                    t = new Token("floatLit", lexeme, line);
                                }
                                else if (Utils::is_nonzero((int)c)) {
                                    lexeme += c;
                                    handler.get(c);
                                    process_remaining_digits(lexeme, c);
                                    t = new Token("floatLit", lexeme, line);
                                }
                            }
                        }
                        else if (c == '0') { // [1..9].<digit>*0
                            process_until_blank(lexeme, c);
                            t = new ErrorToken("floatLit", lexeme, line);
                        }
                        else { // [1..9].<digit>*<not-0>
                            handler.unget();
                            t = new Token("floatLit", lexeme, line);
                        }
                    }
                    else {
                        while (!done && Utils::is_alphanum_or_underscore(c)) {
                            lexeme += c;
                            handler.get(c);
                            done = handler.eof();
                        }
                        handler.unget();
                        t = new ErrorToken("floatLit", lexeme, line);
                    }
                }
                else {
                    while (!done && Utils::is_alphanum_or_underscore(c)) {
                        lexeme += c;
                        handler.get(c);
                        done = handler.eof();
                    }
                    handler.unget();
                    t = new ErrorToken("floatLit", lexeme, line);
                }
            }
            else {
                handler.unget();
                t = new Token("intLit", lexeme, line);
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
            t = new Token("neq", "<>", line);
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
            lexeme += c;
            handler.get(c);
            while (c != '\n' && !done) {
                lexeme += c;
                handler.get(c);
                done = handler.eof();
            }
            handler.unget();
            t = new Token("inlinecmt", lexeme, starting_line);
        }
        else if (!done && c == '*') {
            lexeme += c;
            int num_open = 1;
            bool prevent_double_check = false;
            while (!done && num_open > 0) {
                handler.get(c);
                done = handler.eof();
                while (!done && c != '*') {
                    if (c == '\n') {
                        line++;
                        lexeme += "\\n";
                    }
                    else
                        lexeme += c;
                    handler.get(c);
                    done = handler.eof();
                    prevent_double_check = false;
                }
                lexeme += c;
                if (!done) {
                    if (!prevent_double_check && lexeme.at(lexeme.size() - 2) == '/')
                        num_open++;
                    else {
                        handler.get(c);
                        done = handler.eof();
                        if (c == '/') {
                            num_open--;
                            lexeme += c;
                            prevent_double_check = true;
                        }
                        else
                            handler.unget();
                    }
                }
            }
            if (done)
                t = new ErrorToken("blockcmt reached EOF", lexeme, starting_line);
            else
                t = new Token("blockcmt", lexeme, starting_line);
        }
        else {
            handler.unget(); // if this is part of another lexeme, unget
            t = new Token("div", "/", line);
        }
    }
    else if (c == ':') {
        handler.get(c);
        if (c == ':') {
            t = new Token("sr", "::", line);
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
            t = new ErrorToken("stringLit", lexeme, line);
        }
        else { // "<something that's not blank>
            // TODO: match only alphanum or space
            while (c != '"' && (Utils::is_alphanum_or_underscore(c) || c == ' ')) { // until either end of line or another "
                lexeme += c;
                handler.get(c);
            }
            handler.unget(); // check whatever was the last char
            handler.get(c);

            if (c == '"') { // "<someting>"
                lexeme.erase(0, 1);
                t = new Token("stringLit", lexeme, line);
            }
            else {
                lexeme += c;
                if (c == '\n') {
                    lexeme.pop_back();
                    handler.unget();
                }
                t = new ErrorToken("stringLit", lexeme, line);
            }
        }
    }
    else if (chars.find(c) != chars.end()) {
        auto test = chars.find(c);
        string temp{ c };
        t = new Token(test->second, temp, line);
    }
    else {
        t = new ErrorToken("char", lexeme, line);
    }
    done = handler.get() == -1;
    handler.unget();
    t->process(out_tokens, out_errors);
    return t;
}