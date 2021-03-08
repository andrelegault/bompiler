#include "Grammar.h"
#include "Utils.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <filesystem>

using std::ios_base;
using std::ifstream;
using std::map;
using std::unordered_map;
using std::unordered_set;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;
using std::filesystem::exists;


Rule::Rule(
    const string &original,
    const string &symbol,
    const vector<string> &sentential_form):
        original(original),
        symbol(symbol),
        sentential_form(sentential_form) {

}

Rule* Rule::from_line(const string &line) {
    auto parts = Utils::split_string(line, " ");
    vector<string> sentential_form;
    // starts at index 2 to ignore nonterminal and `::=`
    for (int i = 2; i < parts.size(); ++i) {
        string temp = parts[i];
        // only trim <words-like-this>
        // not 'these-words'
        if (temp[0] == '<')
            temp = Utils::trim_around(parts[i]);
        sentential_form.push_back(Utils::to_lower(temp));
    }
    return new Rule(line, Utils::to_lower(Utils::trim_around(parts[0])), sentential_form);
}

ostream& operator<<(ostream& stream, const Rule &rule) {
    stream << rule.original;
    return stream;
}

Grammar::Grammar(const unordered_set<string> &terminals,
                 const unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> &non_terminals,
                 const unordered_map<string, map<string, Rule*>> &translation_table):
    terminals(terminals),
    non_terminals(non_terminals),
    translation_table(translation_table) {

}

Grammar* Grammar::from_file(const string &filename) {
    if (!exists(filename))
        throw;
    ifstream input(filename, ios_base::in);
    vector<Rule*> rules;
    unordered_set<string> terminals;
    unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> non_terminals;
    unordered_map<string, map<string, Rule*>> translation_table;
    
    non_terminals = {
        {"addop",                           make_pair<unordered_set<string>, unordered_set<string>>({"plus", "minus", "or"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"aparamstail",                     make_pair<unordered_set<string>, unordered_set<string>>({"comma", }, {"rpar"})},
        {"arithexprtail",                   make_pair<unordered_set<string>, unordered_set<string>>({"plus", "minus", "or", }, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "comma", "colon", "rsqbr", "rpar"})},
        {"assignop",                        make_pair<unordered_set<string>, unordered_set<string>>({"assign"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"classdeclbody",                   make_pair<unordered_set<string>, unordered_set<string>>({"public", "private", "func", "integer", "float", "string", "id", }, {"rcurbr"})},
        {"exprtail",                        make_pair<unordered_set<string>, unordered_set<string>>({"eq", "neq", "lt", "gt", "leq", "geq", }, {"semi", "comma", "colon", "rsqbr", "rpar"})},
        {"arithexpr",                       make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "comma", "colon", "rsqbr", "rpar"})},
        {"fparamstail",                     make_pair<unordered_set<string>, unordered_set<string>>({"comma", }, {"rpar"})},
        {"classmethod",                     make_pair<unordered_set<string>, unordered_set<string>>({"sr", }, {"lpar"})},
        {"fparams",                         make_pair<unordered_set<string>, unordered_set<string>>({"integer", "float", "string", "id", }, {"rpar"})},
        {"funcdecltail",                    make_pair<unordered_set<string>, unordered_set<string>>({"void", "integer", "float", "string", "id"}, {"lcurbr", "semi"})},
        {"funcorassignstatidnestfunctail",  make_pair<unordered_set<string>, unordered_set<string>>({"dot", }, {"semi"})},
        {"funcorassignstatidnestvartail",   make_pair<unordered_set<string>, unordered_set<string>>({"dot", "assign"}, {"semi"})},
        {"funcorassignstatidnest",          make_pair<unordered_set<string>, unordered_set<string>>({"lpar", "lsqbr", "dot", "assign"}, {"semi"})},
        {"assignstattail",                  make_pair<unordered_set<string>, unordered_set<string>>({"assign"}, {"semi"})},
        {"funcorvar",                       make_pair<unordered_set<string>, unordered_set<string>>({"id"}, {"mult", "div", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"funcorvaridnesttail",             make_pair<unordered_set<string>, unordered_set<string>>({"dot", }, {"mult", "div", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"funcorvaridnest",                 make_pair<unordered_set<string>, unordered_set<string>>({"lpar", "lsqbr", "dot", }, {"mult", "div", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"aparams",                         make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus", }, {"rpar"})},
        {"funcstattailidnest",              make_pair<unordered_set<string>, unordered_set<string>>({"id", }, {"semi"})},
        {"funcstattail",                    make_pair<unordered_set<string>, unordered_set<string>>({"dot", "lpar", "lsqbr"}, {"semi"})},
        {"function",                        make_pair<unordered_set<string>, unordered_set<string>>({"func"}, {"main", "func"})},
        {"funchead",                        make_pair<unordered_set<string>, unordered_set<string>>({"func"}, {"lcurbr"})},
        {"inherit",                         make_pair<unordered_set<string>, unordered_set<string>>({"inherits", }, {"lcurbr"})},
        {"intlit",                          make_pair<unordered_set<string>, unordered_set<string>>({"intlit", }, {"rcurbr"})},
        {"memberdecl",                      make_pair<unordered_set<string>, unordered_set<string>>({"func", "integer", "float", "string", "id"}, {"public", "private", "func", "integer", "float", "string", "id", "rcurbr"})},
        {"funcdecl",                        make_pair<unordered_set<string>, unordered_set<string>>({"func"}, {"public", "private", "func", "integer", "float", "string", "id", "rcurbr"})},
        {"methodbodyvar",                   make_pair<unordered_set<string>, unordered_set<string>>({"var", }, {"if", "while", "read", "write", "return", "break", "continue", "id", "rcurbr"})},
        {"nestedid",                        make_pair<unordered_set<string>, unordered_set<string>>({"comma", }, {"lcurbr"})},
        {"classdecl",                       make_pair<unordered_set<string>, unordered_set<string>>({"class", }, {"func", "main"})},
        {"funcdef",                         make_pair<unordered_set<string>, unordered_set<string>>({"func", }, {"main"})},
        {"funcbody",                        make_pair<unordered_set<string>, unordered_set<string>>({"lcurbr"}, {"main", "func"})},
        {"relop",                           make_pair<unordered_set<string>, unordered_set<string>>({"eq", "neq", "lt", "gt", "leq", "geq"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"sign",                            make_pair<unordered_set<string>, unordered_set<string>>({"plus", "minus"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"start",                           make_pair<unordered_set<string>, unordered_set<string>>({"main", "class", "func"}, {})},
        {"prog",                            make_pair<unordered_set<string>, unordered_set<string>>({"main", "class", "func"}, {})},
        {"funcorassignstat",                make_pair<unordered_set<string>, unordered_set<string>>({"id"}, {"semi"})},
        {"statblock",                       make_pair<unordered_set<string>, unordered_set<string>>({"lcurbr", "if", "while", "read", "write", "return", "break", "continue", "id", }, {"else", "semi"})},
        {"expr",                            make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"semi", "comma", "colon", "rsqbr", "rpar"})},
        {"statement",                       make_pair<unordered_set<string>, unordered_set<string>>({"if", "while", "read", "write", "return", "break", "continue", "id"}, {"if", "while", "read", "write", "return", "break", "continue", "id", "else", "semi", "rcurbr"})},
        {"statementlist",                   make_pair<unordered_set<string>, unordered_set<string>>({"if", "while", "read", "write", "return", "break", "continue", "id", }, {"rcurbr"})},
        {"term",                            make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"multop",                          make_pair<unordered_set<string>, unordered_set<string>>({"mult", "div", "and"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"factor",                          make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"mult", "div", "and", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"termtail",                        make_pair<unordered_set<string>, unordered_set<string>>({"mult", "div", "and", }, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"type",                            make_pair<unordered_set<string>, unordered_set<string>>({"integer", "float", "string", "id"}, {"lcurbr", "semi", "id"})},
        {"arraysizerept",                   make_pair<unordered_set<string>, unordered_set<string>>({"lsqbr", }, {"rpar", "comma", "semi"})},
        {"vardecl",                         make_pair<unordered_set<string>, unordered_set<string>>({"integer", "float", "string", "id"}, {"public", "private", "func", "integer", "string", "id", "rcurbr"})},
        {"vardeclrep",                      make_pair<unordered_set<string>, unordered_set<string>>({"integer", "float", "string", "id", }, {"rcurbr"})},
        {"variable",                        make_pair<unordered_set<string>, unordered_set<string>>({"id"}, {"rpar"})},
        {"indicerep",                       make_pair<unordered_set<string>, unordered_set<string>>({"lsqbr", }, {"mult", "div", "and", "semi", "asssign", "dot", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"variableidnesttail",              make_pair<unordered_set<string>, unordered_set<string>>({"dot", }, {"rpar"})},
        {"variableidnest",                  make_pair<unordered_set<string>, unordered_set<string>>({"lsqbr", "dot", }, {"rpar"})},
        {"visibility",                      make_pair<unordered_set<string>, unordered_set<string>>({"public", "private", }, {"func", "integer", "float", "string", "id"})}
    };
    
    // every nonterminal has a first and follow set composed of terminals
    // every rule has a nonterminal (symbol)

    // Scans every line of the input grammar.
    // Stores all the seen symbols (terminal and non-terminal) in 2 sets.
    // Stores all the rules in a vector and refers back to them using their order, i.e., the order matters.
    // DO NOT CHANGE ``calgary.grm``
    for(string line; getline(input, line);) {
        Rule *r = Rule::from_line(line);
        rules.push_back(r);
        for(int i = 0; i < r->sentential_form.size(); ++i) {
            if (r->sentential_form[i][0] == '\'') {
                const string trimmed = Utils::trim_around(r->sentential_form[i]);
                r->sentential_form[i] = trimmed;
                terminals.insert(trimmed);
            }
        }
    }

    translation_table = {
        {"addop", {
            {"minus", rules[5]}, {"plus", rules[4]}, {"or", rules[6]}
        }},
        {"aparams", {
            {"id", rules[0]}, {"rpar", rules[1]}, {"lpar", rules[0]}, {"minus", rules[0]}, {"plus", rules[0]}, {"intlit", rules[0]}, {"?", rules[0]}, {"not", rules[0]}, {"stringlit", rules[0]}, {"floatlit", rules[0]} 
        }},
        {"aparamstail", {
            {"rpar", rules[3]}, {"comma", rules[2]}
        }},
        {"arithexpr", {
            {"id", rules[7]}, {"lpar", rules[7]}, {"minus", rules[7]}, {"plus", rules[7]}, {"intlit", rules[7]}, {"qm", rules[7]}, {"not", rules[7]}, {"stringlit", rules[7]}, {"floatlit", rules[7]}
        }},
        {"arithexprtail", {
            {"semi", rules[9]}, {"rpar", rules[9]}, {"minus", rules[8]}, {"plus", rules[8]}, {"geq", rules[9]}, {"leq", rules[9]}, {"gt", rules[9]}, {"lt", rules[9]}, {"neq", rules[9]}, {"eq", rules[9]}, {"comma", rules[9]}, {"rsqbr", rules[9]}, {"colon", rules[9]}, {"or", rules[8]}
        }},
        {"arraysizerept", {
            {"semi", rules[11]}, {"rpar", rules[11]}, {"comma", rules[11]}, {"lsqbr", rules[10]}
        }},
        {"assignop", {
            {"assign", rules[12]}
        }},
        {"assignstattail", {
            {"assign", rules[13]}
        }},
        {"classdecl", {
            {"main", rules[15]}, {"func", rules[15]}, {"class", rules[14]}
        }},
        {"classdeclbody", {
            {"private", rules[16]}, {"public", rules[16]}, {"id", rules[16]}, {"string", rules[16]}, {"float", rules[16]}, {"integer", rules[16]}, {"rcurbr", rules[17]}, {"func", rules[16]}
        }},
        {"classmethod", {
            {"lpar", rules[19]}, {"sr", rules[18]}
        }},
        {"expr", {
            {"id", rules[20]}, {"lpar", rules[20]}, {"minus", rules[20]}, {"plus", rules[20]}, {"intlit", rules[20]}, {"qm", rules[20]}, {"not", rules[20]}, {"stringlit", rules[20]}, {"floatlit", rules[20]}
        }},
        {"exprtail", {
            {"semi", rules[22]}, {"rpar", rules[22]}, {"geq", rules[21]}, {"leq", rules[21]}, {"gt", rules[21]}, {"lt", rules[21]}, {"neq", rules[21]}, {"eq", rules[21]}, {"comma", rules[22]}, {"rsqbr", rules[22]}, {"colon", rules[22]}
        }},
        {"factor", {
            {"id", rules[27]}, {"lpar", rules[31]}, {"minus", rules[33]}, {"plus", rules[33]}, {"intlit", rules[28]}, {"qm", rules[34]}, {"not", rules[32]}, {"stringlit", rules[30]}, {"floatlit", rules[29]}
        }},
        {"fparams", {
            {"id", rules[23]}, {"string", rules[23]}, {"float", rules[23]}, {"integer", rules[23]}, {"rpar", rules[24]}
        }},
        {"fparamstail", {
            {"rpar", rules[26]}, {"comma", rules[25]}
        }},
        {"funcbody", {
            {"lcurbr", rules[35]}
        }},
        {"funcdecl", {
            {"func", rules[36]}
        }},
        {"funcdecltail", {
            {"id", rules[37]}, {"string", rules[37]}, {"float", rules[37]}, {"integer", rules[37]}, {"void", rules[38]}
        }},
        {"funcdef", {
            {"main", rules[40]}, {"func", rules[39]}
        }},
        {"funchead", {
            {"func", rules[41]}
        }},
        {"funcorassignstat", {
            {"id", rules[43]}
        }},
        {"funcorassignstatidnest", {
            {"dot", rules[43]}, {"lpar", rules[44]}, {"lsqbr", rules[43]}, {"assign", rules[43]}
        }},
        {"funcorassignstatidnestfunctail", {
            {"dot", rules[45]}, {"semi", rules[46]}
        }},
        {"funcorassignstatidnestvartail", {
            {"dot", rules[47]}, {"assign", rules[48]}
        }},
        {"funcorvar", {
            {"id", rules[49]}
        }},
        {"funcorvaridnest", {
            {"dot", rules[50]}, {"semi", rules[50]}, {"rpar", rules[50]}, {"lpar", rules[51]}, {"minus", rules[50]}, {"plus", rules[50]}, {"geq", rules[50]}, {"leq", rules[50]}, {"gt", rules[50]}, {"lt", rules[50]}, {"neq", rules[50]}, {"eq", rules[50]}, {"comma", rules[50]}, {"and", rules[50]}, {"div", rules[50]}, {"mult", rules[50]}, {"rsqbr", rules[50]}, {"lsqbr", rules[50]}, {"colon", rules[50]}, {"or", rules[50]}
        }},
        {"funcorvaridnesttail", {
            {"dot", rules[52]}, {"semi", rules[53]}, {"rpar", rules[53]}, {"minus", rules[53]}, {"plus", rules[53]}, {"geq", rules[53]}, {"leq", rules[53]}, {"gt", rules[53]}, {"lt", rules[53]}, {"neq", rules[53]}, {"eq", rules[53]}, {"comma", rules[53]}, {"and", rules[53]}, {"div", rules[53]}, {"mult", rules[53]}, {"rsqbr", rules[53]}, {"colon", rules[53]}, {"or", rules[53]}
        }},
        {"funcstattail", {
            {"dot", rules[54]}, {"lpar", rules[55]}, {"lsqbr", rules[54]}
        }},
        {"funcstattailidnest", {
            {"dot", rules[56]}, {"semi", rules[57]}
        }},
        {"function", {
            {"func", rules[58]}
        }},
        {"indicerep", {
            {"dot", rules[60]}, {"semi", rules[60]}, {"rpar", rules[60]}, {"minus", rules[60]}, {"plus", rules[60]}, {"geq", rules[60]}, {"leq", rules[60]}, {"gt", rules[60]}, {"lt", rules[60]}, {"neq", rules[60]}, {"eq", rules[60]}, {"comma", rules[60]}, {"and", rules[60]}, {"div", rules[60]}, {"mult", rules[60]}, {"rsrbr", rules[60]}, {"lsqbr", rules[59]}, {"colon", rules[60]}, {"assign", rules[60]}, {"or", rules[60]}
        }},
        {"inherit", {
            {"lcurbr", rules[62]}, {"inherits", rules[61]}
        }},
        {"intlit", {
            {"intlit", rules[63]}, {"rsqbr", rules[64]}
        }},
        {"memberdecl", {
            {"id", rules[66]}, {"string", rules[66]}, {"float", rules[66]}, {"integer", rules[66]}, {"func", rules[65]}
        }},
        {"methodbodyvar", {
            {"id", rules[68]}, {"continue", rules[68]}, {"break", rules[68]}, {"return", rules[68]}, {"write", rules[68]}, {"read", rules[68]}, {"while", rules[68]}, {"if", rules[68]}, {"rcurbr", rules[68]}, {"var", rules[67]}
        }},
        {"multop", {
            {"and", rules[71]}, {"div", rules[70]}, {"mult", rules[69]}
        }},
        {"nestedid", {
            {"lcurbr", rules[73]}, {"comma", rules[72]}
        }},
        {"prog", {
            {"main", rules[74]}, {"func", rules[74]}, {"class", rules[74]}
        }},
        {"relop", {
            {"geq", rules[80]}, {"leq", rules[79]}, {"gt", rules[78]}, {"lt", rules[77]}, {"neq", rules[76]}, {"eq", rules[75]}
        }},
        {"sign", {
            {"minus", rules[83]}, {"plus", rules[82]}
        }},
        {"start", {
            {"main", rules[81]}, {"func", rules[81]}, {"class", rules[81]}
        }},
        {"statblock", {
            {"id", rules[85]}, {"semi", rules[86]}, {"continue", rules[85]}, {"break", rules[85]}, {"return", rules[85]}, {"write", rules[85]}, {"read", rules[85]}, {"while", rules[85]}, {"else", rules[86]}, {"if", rules[85]}, {"lcurbr", rules[84]}
        }},
        {"statement", {
            {"id", rules[87]}, {"continue", rules[94]}, {"break", rules[93]}, {"return", rules[92]}, {"write", rules[91]}, {"read", rules[90]}, {"while", rules[89]}, {"if", rules[88]}
        }},
        {"statementlist", {
            {"id", rules[95]}, {"continue", rules[95]}, {"break", rules[95]}, {"return", rules[95]}, {"write", rules[95]}, {"read", rules[95]}, {"while", rules[95]}, {"if", rules[95]}, {"rcurbr", rules[96]}
        }},
        {"term", {
            {"id", rules[97]}, {"lpar", rules[96]}, {"minus", rules[96]}, {"plus", rules[96]}, {"intlit", rules[96]}, {"qm", rules[96]}, {"not", rules[96]}, {"stringlit", rules[96]}, {"floatlit", rules[96]}
        }},
        {"termtail", {
            {"semi", rules[99]}, {"rpar", rules[99]}, {"minus", rules[99]}, {"plus", rules[99]}, {"geq", rules[99]}, {"leq", rules[99]}, {"gt", rules[99]}, {"lt", rules[99]}, {"neq", rules[99]}, {"eq", rules[99]}, {"comma", rules[99]}, {"and", rules[98]}, {"div", rules[98]}, {"mult", rules[98]}, {"rsqbr", rules[99]}, {"colon", rules[99]}, {"or", rules[99]}
        }},
        {"type", {
            {"id", rules[103]}, {"string", rules[102]}, {"float", rules[101]}, {"integer", rules[100]}
        }},
        {"vardecl", {
            {"id", rules[104]}, {"string", rules[104]}, {"float", rules[104]}, {"integer", rules[104]}
        }},
        {"vardeclrep", {
            {"id", rules[105]}, {"string", rules[105]}, {"float", rules[105]}, {"integer", rules[105]}, {"rcurbr", rules[106]}
        }},
        {"variable", {
            {"id", rules[107]}
        }},
        {"variableidnest", {
            {"dot", rules[108]}, {"rpar", rules[108]}, {"lsqbr", rules[108]}
        }},
        {"variableidnesttail", {
            {"dot", rules[109]}, {"rpar", rules[110]}
        }},
        {"visibility", {
            {"private", rules[112]}, {"public", rules[111]}, {"string", rules[113]}, {"float", rules[113]}, {"integer", rules[113]}, {"func", rules[113]}
        }}
    };
    
    input.close();
    
    return new Grammar(terminals, non_terminals, translation_table);
}

Grammar::~Grammar() {
    // for (auto &it : translation_table) {
    //     for(auto &it2 : it.second) {
    //         for (auto &vec : it2.second->sentential_form) {
    //             for (auto &kek : vec)
    //                 delete kek;
    //         }
    //     }
    // }
}