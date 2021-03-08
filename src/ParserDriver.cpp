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

const string GRAMMAR_FILE = "data/handout/calgary.grm";

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // report version
        cout << argv[0] << "Version " << bompiler_VERSION_MAJOR << "."
            << bompiler_VERSION_MINOR << "." << bompiler_VERSION_PATCH << endl;
        cout << "Usage: parserdriver" << " <path-to-source-file>" << endl;
    }
    else {
        const string& source = argv[1];
        Grammar *g = Grammar::from_file(GRAMMAR_FILE);
        LexicalAnalyzer analyzer(source);
        Parser parser(g, &analyzer, source);
        parser.parse();
        cout << "Finished" << endl;
    }
}