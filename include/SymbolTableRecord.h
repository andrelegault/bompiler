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
	const string name, kind;
	const vector<string> params;
	ASTNode *node;
	SymbolTable *link = nullptr;
	SymbolTableRecord();
};

