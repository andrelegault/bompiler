#pragma once
#include <string>
#include <fstream>
#include "AST.h"

using std::string;
using std::ofstream;

class SemanticAnalyzer {
	public:
		SemanticAnalyzer(const string &filename);
		void analyze(ASTNode *root);
		static ofstream semantic_errors, symbol_tables;
};
