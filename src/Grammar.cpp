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
    for (int i = 2; i < parts.size(); ++i)
        sentential_form.push_back(parts[i]);
    return new Rule(line, parts[0], sentential_form);
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
        {"ADDOP",                           make_pair<unordered_set<string>, unordered_set<string>>({"plus", "minus", "or"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"APARAMSTAIL",                     make_pair<unordered_set<string>, unordered_set<string>>({"comma", "epsilon"}, {"rpar"})},
        {"ARITHEXPRTAIL",                   make_pair<unordered_set<string>, unordered_set<string>>({"plus", "minus", "or", "epsilon"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "comma", "colon", "rsqbr", "rpar"})},
        {"ASSIGNOP",                        make_pair<unordered_set<string>, unordered_set<string>>({"assign"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"CLASSDECLBODY",                   make_pair<unordered_set<string>, unordered_set<string>>({"public", "private", "func", "integer", "float", "string", "id", "epsilon"}, {"rcurbr"})},
        {"EXPRTAIL",                        make_pair<unordered_set<string>, unordered_set<string>>({"eq", "neq", "lt", "gt", "leq", "geq", "epsilon"}, {"semi", "comma", "colon", "rsqbr", "rpar"})},
        {"ARITHEXPR",                       make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "comma", "colon", "rsqbr", "rpar"})},
        {"FPARAMSTAIL",                     make_pair<unordered_set<string>, unordered_set<string>>({"comma", "epsilon"}, {"rpar"})},
        {"CLASSMETHOD",                     make_pair<unordered_set<string>, unordered_set<string>>({"sr", "epsilon"}, {"lpar"})},
        {"FPARAMS",                         make_pair<unordered_set<string>, unordered_set<string>>({"integer", "float", "string", "id", "epsilon"}, {"rpar"})},
        {"FUNCDECLTAIL",                    make_pair<unordered_set<string>, unordered_set<string>>({"void", "integer", "float", "string", "id"}, {"lcurbr", "semi"})},
        {"FUNCORASSIGNSTATIDNESTFUNCTAIL",  make_pair<unordered_set<string>, unordered_set<string>>({"dot", "epsilon"}, {"semi"})},
        {"FUNCORASSIGNSTATIDNESTVARTAIL",   make_pair<unordered_set<string>, unordered_set<string>>({"dot", "assign"}, {"semi"})},
        {"FUNCORASSIGNSTATIDNEST",          make_pair<unordered_set<string>, unordered_set<string>>({"lpar", "lsqbr", "dot", "assign"}, {"semi"})},
        {"ASSIGNSTATTAIL",                  make_pair<unordered_set<string>, unordered_set<string>>({"assign"}, {"semi"})},
        {"FUNCORVAR",                       make_pair<unordered_set<string>, unordered_set<string>>({"id"}, {"mult", "div", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"FUNCORVARIDNESTTAIL",             make_pair<unordered_set<string>, unordered_set<string>>({"dot", "epsilon"}, {"mult", "div", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"FUNCORVARIDNEST",                 make_pair<unordered_set<string>, unordered_set<string>>({"lpar", "lsqbr", "dot", "epsilon"}, {"mult", "div", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"APARAMS",                         make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus", "epsilon"}, {"rpar"})},
        {"FUNCSTATTAILIDNEST",              make_pair<unordered_set<string>, unordered_set<string>>({"id", "epsilon"}, {"semi"})},
        {"FUNCSTATTAIL",                    make_pair<unordered_set<string>, unordered_set<string>>({"dot", "lpar", "lsqbr"}, {"semi"})},
        {"FUNCTION",                        make_pair<unordered_set<string>, unordered_set<string>>({"func"}, {"main", "func"})},
        {"FUNCHEAD",                        make_pair<unordered_set<string>, unordered_set<string>>({"func"}, {"lcurbr"})},
        {"INHERIT",                         make_pair<unordered_set<string>, unordered_set<string>>({"inherits", "epsilon"}, {"lcurbr"})},
        {"INTLIT",                          make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "epsilon"}, {"rcurbr"})},
        {"MEMBERDECL",                      make_pair<unordered_set<string>, unordered_set<string>>({"func", "integer", "float", "string", "id"}, {"public", "private", "func", "integer", "float", "string", "id", "rcurbr"})},
        {"FUNCDECL",                        make_pair<unordered_set<string>, unordered_set<string>>({"func"}, {"public", "private", "func", "integer", "float", "string", "id", "rcurbr"})},
        {"METHODBODYVAR",                   make_pair<unordered_set<string>, unordered_set<string>>({"var", "epsilon"}, {"if", "while", "read", "write", "return", "break", "continue", "id", "rcurbr"})},
        {"NESTEDID",                        make_pair<unordered_set<string>, unordered_set<string>>({"comma", "epsilon"}, {"lcurbr"})},
        {"CLASSDECL",                       make_pair<unordered_set<string>, unordered_set<string>>({"class", "epsilon"}, {"func", "main"})},
        {"FUNCDEF",                         make_pair<unordered_set<string>, unordered_set<string>>({"func", "epsilon"}, {"main"})},
        {"FUNCBODY",                        make_pair<unordered_set<string>, unordered_set<string>>({"lcurbr"}, {"main", "func"})},
        {"RELOP",                           make_pair<unordered_set<string>, unordered_set<string>>({"eq", "neq", "lt", "gt", "leq", "geq"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"SIGN",                            make_pair<unordered_set<string>, unordered_set<string>>({"plus", "minus"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"START",                           make_pair<unordered_set<string>, unordered_set<string>>({"main", "class", "func"}, {})},
        {"PROG",                            make_pair<unordered_set<string>, unordered_set<string>>({"main", "class", "func"}, {})},
        {"FUNCORASSIGNSTAT",                make_pair<unordered_set<string>, unordered_set<string>>({"id"}, {"semi"})},
        {"STATBLOCK",                       make_pair<unordered_set<string>, unordered_set<string>>({"lcurbr", "if", "while", "read", "write", "return", "break", "continue", "id", "epsilon"}, {"else", "semi"})},
        {"EXPR",                            make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"semi", "comma", "colon", "rsqbr", "rpar"})},
        {"STATEMENT",                       make_pair<unordered_set<string>, unordered_set<string>>({"if", "while", "read", "write", "return", "break", "continue", "id"}, {"if", "while", "read", "write", "return", "break", "continue", "id", "else", "semi", "rcurbr"})},
        {"STATEMENTLIST",                   make_pair<unordered_set<string>, unordered_set<string>>({"if", "while", "read", "write", "return", "break", "continue", "id", "epsilon"}, {"rcurbr"})},
        {"TERM",                            make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"MULTOP",                          make_pair<unordered_set<string>, unordered_set<string>>({"mult", "div", "and"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"})},
        {"FACTOR",                          make_pair<unordered_set<string>, unordered_set<string>>({"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"mult", "div", "and", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"TERMTAIL",                        make_pair<unordered_set<string>, unordered_set<string>>({"mult", "div", "and", "epsilon"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"TYPE",                            make_pair<unordered_set<string>, unordered_set<string>>({"integer", "float", "string", "id"}, {"lcurbr", "semi", "id"})},
        {"ARRAYSIZEREPT",                   make_pair<unordered_set<string>, unordered_set<string>>({"lsqbr", "epsilon"}, {"rpar", "comma", "semi"})},
        {"VARDECL",                         make_pair<unordered_set<string>, unordered_set<string>>({"integer", "float", "string", "id"}, {"public", "private", "func", "integer", "string", "id", "rcurbr"})},
        {"VARDECLREP",                      make_pair<unordered_set<string>, unordered_set<string>>({"integer", "float", "string", "id", "epsilon"}, {"rcurbr"})},
        {"VARIABLE",                        make_pair<unordered_set<string>, unordered_set<string>>({"id"}, {"rpar"})},
        {"INDICEREP",                       make_pair<unordered_set<string>, unordered_set<string>>({"lsqbr", "epsilon"}, {"mult", "div", "and", "semi", "asssign", "dot", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"})},
        {"VARIABLEIDNESTTAIL",              make_pair<unordered_set<string>, unordered_set<string>>({"dot", "epsilon"}, {"rpar"})},
        {"VARIABLEIDNEST",                  make_pair<unordered_set<string>, unordered_set<string>>({"lsqbr", "dot", "epsilon"}, {"rpar"})},
        {"VISIBILITY",                      make_pair<unordered_set<string>, unordered_set<string>>({"public", "private", "epsilon"}, {"func", "integer", "float", "string", "id"})}
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
        for(const auto &symbol : r->sentential_form) {
            const string trimmed = symbol.substr(1, symbol.size()-2);
            if (symbol[0] == '\'')
                terminals.insert(trimmed);
        }
    }

    translation_table = {
        {"ADDOP", {
            {"minus", rules[5]}, {"plus", rules[4]}, {"or", rules[6]}
        }},
        {"APARAMS", {
            {"id", rules[0]}, {"rpar", rules[1]}, {"lpar", rules[0]}, {"minus", rules[0]}, {"plus", rules[0]}, {"intlit", rules[0]}, {"?", rules[0]}, {"not", rules[0]}, {"stringlit", rules[0]}, {"floatlit", rules[0]} 
        }},
        {"APARAMSTAIL", {
            {"rpar", rules[3]}, {"comma", rules[2]}
        }},
        {"ARITHEXPR", {
            {"id", rules[7]}, {"lpar", rules[7]}, {"minus", rules[7]}, {"plus", rules[7]}, {"intlit", rules[7]}, {"qm", rules[7]}, {"not", rules[7]}, {"stringlit", rules[7]}, {"floatlit", rules[7]}
        }},
        {"ARITHEXPRTAIL", {
            {"semi", rules[9]}, {"rsqbr", rules[9]}, {"minus", rules[8]}, {"minus", rules[8]}, {"geq", rules[9]}, {"leq", rules[9]}, {"gt", rules[9]}, {"lt", rules[9]}, {"neq", rules[9]}, {"eq", rules[9]}, {"comma", rules[9]}, {"rsqbr", rules[9]}, {"colon", rules[9]}, {"or", rules[8]}
        }},
        {"ARRAYSIZEREPT", {
            {"semi", rules[11]}, {"rpar", rules[11]}, {"comma", rules[11]}, {"lsqbr", rules[10]}
        }},
        {"ASSIGNOP", {
            {"assign", rules[12]}
        }},
        {"ASSIGNSTATTAIL", {
            {"assign", rules[13]}
        }},
        {"CLASSDECL", {
            {"main", rules[15]}, {"func", rules[15]}, {"class", rules[14]}
        }},
        {"CLASSDECLBODY", {
            {"private", rules[16]}, {"public", rules[16]}, {"id", rules[16]}, {"string", rules[16]}, {"float", rules[16]}, {"integer", rules[16]}, {"rcurbr", rules[17]}, {"func", rules[16]}
        }},
        {"CLASSMETHOD", {
            {"lpar", rules[19]}, {"sr", rules[18]}
        }},
        {"EXPR", {
            {"id", rules[21]}, {"lpar", rules[21]}, {"minus", rules[21]}, {"plus", rules[21]}, {"intlit", rules[21]}, {"qm", rules[21]}, {"not", rules[21]}, {"stringlit", rules[21]}, {"floatlit", rules[21]}
        }},
        {"EXPRTAIL", {
            {"semi", rules[22]}, {"rpar", rules[22]}, {"geq", rules[21]}, {"leq", rules[21]}, {"gt", rules[21]}, {"lt", rules[21]}, {"neq", rules[21]}, {"eq", rules[21]}, {"comma", rules[22]}, {"rsqbr", rules[22]}, {"colon", rules[22]}
        }},
        {"FACTOR", {
            {"id", rules[27]}, {"lpar", rules[31]}, {"minus", rules[33]}, {"plus", rules[33]}, {"intlit", rules[28]}, {"qm", rules[34]}, {"not", rules[32]}, {"stringlit", rules[30]}, {"floatlit", rules[29]}
        }},
        {"FPARAMS", {
            {"id", rules[23]}, {"string", rules[23]}, {"float", rules[23]}, {"integer", rules[23]}, {"rpar", rules[24]}
        }},
        {"FPARAMSTAIL", {
            {"rpar", rules[26]}, {"comma", rules[25]}
        }},
        {"FUNCBODY", {
            {"lcurbr", rules[35]}
        }},
        {"FUNCDECL", {
            {"func", rules[36]}
        }},
        {"FUNCDECLTAIL", {
            {"id", rules[37]}, {"string", rules[37]}, {"float", rules[37]}, {"integer", rules[37]}, {"void", rules[38]}
        }},
        {"FUNCDEF", {
            {"main", rules[40]}, {"func", rules[39]}
        }},
        {"FUNCHEAD", {
            {"func", rules[41]}
        }},
        {"FUNCORASSIGNSTAT", {
            {"id", rules[43]}
        }},
        {"FUNCORASSIGNSTATIDNEST", {
            {"dot", rules[43]}, {"lpar", rules[44]}, {"lsqbr", rules[43]}, {"assign", rules[43]}
        }},
        {"FUNCORASSIGNSTATIDNESTFUNCTAIL", {
            {"dot", rules[45]}, {"semi", rules[46]}
        }},
        {"FUNCORASSIGNSTATIDNESTVARTAIL", {
            {"dot", rules[47]}, {"assign", rules[48]}
        }},
        {"FUNCORVAR", {
            {"id", rules[49]}
        }},
        {"FUNCORVARIDNEST", {
            {"dot", rules[50]}, {"semi", rules[50]}, {"rpar", rules[50]}, {"lpar", rules[51]}, {"minus", rules[50]}, {"plus", rules[50]}, {"geq", rules[50]}, {"leq", rules[50]}, {"gt", rules[50]}, {"lt", rules[50]}, {"neq", rules[50]}, {"eq", rules[50]}, {"comma", rules[50]}, {"and", rules[50]}, {"div", rules[50]}, {"mult", rules[50]}, {"rsqbr", rules[50]}, {"lsqbr", rules[50]}, {"colon", rules[50]}, {"or", rules[50]}
        }},
        {"FUNCORVARIDNESTTAIL", {
            {"dot", rules[52]}, {"semi", rules[53]}, {"rpar", rules[53]}, {"minus", rules[53]}, {"plus", rules[53]}, {"geq", rules[53]}, {"leq", rules[53]}, {"gt", rules[53]}, {"lt", rules[53]}, {"neq", rules[53]}, {"eq", rules[53]}, {"comma", rules[53]}, {"and", rules[53]}, {"div", rules[53]}, {"mult", rules[53]}, {"rsqbr", rules[53]}, {"colon", rules[53]}, {"or", rules[53]}
        }},
        {"FUNCSTATTAIL", {
            {"dot", rules[54]}, {"lpar", rules[55]}, {"lsqbr", rules[54]}
        }},
        {"FUNCSTATTAILIDNEST", {
            {"dot", rules[56]}, {"semi", rules[57]}
        }},
        {"FUNCTION", {
            {"func", rules[58]}
        }},
        {"INDICEREP", {
            {"dot", rules[60]}, {"semi", rules[60]}, {"rpar", rules[60]}, {"minus", rules[60]}, {"plus", rules[60]}, {"geq", rules[60]}, {"leq", rules[60]}, {"gt", rules[60]}, {"lt", rules[60]}, {"neq", rules[60]}, {"eq", rules[60]}, {"comma", rules[60]}, {"and", rules[60]}, {"div", rules[60]}, {"mult", rules[60]}, {"rsrbr", rules[60]}, {"lsqbr", rules[59]}, {"colon", rules[60]}, {"assign", rules[60]}, {"or", rules[60]}
        }},
        {"INHERIT", {
            {"lcurbr", rules[62]}, {"inherits", rules[61]}
        }},
        {"INTLIT", {
            {"intlit", rules[63]}, {"rsqbr", rules[64]}
        }},
        {"MEMBERDECL", {
            {"id", rules[66]}, {"string", rules[66]}, {"float", rules[66]}, {"integer", rules[66]}, {"func", rules[65]}
        }},
        {"METHODBODYVAR", {
            {"id", rules[68]}, {"continue", rules[68]}, {"break", rules[68]}, {"return", rules[68]}, {"write", rules[68]}, {"read", rules[68]}, {"while", rules[68]}, {"if", rules[68]}, {"rcurbr", rules[68]}, {"var", rules[67]}
        }},
        {"MULTOP", {
            {"and", rules[71]}, {"div", rules[70]}, {"mult", rules[69]}
        }},
        {"NESTEDID", {
            {"lcurbr", rules[73]}, {"comma", rules[72]}
        }},
        {"PROG", {
            {"main", rules[74]}
        }},
        {"RELOP", {
            {"geq", rules[80]}, {"leq", rules[79]}, {"gt", rules[78]}, {"lt", rules[77]}, {"neq", rules[76]}, {"eq", rules[75]}
        }},
        {"SIGN", {
            {"minus", rules[83]}, {"plus", rules[82]}
        }},
        {"START", {
            {"main", rules[81]}, {"func", rules[81]}, {"class", rules[81]}
        }},
        {"STATBLOCK", {
            {"id", rules[85]}, {"semi", rules[86]}, {"continue", rules[85]}, {"break", rules[85]}, {"return", rules[85]}, {"write", rules[85]}, {"read", rules[85]}, {"while", rules[85]}, {"else", rules[86]}, {"if", rules[85]}, {"lcurbr", rules[84]}
        }},
        {"STATEMENT", {
            {"id", rules[87]}, {"continue", rules[94]}, {"break", rules[93]}, {"return", rules[92]}, {"write", rules[91]}, {"read", rules[90]}, {"while", rules[89]}, {"if", rules[88]}
        }},
        {"STATEMENTLIST", {
            {"id", rules[95]}, {"continue", rules[95]}, {"break", rules[95]}, {"return", rules[95]}, {"write", rules[95]}, {"read", rules[95]}, {"while", rules[95]}, {"if", rules[95]}, {"rcurbr", rules[96]}
        }},
        {"TERM", {
            {"id", rules[97]}, {"lpar", rules[96]}, {"minus", rules[96]}, {"plus", rules[96]}, {"intlit", rules[96]}, {"qm", rules[96]}, {"not", rules[96]}, {"stringlit", rules[96]}, {"floatlit", rules[96]}
        }},
        {"TERMTAIL", {
            {"semi", rules[99]}, {"rpar", rules[99]}, {"minus", rules[99]}, {"plus", rules[99]}, {"geq", rules[99]}, {"leq", rules[99]}, {"gt", rules[99]}, {"lt", rules[99]}, {"neq", rules[99]}, {"eq", rules[99]}, {"comma", rules[99]}, {"and", rules[98]}, {"div", rules[98]}, {"mult", rules[98]}, {"rsqbr", rules[99]}, {"colon", rules[99]}, {"or", rules[99]}
        }},
        {"TYPE", {
            {"id", rules[103]}, {"string", rules[102]}, {"float", rules[101]}, {"integer", rules[100]}
        }},
        {"VARDECL", {
            {"id", rules[104]}, {"string", rules[104]}, {"float", rules[104]}, {"integer", rules[104]}
        }},
        {"VARDECLREP", {
            {"id", rules[105]}, {"string", rules[105]}, {"float", rules[105]}, {"integer", rules[105]}, {"rcurbr", rules[106]}
        }},
        {"VARIABLE", {
            {"id", rules[107]}
        }},
        {"VARIABLEIDNEST", {
            {"dot", rules[108]}, {"rpar", rules[108]}, {"lsqbr", rules[108]}
        }},
        {"VARIABLEIDNESTTAIL", {
            {"dot", rules[109]}, {"rpar", rules[110]}
        }},
        {"VISIBILITY", {
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