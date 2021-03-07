#pragma once
#include "Grammar.h"

using std::string;

class Parser {
    public:
        Parser(Grammar * const grammar, LexicalAnalyzer * const analyzer);
        ~Parser();
        bool parse();
    private:
        Grammar *grammar = nullptr;
        LexicalAnalyzer *analyzer = nullptr;
};