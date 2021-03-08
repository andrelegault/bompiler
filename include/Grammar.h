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


class Rule {
    public:
    string original;
    string symbol;
    vector<string> sentential_form;
    Rule(const string &original, const string &symbol, const vector<string> &sentential_form);
    friend ostream& operator<<(ostream& stream, const Rule &rule);
    static Rule* from_line(const string &line);
};

ostream& operator<<(ostream& stream, const Rule &rule);

class Grammar {
public:
    unordered_set<string> terminals;
    unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> non_terminals;
    unordered_map<string, map<string, Rule*>> translation_table;
    Grammar(
        const unordered_set<string> &terminals,
        const unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> &non_terminals,
        const unordered_map<string, map<string, Rule*>> &translation_table);
    ~Grammar();
    static Grammar* from_file(const string &filename);
};