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


// fwd def

/*
struct AbstractSymbol {
	const string value;
	Symbol(const string &value);
	virtual bool is_terminal() = 0;
};

struct NonTerminalSymbol : AbstractSymbol {
	const unordered_set<string> first, follow;
	NonTerminalSymbol(
			const string &value,
			const unordered_set<string> &first,
			const unordered_set<string> &follow);
	virtual bool is_terminal() override;
};

struct TerminalSymbol {
	const string value;
	const Rule *source;
	Symbol(const string &value);
	virtual bool is_terminal() = 0;
};
*/

struct Symbol {
	string val;
	string lhs;
	Symbol(const string &val, const string &lhs);
	virtual void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *lookahead, bool &error) = 0;
};

struct ParsingSymbol : Symbol {
	bool is_terminal;
	ParsingSymbol(const bool is_terminal, const string &value, const string &lhs);
	static ParsingSymbol* from_string(const string &lhs, const string &str);
	void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *lookahead, bool &error) override;
};

struct SemanticSymbol : Symbol {
	string type;
	SemanticSymbol(const string &val, const string &lhs);
	void process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *lookahead, bool &error) override;
};

struct Rule {
    string original;
    vector<ParsingSymbol*> sentential_form;
    Rule(const string &original, const vector<ParsingSymbol*> &sentential_form);
	Rule();
    static Rule* from_line(const string &line);
};

class Grammar {
public:
    unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> non_terminals;
    unordered_map<string, map<string, Rule*>> translation_table;
    Grammar(
        const unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> &non_terminals,
        const unordered_map<string, map<string, Rule*>> &translation_table);
    ~Grammar();
    static Grammar* from_file(const string &filename);
	ParsingSymbol *START = new ParsingSymbol(false, "start", "");
	ParsingSymbol *END = new ParsingSymbol(false, "$", "");
	list<Symbol*> derivation;
};

ostream& operator<<(ostream& stream, const Rule &rule);
ostream& operator<<(ostream& stream, const Symbol &symbol);
ostream& operator<<(ostream& stream, const SemanticSymbol &symbol);
ostream& operator<<(ostream& stream, const ParsingSymbol &symbol);
