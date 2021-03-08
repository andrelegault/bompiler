#include "LexicalAnalyzer.h"
#include "Grammar.h"
#include "Parser.h"
#include "Utils.h"
#include <stack>
#include <string>
#include <fstream>

using std::stack;
using std::string;
using std::ios_base;
using std::to_string;
using std::endl;
using std::ofstream;

Parser::Parser(Grammar *grammar, LexicalAnalyzer *analyzer, const string &filename): grammar(grammar), analyzer(analyzer) {
    source.open(filename, ios_base::in); 
    string basename = Utils::get_before_ext(filename);
    out_derivation.open(basename + ".outderivation", ios_base::out); 
    out_ast.open(basename + ".outast", ios_base::out); 
    out_errors.open(basename + ".outsyntaxerrors", ios_base::out); 
}

Parser::~Parser() {
    source.close();
    out_derivation.close();
    out_ast.close();
    out_errors.close();
    delete grammar;
    delete analyzer;
}

bool Parser::parse() {
    stack<string> tokens;
    bool error = false;
    tokens.push("$");
    tokens.push("start");
    Token *a = analyzer->next_token();
    while (tokens.top() != "$") {
        const string x = tokens.top();
        // ignore comments
        if (a->type == "inlinecmt" || a->type == "blockcmt") {
            delete a;
            a = analyzer->next_token();
        }
        else if (grammar->terminals.find(x) != grammar->terminals.end()) {
            if (x == a->type) {
                tokens.pop();
                delete a;
                a = analyzer->next_token();
            }
            else {
                skip_errors(tokens, a);
                error = true;
            }
        } else {
            if (grammar->translation_table.find(x) != grammar->translation_table.end()) {
                if (grammar->translation_table[x].find(a->type) != grammar->translation_table[x].end()) {
                    tokens.pop();
                    vector<string> form = grammar->translation_table[x][a->type]->sentential_form;
                    auto it = form.rbegin();
                    for (; it != form.rend(); ++it)
                        tokens.push(*it);
                }
                else {
                    skip_errors(tokens, a);
                    error = true;
                }
            }
            else {
                skip_errors(tokens, a);
                error = true;
            }
        }
    }
    return a->type == "$" && !error;
}

void Parser::skip_errors(stack<string> &tokens, Token *lookahead) {
    out_errors << "syntax error at " + to_string(lookahead->line);
    auto check = grammar->non_terminals.find(lookahead->type);
    if (check != grammar->non_terminals.end()) {
        string top = tokens.top();
        auto first_set = grammar->non_terminals[top].first;
        auto follow_set = grammar->non_terminals[top].second;
        if (lookahead->type == "$" || follow_set.find(lookahead->type) != follow_set.end()) {
            tokens.pop();
        } else {
            while (first_set.find(lookahead->type) == first_set.end() ||
                  (first_set.find("epsilon") != first_set.end() && follow_set.find(lookahead->type) == follow_set.end())) {
                lookahead = analyzer->next_token();
            }
        }
    }
}
