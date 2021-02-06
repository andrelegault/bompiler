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

Token Parser::next_token() {
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