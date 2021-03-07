#include "Grammar.h"
#include "Utils.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::ios_base;
using std::ifstream;
using std::map;
using std::unordered_map;
using std::unordered_set;
using std::string;
using std::vector;

Rule::Rule(const string &symbol, const vector<string> &sentential_form): symbol(symbol), sentential_form(sentential_form) {

}

Rule* Rule::from_line(const string &line) {
    auto parts = Utils::split_string(line, " ");
    vector<string> sentential_form;
    // starts at index 2 to ignore `::=`
    for (int i = 2; i < parts.size(); ++i)
        sentential_form.push_back(parts[i]);
    return new Rule(parts[0], sentential_form);
}

Grammar::Grammar(const unordered_set<string> &terminals,
                 const unordered_set<string> &non_terminals,
                 const unordered_map<string, map<string, Rule*>> &translation_table):
    terminals(terminals),
    non_terminals(non_terminals),
    translation_table(translation_table) {

}

Grammar* Grammar::from_file(const string &filename) {
    ifstream input(filename, ios_base::in);
    vector<Rule*> rules;
    unordered_set<string> terminals, non_terminals;
    unordered_map<string, map<string, Rule*>> translation_table;
    string line;

    // Scans every line of the input grammar.
    // Stores all the seen symbols (terminal and non-terminal) in 2 sets.
    // Stores all the rules in a vector and refers back to them using their order, i.e., the order matters.
    // DO NOT CHANGE ``calgary.grm``
    while (getline(input, line)) {
        Rule *r = Rule::from_line(line);
        rules.push_back(r);
        non_terminals.insert(r->symbol);
        for(const auto &symbol : r->sentential_form) {
            const string trimmed = symbol.substr(1, symbol.size()-2);
            if (symbol[0] == '\'')
                terminals.insert(trimmed);
            else if (symbol[0] == '<')
                non_terminals.insert(trimmed);
        }
    }
    
    translation_table = {
        {"AddOp", {
            {"minus", rules[5]}, {"plus", rules[4]}, {"or", rules[6]}
        }},
        {"AParams", {
            {"id", rules[0]}, {"rpar", rules[1]}, {"lpar", rules[0]}, {"minus", rules[0]}, {"plus", rules[0]}, {"intnum", rules[0]}, {"?", rules[0]}, {"not", rules[0]}, {"stringlit", rules[0]}, {"floatnum", rules[0]} 
        }},
        {"AParamsTail", {
            {"rpar", rules[3]}, {"comma", rules[2]}
        }},
        {"ArithExpr", {
            {"id", rules[7]}, {"lpar", rules[7]}, {"minus", rules[7]}, {"plus", rules[7]}, {"intnum", rules[7]}, {"qm", rules[7]}, {"not", rules[7]}, {"stringlit", rules[7]}, {"floatnum", rules[7]}
        }},
        {"ArithExprTail", {
            {"semi", rules[9]}, {"rsqbr", rules[9]}, {"minus", rules[8]}, {"minus", rules[8]}, {"geq", rules[9]}, {"leq", rules[9]}, {"gt", rules[9]}, {"lt", rules[9]}, {"neq", rules[9]}, {"eq", rules[9]}, {"comma", rules[9]}, {"rsqbr", rules[9]}, {"colon", rules[9]}, {"or", rules[8]}
        }},
        {"ArraySizeRept", {
            {"semi", rules[11]}, {"rpar", rules[11]}, {"comma", rules[11]}, {"lsqbr", rules[10]}
        }},
        {"AssignOp", {
            {"assign", rules[12]}
        }},
        {"AssignStatTail", {
            {"assign", rules[13]}
        }},
        {"ClassDecl", {
            {"main", rules[15]}, {"func", rules[15]}, {"class", rules[14]}
        }},
        {"ClassDeclBody", {
            {"private", rules[16]}, {"public", rules[16]}, {"id", rules[16]}, {"string", rules[16]}, {"float", rules[16]}, {"integer", rules[16]}, {"rcurbr", rules[17]}, {"func", rules[16]}
        }},
        {"ClassMethod", {
            {"lpar", rules[19]}, {"sr", rules[18]}
        }},
        {"Expr", {
            {"id", rules[21]}, {"lpar", rules[21]}, {"minus", rules[21]}, {"plus", rules[21]}, {"intnum", rules[21]}, {"qm", rules[21]}, {"not", rules[21]}, {"stringlit", rules[21]}, {"floatnum", rules[21]}
        }},
        {"ExprTail", {
            {"semi", rules[22]}, {"rpar", rules[22]}, {"geq", rules[21]}, {"leq", rules[21]}, {"gt", rules[21]}, {"lt", rules[21]}, {"neq", rules[21]}, {"eq", rules[21]}, {"comma", rules[22]}, {"rsqbr", rules[22]}, {"colon", rules[22]}
        }},
        {"Factor", {
            {"id", rules[27]}, {"lpar", rules[31]}, {"minus", rules[33]}, {"plus", rules[33]}, {"intnum", rules[28]}, {"qm", rules[34]}, {"not", rules[32]}, {"stringlit", rules[30]}, {"floatnum", rules[29]}
        }},
        {"FParams", {
            {"id", rules[23]}, {"string", rules[23]}, {"float", rules[23]}, {"integer", rules[23]}, {"rpar", rules[24]}
        }},
        {"FParamsTail", {
            {"rpar", rules[26]}, {"comma", rules[25]}
        }},
        {"FuncBody", {
            {"lcurbr", rules[35]}
        }},
        {"FuncDecl", {
            {"func", rules[36]}
        }},
        {"FuncDeclTail", {
            {"id", rules[37]}, {"string", rules[37]}, {"float", rules[37]}, {"integer", rules[37]}, {"void", rules[38]}
        }},
        {"FuncDef", {
            {"main", rules[40]}, {"func", rules[39]}
        }},
        {"FuncHead", {
            {"func", rules[41]}
        }},
        {"FuncOrAssignStat", {
            {"id", rules[43]}
        }},
        {"FuncOrAssignStatIdNest", {
            {"dot", rules[43]}, {"lpar", rules[44]}, {"lsqbr", rules[43]}, {"assign", rules[43]}
        }},
        {"FuncOrAssignStatIdNestFuncTail", {
            {"dot", rules[45]}, {"semi", rules[46]}
        }},
        {"FuncOrAssignStatIdNestVarTail", {
            {"dot", rules[47]}, {"assign", rules[48]}
        }},
        {"FuncOrVar", {
            {"id", rules[49]}
        }},
        {"FuncOrVarIdNest", {
            {"dot", rules[50]}, {"semi", rules[50]}, {"rpar", rules[50]}, {"lpar", rules[51]}, {"minus", rules[50]}, {"plus", rules[50]}, {"geq", rules[50]}, {"leq", rules[50]}, {"gt", rules[50]}, {"lt", rules[50]}, {"neq", rules[50]}, {"eq", rules[50]}, {"comma", rules[50]}, {"and", rules[50]}, {"div", rules[50]}, {"mult", rules[50]}, {"rsqbr", rules[50]}, {"lsqbr", rules[50]}, {"colon", rules[50]}, {"or", rules[50]}
        }},
        {"FuncOrVarIdNestTail", {
            {"dot", rules[52]}, {"semi", rules[53]}, {"rpar", rules[53]}, {"minus", rules[53]}, {"plus", rules[53]}, {"geq", rules[53]}, {"leq", rules[53]}, {"gt", rules[53]}, {"lt", rules[53]}, {"neq", rules[53]}, {"eq", rules[53]}, {"comma", rules[53]}, {"and", rules[53]}, {"div", rules[53]}, {"mult", rules[53]}, {"rsqbr", rules[53]}, {"colon", rules[53]}, {"or", rules[53]}
        }},
        {"FuncStatTail", {
            {"dot", rules[54]}, {"lpar", rules[55]}, {"lsqbr", rules[54]}
        }},
        {"FuncStatTailIdNest", {
            {"dot", rules[56]}, {"semi", rules[57]}
        }},
        {"Function", {
            {"func", rules[58]}
        }},
        {"IndiceRep", {
            {"dot", rules[60]}, {"semi", rules[60]}, {"rpar", rules[60]}, {"minus", rules[60]}, {"plus", rules[60]}, {"geq", rules[60]}, {"leq", rules[60]}, {"gt", rules[60]}, {"lt", rules[60]}, {"neq", rules[60]}, {"eq", rules[60]}, {"comma", rules[60]}, {"and", rules[60]}, {"div", rules[60]}, {"mult", rules[60]}, {"rsrbr", rules[60]}, {"lsqbr", rules[59]}, {"colon", rules[60]}, {"assign", rules[60]}, {"or", rules[60]}
        }},
        {"inherit", {
            {"lcurbr", rules[62]}, {"inherits", rules[61]}
        }},
        {"intnum", {
            {"intnum", rules[63]}, {"rsqbr", rules[64]}
        }},
        {"MemberDecl", {
            {"id", rules[66]}, {"string", rules[66]}, {"float", rules[66]}, {"integer", rules[66]}, {"func", rules[65]}
        }},
        {"MethodBodyVar", {
            {"id", rules[68]}, {"continue", rules[68]}, {"break", rules[68]}, {"return", rules[68]}, {"write", rules[68]}, {"read", rules[68]}, {"while", rules[68]}, {"if", rules[68]}, {"rcurbr", rules[68]}, {"var", rules[67]}
        }},
        {"MultOp", {
            {"and", rules[71]}, {"div", rules[70]}, {"mult", rules[69]}
        }},
        {"NestedId", {
            {"lcurbr", rules[73]}, {"comma", rules[72]}
        }},
        {"Prog", {
            {"main", rules[74]}
        }},
        {"RelOp", {
            {"geq", rules[80]}, {"leq", rules[79]}, {"gt", rules[78]}, {"lt", rules[77]}, {"neq", rules[76]}, {"eq", rules[75]}
        }},
        {"Sign", {
            {"minus", rules[83]}, {"plus", rules[82]}
        }},
        {"Start", {
            {"main", rules[81]}, {"func", rules[81]}, {"class", rules[81]}
        }},
        {"StatBlock", {
            {"id", rules[85]}, {"semi", rules[86]}, {"continue", rules[85]}, {"break", rules[85]}, {"return", rules[85]}, {"write", rules[85]}, {"read", rules[85]}, {"while", rules[85]}, {"else", rules[86]}, {"if", rules[85]}, {"lcurbr", rules[84]}
        }},
        {"Statement", {
            {"id", rules[87]}, {"continue", rules[94]}, {"break", rules[93]}, {"return", rules[92]}, {"write", rules[91]}, {"read", rules[90]}, {"while", rules[89]}, {"if", rules[88]}
        }},
        {"StatementList", {
            {"id", rules[95]}, {"continue", rules[95]}, {"break", rules[95]}, {"return", rules[95]}, {"write", rules[95]}, {"read", rules[95]}, {"while", rules[95]}, {"if", rules[95]}, {"rcurbr", rules[96]}
        }},
        {"Term", {
            {"id", rules[97]}, {"lpar", rules[96]}, {"minus", rules[96]}, {"plus", rules[96]}, {"intnum", rules[96]}, {"qm", rules[96]}, {"not", rules[96]}, {"stringlit", rules[96]}, {"floatnum", rules[96]}
        }},
        {"TermTail", {
            {"semi", rules[99]}, {"rpar", rules[99]}, {"minus", rules[99]}, {"plus", rules[99]}, {"geq", rules[99]}, {"leq", rules[99]}, {"gt", rules[99]}, {"lt", rules[99]}, {"neq", rules[99]}, {"eq", rules[99]}, {"comma", rules[99]}, {"and", rules[98]}, {"div", rules[98]}, {"mult", rules[98]}, {"rsqbr", rules[99]}, {"colon", rules[99]}, {"or", rules[99]}
        }},
        {"Type", {
            {"id", rules[103]}, {"string", rules[102]}, {"float", rules[101]}, {"integer", rules[100]}
        }},
        {"VarDecl", {
            {"id", rules[104]}, {"string", rules[104]}, {"float", rules[104]}, {"integer", rules[104]}
        }},
        {"VarDeclRep", {
            {"id", rules[105]}, {"string", rules[105]}, {"float", rules[105]}, {"integer", rules[105]}, {"rcurbr", rules[106]}
        }},
        {"Variable", {
            {"id", rules[107]}
        }},
        {"VariableIdNest", {
            {"dot", rules[108]}, {"rpar", rules[108]}, {"lsqbr", rules[108]}
        }},
        {"VariableIdNestTail", {
            {"dot", rules[109]}, {"rpar", rules[110]}
        }},
        {"Visibility", {
            {"private", rules[112]}, {"public", rules[111]}, {"string", rules[114]}, {"float", rules[114]}, {"integer", rules[114]}, {"func", rules[114]}
        }}
    };
    
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