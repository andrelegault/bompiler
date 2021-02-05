#include <iostream>
#include <fstream>
#include "bompilerConfig.h"

/// Driver
// letter: [a-zA-Z]
// digit: \d
// nonzero: [1-9]
// fraction: .((\d*[1-9])|0)
// alphanum: [a-zA-Z]|\d|_
// id: ^[a-zA-Z]([a-zA-Z]|\d|_)*
// integer: ^([1-9]\d*|0)$
// float: ^([1-9]\d*|0).((\d*[1-9])|0)((e[+|-])?([1-9]\d*|0))*$

int main(int argc, char* argv[]) {
    std::ifstream src_file("data/lexpositivegrading.src");
    char c;
    while (src_file.get(c)) {
        std::cout << c << std::endl;
    }
    if (argc < 2) {
        // report version
        std::cout << argv[0] << "Version " << bompiler_VERSION_MAJOR << "."
            << bompiler_VERSION_MINOR << "." << bompiler_VERSION_PATCH << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }
}