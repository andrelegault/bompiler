#include "LexicalAnalyzer.h"
#include "Grammar.h"
#include "Parser.h"
#include "Utils.h"
#include <unistd.h>
#include <stack>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using std::stack;
using std::string;
using std::ios_base;
using std::to_string;
using std::endl;
using std::ofstream;
using std::cout;
using std::endl;
using std::vector;

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
	delete END;
	delete START;
    // delete grammar;
    // delete analyzer;
}

bool Parser::parse() {
    stack<Symbol*> symbols;
    bool error = false;
    symbols.push(END);
    symbols.push(START);
    Token *lookahead = analyzer->next_token();
    while (symbols.top() != END) {
		usleep(20000);
        const Symbol *x {symbols.top()};
        cout << *x << ",\t\t\t\t\t" << *lookahead << endl;
        if (lookahead->type == "inlinecmt" || lookahead->type == "blockcmt") {
            // ignore comments
            delete lookahead;
            lookahead = analyzer->next_token();
        }
        else if (x->is_terminal == true) {
            if (x->val == lookahead->type) {
                symbols.pop();
                delete lookahead;
                lookahead = analyzer->next_token();
            }
            else {
                cout << "wrong1" << endl;
                skip_errors(symbols, lookahead);
                error = true;
            }
        }
		else {
            if (grammar->translation_table.find(x->val) != grammar->translation_table.end()) {
                if (grammar->translation_table[x->val].find(lookahead->type) != grammar->translation_table[x->val].end()) {
                    symbols.pop();
                    const vector<Symbol*> form = grammar->translation_table[x->val][lookahead->type]->sentential_form;
                    if (form.size() == 1 && form[0]->val == "epsilon") {
                        cout << "[" << x->val << "][" << lookahead->type << "]" << " -> EPSILON " << endl;
                    } else {
						auto it = form.rbegin();
						cout << "PUSHING -> ";
						for (; it != form.rend(); ++it) {
							Symbol *s = *it;
							if (s->val != "epsilon") {
								cout << s->val << ", ";
								symbols.push(s);
							}
						}
						cout << endl;
					}
                }
                else {
                    cout << "wrong2" << endl;
                    skip_errors(symbols, lookahead);
                    error = true;
                }
            }
            else {
                cout << "wrong3" << endl;
                skip_errors(symbols, lookahead);
                error = true;
            }
        }
    }
    return lookahead->type == "$" && !error;
}

void Parser::skip_errors(stack<Symbol*> &symbols, Token *lookahead) {
    out_errors << "syntax error at " + to_string(lookahead->line);
    unsigned int microseconds = 2000000;
	usleep(microseconds);
    if (symbols.top()->is_terminal == false) {
        if (grammar->non_terminals[symbols.top()->lhs].second.find(lookahead->type) != grammar->non_terminals[symbols.top()->lhs].second.end()) {
            symbols.pop();
        }
		else {
			while (true) {
				//delete lookahead;
				lookahead = analyzer->next_token();
				cout << *(symbols.top()) << *lookahead << endl;
				if (grammar->non_terminals[symbols.top()->lhs].first.find(lookahead->type) != grammar->non_terminals[symbols.top()->lhs].first.end())
					break;

				if (grammar->non_terminals[symbols.top()->lhs].first.find("epsilon") != grammar->non_terminals[symbols.top()->lhs].first.end() ||
						grammar->non_terminals[symbols.top()->lhs].second.find(lookahead->type) != grammar->non_terminals[symbols.top()->lhs].second.end())
					break;
			}
        }
    }
	else {
		// i figured out the issue (i think)
		// i shouldnt check for the follow set of the top() directly, I should check the non-terminal top() belongs to
		// i basically have to link each token in the stack to its lhs 
		while (true) {
			//delete lookahead;
			lookahead = analyzer->next_token();
				cout << *(symbols.top()) << *lookahead << endl;
			if (grammar->non_terminals[symbols.top()->lhs].first.find(lookahead->type) != grammar->non_terminals[symbols.top()->lhs].first.end())
				break;

			if (grammar->non_terminals[symbols.top()->lhs].first.find("epsilon") != grammar->non_terminals[symbols.top()->lhs].first.end() ||
					grammar->non_terminals[symbols.top()->lhs].second.find(lookahead->type) != grammar->non_terminals[symbols.top()->lhs].second.end())
				break;
					
		}

        // stop when first set includes lookahead AND (epsilon not in first set or follow set includes lookahead)
		/*
        while (symbols.top() != "$" && first_set.find(lookahead->type) == first_set.end() ||
              (first_set.find("epsilon") == first_set.end() && follow_set.find(lookahead->type) == follow_set.end())) {
            bool first = first_set.find(lookahead->type) == first_set.end();
            bool second = first_set.find("epsilon") != first_set.end() && follow_set.find(lookahead->type) == follow_set.end();
            bool result = first ^ second;
            cout << "symbols.top() = " << symbols.top() << "| lookahead = " << *lookahead << " " << first << ", " << second << endl;
            if (lookahead->type == "func" || lookahead->type == "main") {
                if (first_set.find("func") != first_set.end()) {
                    cout << "hello world" << endl;
                }
            }
            lookahead = analyzer->next_token();
            usleep(microseconds);
        }
		*/
    }
	usleep(microseconds);
}
