#include <string>
#include <fstream>
#include "Compiler.h"
#include "Visitor.h"
#include "AST.h"
#include "Utils.h"
using std::ios_base;
using std::string;
using std::ofstream;

ofstream Compiler::moon_code;

Compiler::Compiler(const string &filename) {
    string basename = Utils::get_before_ext(filename);
	Compiler::moon_code.open(basename + ".moon", ios_base::out); 
}

void Compiler::compile(ASTNode *root) {
	SizeSetterVisitor size_setter;

	root->accept(&size_setter);
}
