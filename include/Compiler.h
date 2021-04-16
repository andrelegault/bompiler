#pragma once
#include <string>
#include <fstream>
#include "AST.h"

using std::string;
using std::ofstream;

class Compiler {
	public:
		Compiler(const string &filename);
		void compile(ASTNode *root);
		static ofstream moon_code;
};
