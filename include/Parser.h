#pragma once
#include "Grammar.h"
#include "LexicalAnalyzer.h"
#include "AST.h"
#include <fstream>
#include <stack>
#include <string>

using std::string;
using std::ifstream;
using std::ofstream;
using std::stack;

class Symbol;
class ParsingSymbol;
class SemanticSymbol;
class Grammar;

class Parser {
    public:
        Parser(Grammar *grammar, LexicalAnalyzer *analyzer, const string &filename);
        ~Parser();
        bool parse();
        void skip_errors(Token* &lookahead);
        ofstream out_derivation;
        ofstream out_ast;
        ofstream out_errors;
		vector<Symbol*> symbols;
		vector<ASTNode*> attributes;
		vector<ASTNode*> testing;
		static int id;
    private:
        Grammar *grammar = nullptr;
        LexicalAnalyzer *analyzer = nullptr;
        ifstream source;
};
