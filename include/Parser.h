#pragma once
#include "Grammar.h"
#include <fstream>
#include <stack>

using std::string;
using std::ifstream;
using std::ofstream;
using std::stack;

class Parser {
    public:
        Parser(Grammar *grammar, LexicalAnalyzer *analyzer, const string &filename);
        ~Parser();
        bool parse();
        void skip_errors(stack<string> &tokens, Token* lookahead);
    private:
        Grammar *grammar = nullptr;
        LexicalAnalyzer *analyzer = nullptr;
        ifstream source;
        ofstream out_derivation;
        ofstream out_ast;
        ofstream out_errors;
};