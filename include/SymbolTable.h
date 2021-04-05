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
		map<string, SymbolTableRecord*> records;
		string name;
		const string type;

		SymbolTable(const string &type);

		void insert(SymbolTableRecord *node);

		// should search in parent if not found, and then parent's parent if still not found, etc.
		// need to take visiblity descriptors, i.e., `public` and `private into account
		bool search(const string &name);

		string print();

		// no need to create sub tables for (for, if, while, etc.)
};
