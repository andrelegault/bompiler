#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "bompilerConfig.h"
#include "Parser.h"

/// Driver
// letter: [a-zA-Z]
// digit: \d
// nonzero: [1-9]
// fraction: .((\d*[1-9])|0)
// alphanum: [a-zA-Z]|\d|_

// id: [a-zA-Z]([a-zA-Z]|\d|_)*
// integer: ^([1-9]\d*|0)$
// float: ^([1-9]\d*|0)\.((\d*[1-9])|0)((e[+|-])?([1-9]\d*|0))*$
// comment: ^\/\*.*\*\/ -> see https://ideone.com/2xC4ih for an example of how to use multi-line matching

// keywords: if|then|else|integer|float|string|void|public|private|func|var|class|while|read|write|return|main|niherits|break|continue

/*
3 primitive types: id, integer, and float.
one solution: use multiple tries where nodes are regular expressions in order to land on a type
the benefit is that we don't have to check => this actually does multiple matching operations vs. one big matching operation (arguibly better)

note: IGNORE COMMENTS!! yay!

to remove (ignore) multi-line commeents:
*/

void test_n_shit() {
    std::smatch thing;
    std::string sto("abc");
    std::regex re("[a-zA-Z]([a-zA-Z]|\\d|_)*");
    if (std::regex_match(sto, thing, re)) {
        std::cout << "yes!" << std::endl;
        for (auto kek : thing)
            std::cout << kek.str() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // report version
        std::cout << argv[0] << "Version " << bompiler_VERSION_MAJOR << "."
            << bompiler_VERSION_MINOR << "." << bompiler_VERSION_PATCH << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
    }
    Parser parser("data/lexpositivegrading.src");
    while (!parser.done) {
        std::cout << parser.next_token() << std::endl;
    }
    return 0;
}
int main2(int argc, char* argv[]) {
    std::ifstream src_file("data/lexpositivegrading.src");
    std::string s;
    while (std::getline(src_file, s)) {
        // remove \n
        s.pop_back();
        std::smatch m;
        std::regex re("[a-zA-Z]([a-zA-Z]|\\d|_)*");
        if (std::regex_match(s, m, re)) {
            std::cout << "[id:" << m[0].str() << "]" << std::endl;
            // for(auto match : m)
                // std::cout << m.str() << std::endl;
        }
        else {
            std::cout << "[Invalid identifier:" << s << "]" << std::endl;
        }
    }
    if (argc < 2) {
        // report version
        std::cout << argv[0] << "Version " << bompiler_VERSION_MAJOR << "."
            << bompiler_VERSION_MINOR << "." << bompiler_VERSION_PATCH << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }
    return 0;
}