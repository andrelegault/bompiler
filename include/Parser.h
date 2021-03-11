#pragma once
#include "Grammar.h"
#include "LexicalAnalyzer.h"
#include <fstream>
#include <stack>
#include <string>

using std::string;
using std::ifstream;
using std::ofstream;
using std::stack;

class Symbol;
class ParsingSymbol;
class Grammar;

class Parser {
    public:
        Parser(Grammar *grammar, LexicalAnalyzer *analyzer, const string &filename);
        ~Parser();
        bool parse();
        void skip_errors(Token* lookahead);
        ofstream out_derivation;
        ofstream out_ast;
        ofstream out_errors;
		stack<Symbol*> symbols;
		stack<string> attributes;
    private:
        Grammar *grammar = nullptr;
        LexicalAnalyzer *analyzer = nullptr;
        ifstream source;
};
