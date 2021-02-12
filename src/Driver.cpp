#include <iostream>
#include "bompilerConfig.h"
#include "LexicalAnalyzer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // report version
        std::cout << argv[0] << "Version " << bompiler_VERSION_MAJOR << "."
            << bompiler_VERSION_MINOR << "." << bompiler_VERSION_PATCH << std::endl;
        std::cout << "Usage: lexdriver" << " <path-to-source-file>" << std::endl;
    }
    else {
        const std::string& source = argv[1];
        LexicalAnalyzer parser(source);
        std::cout << "Data file: " << source << std::endl;
        while (!parser.done) {
            Token* t = parser.next_token();
            delete t;
        }
        std::cout << "Finished" << std::endl;
    }
}