#pragma once
#include <map>
#include <string>
#include <vector>
#include "AST.h"

using std::string;
using std::map;
using std::vector;

class ASTNode;
class SymbolTable;

class SymbolTableRecord {
	// needs to contain all necessary information to know in order to make checks, e.g., type of params, number of params, etc.
	public:
		string type = "";
		string name = "";
		string kind = "";
		string base = ""; // for associating a function to its class
		SymbolTable *link = nullptr;
		vector<string> types; // first entry is return type, second is first param, second is second param and so on...
		vector<ASTNode*> children;
		ASTNode *node;
		SymbolTableRecord(ASTNode *node);
		string to_str() const;
};
