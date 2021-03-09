#pragma once
#include "Grammar.h"
#include <fstream>
#include <stack>
#include <string>

using std::string;
using std::ifstream;
using std::ofstream;
using std::stack;

class Parser {
    public:
        Parser(Grammar *grammar, LexicalAnalyzer *analyzer, const string &filename);
        ~Parser();
        bool parse();
        void skip_errors(stack<Symbol*> &symbols, Token* lookahead);
    private:
        Grammar *grammar = nullptr;
        LexicalAnalyzer *analyzer = nullptr;
        ifstream source;
        ofstream out_derivation;
        ofstream out_ast;
        ofstream out_errors;
		Symbol *START = new Symbol(false, "start", "");
		Symbol *END = new Symbol(false, "$", "");
};
