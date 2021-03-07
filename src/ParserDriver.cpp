#include "bompilerConfig.h"
#include "Grammar.h"
#include "LexicalAnalyzer.h"
#include "Parser.h"
#include <iostream>

using std::ifstream;
using std::ios_base;
using std::cout;
using std::string;
using std::endl;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // report version
        cout << argv[0] << "Version " << bompiler_VERSION_MAJOR << "."
            << bompiler_VERSION_MINOR << "." << bompiler_VERSION_PATCH << endl;
        cout << "Usage: lexdriver" << " <path-to-source-file>" << endl;
    }
    else {
        const string& source = argv[1];
        Grammar *g = Grammar::from_file("data/calgary.grm");
        LexicalAnalyzer analyzer("data/polynomial.src");
        Parser parser(g, &analyzer);
        // parser.parse();
        // TODO
        cout << "Finished" << endl;
    }
}