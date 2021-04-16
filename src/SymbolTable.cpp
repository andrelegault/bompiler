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
	ss << setw(15) << name << " | " << setw(10) << kind << " | " << setw(20) << types_str << " | " << setw(3) << node->size;
	return ss.str();
}

SymbolTable::SymbolTable(ASTNode *node, const string &type): node(node), type(type) { }

// this should check if an element with the same name already exists
// if it does, a semantic error has happened: multiply declared identifiers
// else, check in the parent's table until found or parent's parent's table, etc.
void SymbolTable::insert(SymbolTableRecord *node) {
	if (node == nullptr)
		return;
	auto first = records.find(node->name);
	if (first != records.end()) {
		// check params of matching function
		if (first->second->kind == "function") {
			auto &first_types = first->second->types;

			if (first_types.size() == node->types.size()) {
				int same_type_count = 0;
				for(int i = 0; i < first_types.size(); i++) {
					if (first_types[i] == node->types[i])
						same_type_count++;
				}
				if (same_type_count != first_types.size())
					return;
			}
		}
		
		cout << "multiply declared identifier: " << node->name << endl;
		SemanticAnalyzer::semantic_errors << "multiply declared identifier: " << node->name << endl;
	} else {
		records[node->name] = node;
	}
}

// should search in parent if not found, and then parent's parent if still not found, etc.
// need to take visiblity descriptors, i.e., `public` and `private into account
bool SymbolTable::search(const string &target_name, const string &target_type) {
	bool found = false;
	ASTNode *current = this->node;
	do {
		if (current->record->link != nullptr) {
			//for (const auto &what : current->record->link->records)
				//cout << what.first << endl;

			auto result = current->record->link->records.find(target_name);
			if (result != current->record->link->records.end()) {
				//cout << result->second->node->get_type() << " == " << target_type << ", " << result->second->name << " == " << target_name << endl;
				found = result->second->node->get_type() == target_type && result->second->name == target_name;
				// doesn't take into account the `types` vector of parameters, return types, etc.
			}
		}
		current = current->parent;
	}
	while (!found && current != nullptr);
	return found;
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
	cout << container << endl;
	return container;
}
