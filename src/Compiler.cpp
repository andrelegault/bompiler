#include <string>
#include <fstream>
#include <iostream>
#include "Compiler.h"
#include "Visitor.h"
#include "AST.h"
#include "Utils.h"
using std::ios_base;
using std::string;
using std::ofstream;
using std::endl;

ofstream Compiler::moon_code;

Compiler::Compiler(const string &filename) {
    string basename = Utils::get_before_ext(filename);
	Compiler::moon_code.open(basename + ".moon", ios_base::out); 
}

void Compiler::compile(ASTNode *root) {
	Compiler::moon_code << "entry" << endl;
	Compiler::moon_code << "addi r14,r0,topaddr" << endl;
	Compiler::moon_code << "% needed otherwise sw 0(...) changes r0" << endl;
	Compiler::moon_code << "subi r14,r14,4\n" << endl;
	SizeSetterVisitor size_setter;
	CodeGenerationVisitor code_generator;


	root->accept(&size_setter);

	root->accept(&code_generator);
}
