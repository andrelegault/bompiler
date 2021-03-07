#include "LexicalAnalyzer.h"
#include "Grammar.h"
#include "Parser.h"
#include <stack>

using std::stack;
using std::string;

Parser::Parser(Grammar * const grammar, LexicalAnalyzer * const analyzer): grammar(grammar), analyzer(analyzer) {

}

Parser::~Parser() {
    delete grammar;
}

bool Parser::parse() {
    Grammar *grammar = Grammar::from_file("data/calgary.grm");
    stack<string> tokens;
    bool error = false;
    tokens.push("$");
    tokens.push("START");
    Token *a = analyzer->next_token();
    while (tokens.top() != "$") {
        const string x = tokens.top();
        if (grammar->terminals.find(x) != grammar->terminals.end()) {
            if (x == a->type) {
                tokens.pop();
                a = analyzer->next_token();
            }
            else {
                // TODO: skip_errors()
                error = true;
            }
        } else {
            if (grammar->translation_table.find(x) != grammar->translation_table.end()) {
                if (grammar->translation_table.find(a->type) != grammar->translation_table.end()) {
                    tokens.pop();
                    vector<string> form = grammar->translation_table[x][a->type]->sentential_form;
                    auto it = form.rbegin();
                    for (; it != form.rend(); ++it)
                        tokens.push(*it);
                }
                else {
                    // TODO: skip_errors()
                    error = true;
                }
            }
            else {
                // TODO: skip_errors()
                error = true;
            }
        }
    }
    return a->type == "$" && !error;
}