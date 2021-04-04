#include <iostream>
#include <string>
#include "include/bompilerConfig.h"
#include "SemanticAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "Parser.h"

using std::cout;
using std::endl;
using std::string;

const string GRAMMAR_FILE = "data/grammars/calgary.attribute.grm";

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // report version
        cout << argv[0] << "Version " << bompiler_VERSION_MAJOR << "."
            << bompiler_VERSION_MINOR << "." << bompiler_VERSION_PATCH << endl;
        cout << "Usage: semanticanalyzerdriver" << " <path-to-source-file>" << endl;
    }
    else {
		// TODO: support multiple source files
        const string& source = argv[1];
        Grammar *g = Grammar::from_file(GRAMMAR_FILE);
        LexicalAnalyzer analyzer(source);
        Parser parser(g, &analyzer, source);
        bool result = parser.parse();
		if (result) {
			SemanticAnalyzer semantic_analyzer(source);
			semantic_analyzer.analyze(parser.root);
		}
        cout << "Finished" << endl;
		delete g;
    }
	return 0;
}
