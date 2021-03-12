#include "Grammar.h"
#include "Utils.h"
#include "AST.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <filesystem>
#include <list>

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
using std::list;

Symbol::Symbol(const string &val, const string &lhs): val(val), lhs(lhs) { }

ParsingSymbol::ParsingSymbol(
		const bool is_terminal,
		const string &val,
		const string &lhs):
	Symbol(val, lhs),
	is_terminal(is_terminal) { }

	SemanticSymbol::SemanticSymbol(const string &val, const string &lhs) : Symbol(val, lhs) { }

	void SemanticSymbol::process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *lookahead, bool &error) {
		parser->symbols.pop();
		if (!parser->attributes.empty()) {
			auto node = parser->attributes.top();
			parser->attributes.pop();
			//cout << "executing semantic action " << parser->attributes.size() << endl;
			parser->attributes.push(node);
		}
	}


void ParsingSymbol::process(Parser *parser, Grammar *grammar, LexicalAnalyzer *analyzer, Token *lookahead, bool &error) {
	auto &symbols = parser->symbols;
	auto &attributes = parser->attributes;
	if (this->is_terminal == true) {
		if (this->val == lookahead->type) {
			symbols.pop();
			ASTNode *node = ASTNode::make_node(this->val);
			attributes.push(node);
			delete lookahead;
			lookahead = analyzer->next_token();
		}
		else {
			parser->skip_errors(lookahead);
			error = true;
		}
	}
	else {
		parser->out_derivation << "=> ";
		for(const auto &it : grammar->derivation)
			parser->out_derivation << it->val << " ";
		parser->out_derivation << endl;
		if (grammar->translation_table.find(this->val) != grammar->translation_table.end()) {
			if (grammar->translation_table[this->val].find(lookahead->type) != grammar->translation_table[this->val].end()) {
				grammar->derivation.remove(symbols.top());
				symbols.pop();
				//cout << "[" << this->val << "][" << lookahead->type << "] -> ";
				const vector<ParsingSymbol*> form = grammar->translation_table[this->val][lookahead->type]->sentential_form;
				if (form.size() == 1 && form[0]->val == "epsilon") {
					//cout << "EPSILON";
					SemanticSymbol *sem_action = new SemanticSymbol(this->val, this->lhs);
					symbols.push(sem_action);
				} else {
					auto it = form.rbegin();
					for (; it != form.rend(); ++it) {
						ParsingSymbol *s = *it;
						if (s->is_terminal == false) {
							SemanticSymbol *sem_action = new SemanticSymbol(s->val + "(synthetized)", this->lhs +  " (synthetized)");
							symbols.push(sem_action);
						}
						grammar->derivation.push_back(s);
						//cout << s->val << ", ";
						symbols.push(s);
					}
				}
				//cout << endl;
			}
			else {
				parser->skip_errors(lookahead);
				error = true;
			}
		}
		else {
			parser->skip_errors(lookahead);
			error = true;
		}
	}
}

ParsingSymbol* ParsingSymbol::from_string(const string &lhs, const string &str) {
	string val(str);
	bool is_terminal = true;
	if (str[0] == '<')
		is_terminal = false;
	if (str[0] != 'E')
		val = Utils::trim_around(val);
	val = Utils::to_lower(val);
	return new ParsingSymbol(is_terminal, val, lhs);
}


Rule::Rule(
		const string &original,
		const vector<ParsingSymbol*> &sentential_form):
	original(original),
	sentential_form(sentential_form) {
	}


Rule* Rule::from_line(const string &line) {
	auto parts = Utils::split_string(line, " ");
	vector<ParsingSymbol*> sentential_form;
	const string lhs = Utils::to_lower(Utils::trim_around(parts[0]));
	// starts at index 2 to ignore nonterminal and `::=`
	for (int i = 2; i < parts.size(); ++i) {
		ParsingSymbol *s = ParsingSymbol::from_string(lhs, parts[i]);
		sentential_form.push_back(s);
	}
	return new Rule(line, sentential_form);
}

ostream& operator<<(ostream& stream, const SemanticSymbol &symbol) {
	string output = "SemanticSymbol(val=" + symbol.val + ",lhs=" + symbol.lhs + ",id=" + symbol.type + ")";
	stream << output;
	return stream;
}

ostream& operator<<(ostream& stream, const Symbol &symbol) {
	stream << "Symbol(val=" << symbol.val << ",lhs=" << symbol.lhs << ")";
	return stream;
}

ostream& operator<<(ostream& stream, const ParsingSymbol &symbol) {
	string trm;
	if (symbol.is_terminal == true)
		trm = "yes";
	else
		trm = "no";
	stream << "ParsingSymbol(is_terminal=" << trm << ", val=" << symbol.val << ", lhs=" << symbol.lhs << ")";
	return stream;
}

Grammar::Grammar(const unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> &non_terminals,
		const unordered_map<string, map<string, Rule*>> &translation_table):
	non_terminals(non_terminals),
	translation_table(translation_table) {
		derivation.push_back(START);
	}

Grammar* Grammar::from_file(const string &filename) {
	if (!exists(filename))
		throw;
	ifstream input(filename, ios_base::in);
	vector<Rule*> rules;
	unordered_map<string, pair<unordered_set<string>, unordered_set<string>>> non_terminals;
	unordered_map<string, map<string, Rule*>> translation_table;

	non_terminals = {
		{"addop",                           {{"plus", "minus", "or"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}}},
		{"aparamstail",                     {{"comma", "epsilon"}, {"rpar"}}},
		{"arithexprtail",                   {{"plus", "minus", "or", "epsilon"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "comma", "colon", "rsqbr", "rpar"}}},
		{"assignop",                        {{"assign"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}}},
		{"classdeclbody",                   {{"public", "private", "func", "integer", "float", "string", "id", }, {"rcurbr"}}},
		{"exprtail",                        {{"eq", "neq", "lt", "gt", "leq", "geq", "epsilon"}, {"semi", "comma", "colon", "rsqbr", "rpar"}}},
		{"arithexpr",                       {{"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "comma", "colon", "rsqbr", "rpar"}}},
		{"fparamstail",                     {{"comma", "epsilon"}, {"rpar"}}},
		{"classmethod",                     {{"sr", "epsilon"}, {"lpar"}}},
		{"fparams",                         {{"integer", "float", "string", "id", "epsilon"}, {"rpar"}}},
		{"funcdecltail",                    {{"void", "integer", "float", "string", "id"}, {"lcurbr", "semi"}}},
		{"funcorassignstatidnestfunctail",  {{"dot", "epsilon"}, {"semi"}}},
		{"funcorassignstatidnestvartail",   {{"dot", "assign"}, {"semi"}}},
		{"funcorassignstatidnest",          {{"lpar", "lsqbr", "dot", "assign"}, {"semi"}}},
		{"assignstattail",                  {{"assign"}, {"semi"}}},
		{"funcorvar",                       {{"id"}, {"mult", "div", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"}}},
		{"funcorvaridnesttail",             {{"dot", "epsilon"}, {"mult", "div", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"}}},
		{"funcorvaridnest",                 {{"lpar", "lsqbr", "dot", "epsilon"}, {"mult", "div", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"}}},
		{"aparams",                         {{"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus", "epsilon"}, {"rpar"}}},
		{"funcstattailidnest",              {{"id", "epsilon"}, {"semi"}}},
		{"funcstattail",                    {{"dot", "lpar", "lsqbr"}, {"semi"}}},
		{"function",                        {{"func"}, {"main", "func"}}},
		{"funchead",                        {{"func"}, {"lcurbr"}}},
		{"inherit",                         {{"inherits", "epsilon"}, {"lcurbr"}}},
		{"numint",                          {{"numint", "epsilon"}, {"rcurbr"}}},
		{"memberdecl",                      {{"func", "integer", "float", "string", "id"}, {"public", "private", "func", "integer", "float", "string", "id", "rcurbr"}}},
		{"funcdecl",                        {{"func"}, {"public", "private", "func", "integer", "float", "string", "id", "rcurbr"}}},
		{"methodbodyvar",                   {{"var", "epsilon"}, {"if", "while", "read", "write", "return", "break", "continue", "id", "rcurbr"}}},
		{"nestedid",                        {{"comma", "epsilon"}, {"lcurbr"}}},
		{"classdecl",                       {{"class", "epsilon"}, {"func", "main"}}},
		{"funcdef",                         {{"func", "epsilon"}, {"main"}}},
		{"funcbody",                        {{"lcurbr"}, {"main", "func"}}},
		{"relop",                           {{"eq", "neq", "lt", "gt", "leq", "geq"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}}},
		{"sign",                            {{"plus", "minus"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}}},
		{"start",                           {{"main", "class", "func"}, {}}},
		{"prog",                            {{"main", "class", "func"}, {}}},
		{"funcorassignstat",                {{"id"}, {"semi"}}},
		{"statblock",                       {{"lcurbr", "if", "while", "read", "write", "return", "break", "continue", "id", "epsilon"}, {"else", "semi"}}},
		{"expr",                            {{"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"semi", "comma", "colon", "rsqbr", "rpar"}}},
		{"statement",                       {{"if", "while", "read", "write", "return", "break", "continue", "id"}, {"if", "while", "read", "write", "return", "break", "continue", "id", "else", "semi", "rcurbr"}}},
		{"statementlist",                   {{"if", "while", "read", "write", "return", "break", "continue", "id", "epsilon"}, {"rcurbr"}}},
		{"term",                            {{"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"}}},
		{"multop",                          {{"mult", "div", "and"}, {"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}}},
		{"factor",                          {{"intlit", "floatlit", "stringlit", "lpar", "not", "qm", "id", "plus", "minus"}, {"mult", "div", "and", "semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"}}},
		{"termtail",                        {{"mult", "div", "and", "epsilon"}, {"semi", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"}}},
		{"type",                            {{"integer", "float", "string", "id"}, {"lcurbr", "semi", "id"}}},
		{"arraysizerept",                   {{"lsqbr", "epsilon"}, {"rpar", "comma", "semi"}}},
		{"vardecl",                         {{"integer", "float", "string", "id"}, {"public", "private", "func", "integer", "string", "id", "rcurbr"}}},
		{"vardeclrep",                      {{"integer", "float", "string", "id", "epsilon"}, {"rcurbr"}}},
		{"variable",                        {{"id"}, {"rpar"}}},
		{"indicerep",                       {{"lsqbr", "epsilon"}, {"mult", "div", "and", "semi", "asssign", "dot", "eq", "neq", "lt", "gt", "leq", "geq", "plus", "minus", "or", "comma", "colon", "rsqbr", "rpar"}}},
		{"variableidnesttail",              {{"dot", "epsilon"}, {"rpar"}}},
		{"variableidnest",                  {{"lsqbr", "dot", "epsilon"}, {"rpar"}}},
		{"visibility",                      {{"public", "private", "epsilon"}, {"func", "integer", "float", "string", "id"}}}
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
	}

	translation_table = {
		{"addop", {
				  {"minus", rules[5]}, {"plus", rules[4]}, {"or", rules[6]}
			  }},
		{"aparams", {
				    {"id", rules[0]}, {"rpar", rules[1]}, {"lpar", rules[0]}, {"minus", rules[0]}, {"plus", rules[0]}, {"intlit", rules[0]}, {"qm", rules[0]}, {"not", rules[0]}, {"stringlit", rules[0]}, {"floatlit", rules[0]} 
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
					     {"id", rules[42]}
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
				      {"dot", rules[60]}, {"semi", rules[60]}, {"rpar", rules[60]}, {"minus", rules[60]}, {"plus", rules[60]}, {"geq", rules[60]}, {"leq", rules[60]}, {"gt", rules[60]}, {"lt", rules[60]}, {"neq", rules[60]}, {"eq", rules[60]}, {"comma", rules[60]}, {"and", rules[60]}, {"div", rules[60]}, {"mult", rules[60]}, {"rsqbr", rules[60]}, {"lsqbr", rules[59]}, {"colon", rules[60]}, {"assign", rules[60]}, {"or", rules[60]}
			      }},
		{"inherit", {
				    {"lcurbr", rules[62]}, {"inherits", rules[61]}
			    }},
		{"numint", {
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
				 {"id", rules[97]}, {"lpar", rules[97]}, {"minus", rules[97]}, {"plus", rules[97]}, {"intlit", rules[97]}, {"qm", rules[97]}, {"not", rules[97]}, {"stringlit", rules[97]}, {"floatlit", rules[97]}
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

	return new Grammar(non_terminals, translation_table);
}

Grammar::~Grammar() {
	delete START;
	delete END;
	// for (auto &it : translation_table) {
	//     for(auto &it2 : it.second) {
	//         for (auto &vec : it2.second->sentential_form) {
	//             for (auto &kek : vec)
	//                 delete kek;
	//         }
	//     }
	// }
}
