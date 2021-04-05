#include <queue>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include "SymbolTable.h"
#include "SymbolTableRecord.h"
#include "SemanticAnalyzer.h"
#include "AST.h"

using std::cout;
using std::endl;
using std::string;
using std::setw;
using std::stringstream;
using std::queue;

SymbolTableRecord::SymbolTableRecord(ASTNode *node) : node(node) { }

string SymbolTableRecord::to_str() const {
	stringstream ss;
	string types_str = "";
	for(const auto &str : types) {
		types_str += str + ", ";
	}
	ss << setw(15) << name << " | " << setw(10) << kind << " | " << setw(10) << types_str;
	return ss.str();
}

SymbolTable::SymbolTable(const string &type): type(type) { }
//SymbolTableRecord::SymbolTableRecord(const string &name, const string &kind, const vector<string> &params = {}, SymbolTable *parent = nullptr) { }

// add table to parent's

// this should check if an element with the same name already exists
// if it does, a semantic error has happened: multiply declared identifiers
void SymbolTable::insert(SymbolTableRecord *node) {
	if (node == nullptr)
		return;
	if (records.find(node->name) != records.end()) {
		// TODO: call `search` to check global scope + inherited scope
		// find solution for inherited scope
		cout << "multiply declared identifier: " << node->name << endl;
		SemanticAnalyzer::semantic_errors << "multiply declared identifier: " << node->name << endl;
		throw node->name;
	} else {
		records[node->name] = node;
	}
}

// should search in parent if not found, and then parent's parent if still not found, etc.
// need to take visiblity descriptors, i.e., `public` and `private into account
bool SymbolTable::search(const string &name) {
	return true;
}

string SymbolTable::print() {
	string container = "\n";
	queue<SymbolTable*> tables;
	tables.push(this);
	while (!tables.empty()) {
		SymbolTable *current = tables.front();
		tables.pop();
		container += "table::" + current->type + " " + current->name + "\n";
		for (const auto &it : current->records) {
			if (it.second->link != nullptr)
				tables.push(it.second->link);
			container += it.second->to_str() + "\n";
		}
		container += "\n";
	}
	SemanticAnalyzer::symbol_tables << container << endl;
	return container;
}
