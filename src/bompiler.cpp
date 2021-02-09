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

*/

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // report version
        std::cout << argv[0] << "Version " << bompiler_VERSION_MAJOR << "."
            << bompiler_VERSION_MINOR << "." << bompiler_VERSION_PATCH << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
    }
    Parser parser("data/lexnegativegrading.src");
    while (!parser.done) {
        std::cout << parser.next_token();
    }
    return 0;
}