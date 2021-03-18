#pragma once
#include <Parser.h>
#include <LexicalAnalyzer.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <unordered_set>
#include <iostream>
#include <map>
#include <set>
#include <list>

using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::map;
using std::ostream;
using std::pair;
using std::list;


struct Symbol {
	string val;
	string lhs;
	Symbol(const string &lhs, const string &val);
	static Symbol* from_string(const string &lhs, const string &str);
	virtual void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *lookahead, bool &error) = 0;
};

struct TerminalSymbol : Symbol {
	TerminalSymbol(const string &lhs, const string &val);
	void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *lookahead, bool &error) override;
};

struct NonTerminalSymbol : Symbol {
	NonTerminalSymbol(const string &lhs, const string &val);
	void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *lookahead, bool &error) override;
};

struct SemanticSymbol : Symbol {
	const int pop_operations;
	SemanticSymbol(const string &lhs, const string &val, const int &pop_operations = 0);
	void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *lookahead, bool &error) override;
};

struct Rule {
    string original;
    vector<Symbol*> sentential_form;
    Rule(const string &original, const vector<Symbol*> &sentential_form);
	Rule();
    static Rule* from_line(const string &line);
};

class Grammar {
public:
    unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> non_terminals;
    unordered_map<string, map<string, Rule*>> parsing_table;
    Grammar(
        const unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> &non_terminals,
        const unordered_map<string, map<string, Rule*>> &parsing_table);
    ~Grammar();
    static Grammar* from_file(const string &filename);
	NonTerminalSymbol *START = new NonTerminalSymbol("", "start");
	NonTerminalSymbol *END = new NonTerminalSymbol("", "$");
	list<Symbol*> derivation;
};

//ostream& operator<<(ostream& stream, const Rule &rule);
//ostream& operator<<(ostream& stream, const Symbol &symbol);
//ostream& operator<<(ostream& stream, const SemanticSymbol &symbol);
//ostream& operator<<(ostream& stream, const ParsingSymbol &symbol);
