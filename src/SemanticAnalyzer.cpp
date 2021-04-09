#include <iostream>
#include <fstream>
#include <string>
#include "SemanticAnalyzer.h"
#include "Utils.h"
#include "Visitor.h"
#include "AST.h"

using std::ios_base;
using std::string;
using std::ofstream;

ofstream SemanticAnalyzer::semantic_errors;
ofstream SemanticAnalyzer::symbol_tables; 

SemanticAnalyzer::SemanticAnalyzer(const string &filename) {
	// get the files ready for writing
    string basename = Utils::get_before_ext(filename);
	SemanticAnalyzer::semantic_errors.open(basename + ".outsemanticerrors", ios_base::out); 
	SemanticAnalyzer::symbol_tables.open(basename + ".outsymboltables", ios_base::out); 
}

void SemanticAnalyzer::analyze(ASTNode *root) {
	CreatingVisitor create;
	CheckingVisitor check;

	// phase 1: symbol table creation
	
	root->accept(&create);
	root->accept(&check);
	// phase 2: semantic actions processing
}

