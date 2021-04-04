#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <unordered_set>
#include <iostream>
#include <map>
#include <set>
#include <list>
#include <Parser.h>
#include <LexicalAnalyzer.h>

using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::map;
using std::ostream;
using std::pair;
using std::list;

class Parser;
class Grammar;

struct Symbol {
	string val;
	string lhs;
	Symbol(const string &lhs, const string &val);
	static Symbol* from_string(const string &lhs, const string &str);
    virtual string to_str() const = 0;
	virtual void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *&lookahead, bool &error) = 0;
};

struct TerminalSymbol : Symbol {
	TerminalSymbol(const string &lhs, const string &val);
    string to_str() const override;
	void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *&lookahead, bool &error) override;
};

struct NonTerminalSymbol : Symbol {
	NonTerminalSymbol(const string &lhs, const string &val);
    string to_str() const override;
	void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *&lookahead, bool &error) override;
};

struct SemanticSymbol : Symbol {
	const int pop_operations;
	SemanticSymbol(const string &lhs, const string &val, const int &pop_operations = 0);
    string to_str() const override;
	void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *&lookahead, bool &error) override;
};

struct Rule {
    string original;
	~Rule();
    vector<Symbol*> sentential_form;
    Rule(const string &original, const vector<Symbol*> &sentential_form);
	Rule();
    static Rule* from_line(const string &line);
};

class Grammar {
public:
    unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> non_terminals;
    unordered_map<string, map<string, Rule*>> parsing_table;
	unordered_set<string> processable_terminal_nodes{"break", "continue", "public", "private", "id", "floatlit", "stringlit", "integer", "float", "int", "string", "intlit", "void", "plus", "minus", "or", "eq", "neq", "lt", "gt", "geq", "leq", "mult", "div", "and"};
	vector<Rule*> rules;
    Grammar(
		const vector<Rule*> &rules,
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
//ostream& operator<<(ostream& stream, const ParsingSymbol &symbol);
