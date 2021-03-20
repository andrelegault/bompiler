#include "LexicalAnalyzer.h"
#include "Grammar.h"
#include "Parser.h"
#include "Utils.h"
#include <unistd.h>
#include <stack>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using std::stack;
using std::string;
using std::ios_base;
using std::to_string;
using std::endl;
using std::ofstream;
using std::cout;
using std::vector;

Parser::Parser(Grammar *grammar, LexicalAnalyzer *analyzer, const string &filename): grammar(grammar), analyzer(analyzer) {
    source.open(filename, ios_base::in); 
    string basename = Utils::get_before_ext(filename);
    out_derivation.open(basename + ".outderivation", ios_base::out); 
    out_ast.open(basename + ".outast", ios_base::out); 
	out_errors.open(basename + ".outsyntaxerrors", ios_base::out); 
}

Parser::~Parser() {
    source.close();
    out_derivation.close();
    out_ast.close();
    out_errors.close();
}

bool Parser::parse() {
    bool error = false;
    symbols.push(grammar->END);
    symbols.push(grammar->START);
    Token *lookahead = analyzer->next_token();
    while (symbols.top() != grammar->END) {
		while (lookahead->type == "inlinecmt" || lookahead->type == "blockcmt") {
			delete lookahead;
			lookahead = analyzer->next_token();
		}
		symbols.top()->process(this, grammar, analyzer, lookahead, error);
    }
    return lookahead->type == "$" && !error;
}

void Parser::skip_errors(Token *lookahead) {
    exit(0);
    out_errors << "syntax error at " + to_string(lookahead->line) << endl;
	if (lookahead->type == "$" || grammar->non_terminals[symbols.top()->lhs].second.find(lookahead->type) != grammar->non_terminals[symbols.top()->lhs].second.end()) {
		symbols.pop();
	}
	else {
		auto first_set = grammar->non_terminals[symbols.top()->lhs].first;
		auto follow_set = grammar->non_terminals[symbols.top()->lhs].second;
		do {
			delete lookahead;
			lookahead = analyzer->next_token();
		} while (lookahead->type != "$" &&
				 first_set.find(lookahead->type) == first_set.end() ||
				 (first_set.find("epsilon") != first_set.end() && follow_set.find(lookahead->type) != follow_set.end()));
	}
}
