#pragma once
#include <unordered_set>
#include <string>
#include <fstream>
#include "AST.h"

using std::unordered_set;
using std::string;
using std::ofstream;

class SemanticAnalyzer {
	public:
		SemanticAnalyzer(const string &filename);
		void analyze(ASTNode *root);
		static ofstream semantic_errors, symbol_tables;
};
