#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::map;

class Rule {
    public:
    string symbol;
    vector<string> sentential_form;
    Rule(const string &symbol, const vector<string> &sentential_form);
    static Rule* from_line(const string &line);
};

class Grammar {
public:
    unordered_set<string> terminals;
    unordered_set<string> non_terminals;
    unordered_map<string, map<string, Rule*>> translation_table;
    Grammar(const unordered_set<string> &terminals, const unordered_set<string> &non_terminals, const unordered_map<string, map<string, Rule*>> &translation_table);
    ~Grammar();
    static Grammar* from_file(const string &filename);
};