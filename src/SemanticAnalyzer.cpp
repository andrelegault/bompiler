#include <iostream>
#include <fstream>
#include <string>
#include "SemanticAnalyzer.h"
#include "Utils.h"
#include "Visitor.h"
#include "AST.h"

using std::ios_base;
using std::string;

SemanticAnalyzer::SemanticAnalyzer(const string &filename) {
	// get the files ready for writing
    string basename = Utils::get_before_ext(filename);
    semantic_errors.open(basename + ".outsemanticerrors", ios_base::out); 
    symbol_tables.open(basename + ".outsymboltables", ios_base::out); 
}

void SemanticAnalyzer::analyze(ASTNode *root) {
	CreationVisitor sav;
	SemanticCheckingVisitor scv;

	// phase 1: symbol table creation
	
	root->accept(sav);
	// phase 2: semantic actions processing
}

