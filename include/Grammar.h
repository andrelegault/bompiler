#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <unordered_set>
#include <iostream>
#include <map>
#include <set>

using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::map;
using std::ostream;
using std::pair;


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
	const bool is_terminal;
	const string val;
	const string lhs;
	Symbol(const bool is_terminal, const string &value, const string &lhs);
	static Symbol* from_string(const string &lhs, const string &str);
};

struct Rule {
    string original;
    vector<Symbol*> sentential_form;
    Rule(const string &original, const vector<Symbol*> &sentential_form);
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
};

ostream& operator<<(ostream& stream, const Rule &rule);
ostream& operator<<(ostream& stream, const Symbol &symbol);

