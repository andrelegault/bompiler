#pragma once
#include <string>
#include <vector>
#include "AST.h"

using std::string;
using std::vector;

class ASTNode;
class SymbolTableRecord;

class SymbolTable {
	public:
		ASTNode *node;
		vector<SymbolTableRecord*> records;
		string name;
		const string type;
		int compute_size() const;

		SymbolTable(ASTNode *node, const string &type);

		void insert(SymbolTableRecord *node);

		SymbolTableRecord* has_name(const string &to_find) const;

		// should search in parent if not found, and then parent's parent if still not found, etc.
		// need to take visiblity descriptors, i.e., `public` and `private into account
		bool search(const string &target_name, const string &target_type);

		string print();

		// no need to create sub tables for (for, if, while, etc.)
};
