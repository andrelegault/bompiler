#pragma once
#include <map>
#include <string>
#include "AST.h"

using std::string;
using std::map;

class ASTNode;
class SymbolTableRecord;

class SymbolTable {
	public:
		const string name;
		const map<string, SymbolTableRecord*> records;
		SymbolTable *parent;

		SymbolTable(const string &name);

		void insert(ASTNode *node);

		// should search in parent if not found, and then parent's parent if still not found, etc.
		// need to take visiblity descriptors, i.e., `public` and `private into account
		bool search(ASTNode *node);

		string print();

		// no need to create sub tables for (for, if, while, etc.)
};
