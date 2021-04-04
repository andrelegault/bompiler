#include <string>
#include "SymbolTable.h"
#include "SymbolTableRecord.h"
#include "AST.h"


SymbolTableRecord::SymbolTableRecord() { }

SymbolTable::SymbolTable(const string &name): name(name) { }
//SymbolTableRecord::SymbolTableRecord(const string &name, const string &kind, const vector<string> &params = {}, SymbolTable *parent = nullptr) { }

// add table to parent's

// this should check if an element with the same name already exists
// if it does, a semantic error has happened: multiply declared identifiers
void SymbolTable::insert(ASTNode *node) {
}

// should search in parent if not found, and then parent's parent if still not found, etc.
// need to take visiblity descriptors, i.e., `public` and `private into account
bool SymbolTable::search(ASTNode *node) {
	return true;
}

string SymbolTable::print() {
	return "test";
}
