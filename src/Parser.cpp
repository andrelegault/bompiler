#include "Parser.h"
#include "Utils.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

Parser::Parser(const std::string& src) {
    handler.open(src);
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

Token Parser::next_token() {
    /// uses buffer to analyze token by token
    std::string token;
    char c;
    handler.get(c);
    while (!done && is_blank(c)) {
        std::cout << "skipping one" << std::endl;
        done = handler.eof();
        handler.get(c);
    }
    token += c;
    if (c == '=') { // = or ==
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
            line++;
            return { "inlinecmt", token, starting_line };
        }
        else if (c == '*') {
            token = +c;
            handler.get(c);
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
            token +=c;
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