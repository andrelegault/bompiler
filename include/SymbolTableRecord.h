#pragma once
#include <string>
#include <vector>
#include "AST.h"

using std::string;
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
		int offset = 0;
		SymbolTable *link = nullptr;
		vector<ASTNode*> children;
		ASTNode *node;
		SymbolTableRecord(ASTNode *node);
		virtual string to_str() const;
};

class FunctionSymbolTableRecord : public SymbolTableRecord {
	public:
		FunctionSymbolTableRecord(ASTNode *node);
		string return_type = "";
		string visibility = "";
		vector<string> params; // first entry is return type, second is first param, second is second param and so on...
		virtual string to_str() const override;
};

class VariableSymbolTableRecord : public SymbolTableRecord {
	public:
		string type = "";
		string visibility = "";
		VariableSymbolTableRecord(ASTNode *node);
		virtual string to_str() const override;
};

class ClassSymbolTableRecord : public SymbolTableRecord {
	public:
		ClassSymbolTableRecord(ASTNode *node);
		virtual string to_str() const override;
};

class InheritSymbolTableRecord : public SymbolTableRecord {
	public:
		InheritSymbolTableRecord(ASTNode *node);
		virtual string to_str() const override;
};

class ParamSymbolTableRecord : public SymbolTableRecord {
	public:
		ParamSymbolTableRecord(ASTNode *node);
		string type = "";
		virtual string to_str() const override;
};
