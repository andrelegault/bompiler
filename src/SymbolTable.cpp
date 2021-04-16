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
using std::to_string;

SymbolTableRecord::SymbolTableRecord(ASTNode *node) : node(node) { }

FunctionSymbolTableRecord::FunctionSymbolTableRecord(ASTNode *node) : SymbolTableRecord(node) { SymbolTableRecord::kind = "function"; }
VariableSymbolTableRecord::VariableSymbolTableRecord(ASTNode *node) : SymbolTableRecord(node) { SymbolTableRecord::kind = "variable"; }
ClassSymbolTableRecord::ClassSymbolTableRecord(ASTNode *node) : SymbolTableRecord(node) { SymbolTableRecord::kind = "class"; }
InheritSymbolTableRecord::InheritSymbolTableRecord(ASTNode *node) : SymbolTableRecord(node) { SymbolTableRecord::kind = "inherit"; }
ParamSymbolTableRecord::ParamSymbolTableRecord(ASTNode *node) : SymbolTableRecord(node) { SymbolTableRecord::kind = "param"; }
string SymbolTableRecord::to_str() const {
	stringstream ss;
	ss << setw(15) << name << " | " << setw(10) << kind << " | " << setw(20);
	return ss.str();
}
string FunctionSymbolTableRecord::to_str() const {
	stringstream ss;
	string types_str = "";
	for(const auto &str : params) {
		types_str += str + ", ";
	}
	ss << setw(15) << name << " | " << setw(10) << "function" << " | " << setw(20) << types_str;
	return ss.str();
}
string VariableSymbolTableRecord::to_str() const {
	stringstream ss;
	ss << setw(15) << name << " | " << setw(10) << "variable" << " | " << setw(20) << type << " | " << setw(3) << node->size;
	return ss.str();
}
string InheritSymbolTableRecord::to_str() const {
	stringstream ss;
	ss << setw(15) << name << " | " << setw(10) << "inherit";
	return ss.str();
}

string ClassSymbolTableRecord::to_str() const {
	stringstream ss;
	ss << setw(15) << name << " | " << setw(10) << "class" << " | " << setw(3) << node->size;
	return ss.str();
}
string ParamSymbolTableRecord::to_str() const {
	stringstream ss;
	ss << setw(15) << name << " | " << setw(10) << "param" << " | " << setw(20) << " | " << setw(3) << node->size;
	return ss.str();
}

SymbolTable::SymbolTable(ASTNode *node, const string &type): node(node), type(type) { }

// this should check if an element with the same name already exists
// if it does, a semantic error has happened: multiply declared identifiers
// else, check in the parent's table until found or parent's parent's table, etc.
void SymbolTable::insert(SymbolTableRecord *record) {
	if (record == nullptr)
		return;
	auto first = records.find(record->name);
	if (first != records.end()) {
		// check params of matching function
		if (first->second->kind == "function" && record->kind == "function") {
			auto &first_types = dynamic_cast<FunctionSymbolTableRecord*>(first->second)->params;
			auto &second_types = dynamic_cast<FunctionSymbolTableRecord*>(record)->params;

			if (first_types.size() == second_types.size()) {
				int same_type_count = 0;
				for(int i = 0; i < first_types.size(); i++) {
					if (first_types[i] == second_types[i])
						same_type_count++;
				}
				if (same_type_count != first_types.size())
					return;
			}
		}
		
		cout << "multiply declared identifier: " << record->name << endl;
		SemanticAnalyzer::semantic_errors << "multiply declared identifier: " << record->name << endl;
	} else {
		records[record->name] = record;
	}
}

// should search in parent if not found, and then parent's parent if still not found, etc.
// need to take visiblity descriptors, i.e., `public` and `private into account
bool SymbolTable::search(const string &target_name, const string &target_type) {
	bool found = false;
	ASTNode *current = this->node;
	do {
		if (current->table != nullptr) {
			//for (const auto &what : current->record->link->records)
				//cout << what.first << endl;

			auto result = current->table->records.find(target_name);
			if (result != current->table->records.end()) {
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
		container += "table::" + current->type + " " + current->name + " scope offset: " + to_string(this->size) + "\n";
		for (const auto &it : current->records) {
			if (it.second->node->table != nullptr)
				tables.push(it.second->node->table);
			container += it.second->to_str() + "\n";
		}
		container += "\n";
	}
	SemanticAnalyzer::symbol_tables << container << endl;
	cout << container << endl;
	return container;
}
