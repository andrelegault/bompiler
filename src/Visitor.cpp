#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include "AST.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "Visitor.h"
#include "Compiler.h"

using std::cout;
using std::endl;
using std::to_string;
using std::queue;
using std::vector;

Visitor::Visitor() { }

void Visitor::visit(ProgNode *node) { }
void Visitor::visit(ClassDeclListNode *node) { }
void Visitor::visit(ClassDeclNode *node) { }
void Visitor::visit(MembListNode *node) { }
void Visitor::visit(InherListNode *node) { }
void Visitor::visit(FuncDefListNode *node) { }
void Visitor::visit(FuncDefNode *node) { }
void Visitor::visit(FuncBodyNode *node) { }
void Visitor::visit(FuncHeadNode *node) { }
void Visitor::visit(FuncDeclNode *node) { }
void Visitor::visit(FParamListNode *node) { }
void Visitor::visit(AParamListNode *node) { }
void Visitor::visit(FParamNode *node) { }
void Visitor::visit(FCallNode *node) { }
void Visitor::visit(VisibilityNode *node) { }
void Visitor::visit(MemberDeclNode *node) { }
void Visitor::visit(IdNode *node) { }
void Visitor::visit(VarDeclNode *node) { }
void Visitor::visit(VarDeclListNode *node) { }
void Visitor::visit(TypeNode *node) { }
void Visitor::visit(IntegerNode *node) { }
void Visitor::visit(FloatNode *node) { }
void Visitor::visit(StringNode *node) { }
void Visitor::visit(DimListNode *node) { }
void Visitor::visit(NumIntNode *node) { }
void Visitor::visit(StatementNode *node) { }
void Visitor::visit(StatBlockNode *node) { }
void Visitor::visit(VariableNode *node) { }
void Visitor::visit(AssignStmtNode *node) { }
void Visitor::visit(IfStmtNode *node) { }
void Visitor::visit(ReadStmtNode *node) { }
void Visitor::visit(WriteStmtNode *node) { }
void Visitor::visit(ReturnStmtNode *node) { }
void Visitor::visit(ContinueNode *node) { }
void Visitor::visit(BreakNode *node) { }
void Visitor::visit(ArithExprNode *node) { }
void Visitor::visit(RelExprNode *node) { }
void Visitor::visit(WhileStmtNode *node) { }
void Visitor::visit(SignNode *node) { }
void Visitor::visit(PlusNode *node) { }
void Visitor::visit(MinusNode *node) { }
void Visitor::visit(RelOpNode *node) { }
void Visitor::visit(MultOpNode *node) { }
void Visitor::visit(AddOpNode *node) { }
void Visitor::visit(MultNode *node) { }
void Visitor::visit(DivNode *node) { }
void Visitor::visit(AndNode *node) { }
void Visitor::visit(EqNode *node) { }
void Visitor::visit(GtNode *node) { }
void Visitor::visit(LtNode *node) { }
void Visitor::visit(NeqNode *node) { }
void Visitor::visit(GeqNode *node) { }
void Visitor::visit(LeqNode *node) { }
void Visitor::visit(IntLitNode *node) { }
void Visitor::visit(StringLitNode *node) { }
void Visitor::visit(FloatLitNode *node) { }
void Visitor::visit(DotNode *node) { }
void Visitor::visit(EpsilonNode *node) { }
void Visitor::visit(ScopeSpecNode *node) { }
void Visitor::visit(TermNode *node) { }
void Visitor::visit(IndiceListNode *node) { }
void Visitor::visit(PrivateNode *node) { }
void Visitor::visit(PublicNode *node) { }
void Visitor::visit(DataMemberNode *node) { }
void Visitor::visit(VoidNode *node) { }

/* Creating Visitor Definitions */

CreatingVisitor::CreatingVisitor() { }

// do i pass the symboltable in the accept method or do i add to it after its done
// difference b/w main function and another free function is that you just don't have parameters

void CreatingVisitor::visit(ProgNode *node) {
	// fixed for reordering
	node->record = new SymbolTableRecord(node);
	node->record->name = "Global";
	node->table = new SymbolTable(node, node->get_type());
	ASTNode *classdecllist = node->leftmost_child;
	ASTNode *funcdeflist= classdecllist->right;
	ASTNode *main = funcdeflist->right;

	ASTNode *classdecl = classdecllist->leftmost_child;
	while (classdecl != nullptr) {
		node->table->insert(classdecl->record);
		classdecl = classdecl->right;
	}

	ASTNode *funcdef = funcdeflist->leftmost_child;
	while (funcdef != nullptr) {
		node->table->insert(funcdef->record);
		funcdef = funcdef->right;
	}

	node->table->insert(main->record);
}

void CreatingVisitor::visit(FuncDefNode *node) {
	// fixed for reordering
	
	node->record = new FunctionSymbolTableRecord(node);
	node->table = new SymbolTable(node, "function");

	ASTNode *funcbodyorfunchead = node->leftmost_child;
	ASTNode *funcbody = funcbodyorfunchead;
	if (funcbodyorfunchead->right != nullptr) { // process head
		ASTNode *funchead = funcbodyorfunchead;
		funcbody = funcbodyorfunchead->right;

		ASTNode *id = funchead->leftmost_child;
		ASTNode *scopespec = id->right;
		ASTNode *fparamlist = scopespec->right;
		ASTNode *type = fparamlist->right;

		string id_val = "";

		if (scopespec->leftmost_child->is_epsilon()) {
			id_val = id->val;
		} else {
			id_val = scopespec->leftmost_child->val;
			node->record->base = id->val;
		}

		ASTNode *fparam = fparamlist->leftmost_child;
		while (fparam != nullptr) {
			node->table->insert(fparam->record);
			fparam = fparam->right;
		}
		dynamic_cast<FunctionSymbolTableRecord*>(node->record)->return_type = type->leftmost_child->get_type();
		node->record->name = id_val;
		node->table->name = id_val;
	} else {
		node->table->name = "main";
		node->record->name = "main";
	}

	// always process body
	queue<ASTNode*> nodes;
	nodes.push(funcbody);
	while (!nodes.empty()) {
		ASTNode *current = nodes.front();
		node->table->insert(current->record);
		nodes.pop();
		ASTNode *child = current->leftmost_child;
		while(child != nullptr) {
			nodes.push(child);
			child = child->right;
		}
	}
}

void CreatingVisitor::visit(ClassDeclNode *node) {
	// fixed for reordering
	node->record = new ClassSymbolTableRecord(node);
	node->table = new SymbolTable(node, "class");
	// memberlist, inherlist, id
	ASTNode *id = node->leftmost_child;
	ASTNode *inherlist = id->right;
	ASTNode *memberlist = inherlist->right;
	ASTNode *memberdecl = memberlist->leftmost_child;

	node->table->insert(inherlist->record);
	while (memberdecl != nullptr) {
		if (!memberdecl->is_epsilon()) {
			ASTNode *visibility = memberdecl->leftmost_child;
			ASTNode *funcdeclorvardecl = visibility->right;
			if (funcdeclorvardecl->record->kind == "function")
				dynamic_cast<FunctionSymbolTableRecord*>(funcdeclorvardecl->record)->visibility = visibility->get_type();
			else
				dynamic_cast<VariableSymbolTableRecord*>(funcdeclorvardecl->record)->visibility = visibility->get_type();
			node->table->insert(funcdeclorvardecl->record);
		}
		memberdecl = memberdecl->right;
	}

	node->record->base = inherlist->leftmost_child->val;
	node->table->name = id->val;
	node->record->name = id->val;
}

void CreatingVisitor::visit(InherListNode *node) {
	// fixed for reordering
	node->record = new InheritSymbolTableRecord(node);
	string inherval = "";
	ASTNode *inher_id = node->leftmost_child;
	while (inher_id != nullptr) {
		if (!inher_id->is_epsilon())
			inherval += inher_id->val + ", ";
		inher_id = inher_id->right;
	}

	if (inherval == "") {
		inherval = "none";
	}

	node->record->name = inherval;
}

void CreatingVisitor::visit(FParamNode *node) {
	// fixed for reordering
	ASTNode *type = node->leftmost_child;
	ASTNode *id = type->right;
	ASTNode *dimlist = id->right;

	DimListNode *dim = dynamic_cast<DimListNode*>(dimlist);
	string types_str = type->leftmost_child->get_type() == "id" ? type->leftmost_child->val : type->leftmost_child->get_type();
	types_str += dim->get_dims_str();

	node->record = new ParamSymbolTableRecord(node);
	node->record->name = id->val;
	dynamic_cast<ParamSymbolTableRecord*>(node->record)->type = types_str;
}

void CreatingVisitor::visit(FuncDeclNode *node) {
	// fixed for reordering
	ASTNode *id = node->leftmost_child;
	ASTNode *fparamlist = id->right;
	ASTNode *return_type = fparamlist->right;
	string return_type_val = return_type->get_type() == "id" ? return_type->val : return_type->get_type();

	node->record = new FunctionSymbolTableRecord(node);
	node->record->name = id->val;
	dynamic_cast<FunctionSymbolTableRecord*>(node->record)->return_type = return_type_val;

	ASTNode *fparam = fparamlist->leftmost_child;

	while (fparam != nullptr) {
 		if (!fparam->is_epsilon())
			dynamic_cast<FunctionSymbolTableRecord*>(node->record)->params.push_back(dynamic_cast<ParamSymbolTableRecord*>(fparam->record)->type);
		fparam = fparam->right;
	}

}

void CreatingVisitor::visit(VarDeclNode *node) {
	// fixed for reordering
	ASTNode *type = node->leftmost_child;
	ASTNode *id = type->right;
	ASTNode *dimlist = id->right;

	ASTNode *actual_type = type->leftmost_child;
	string type_val = actual_type->get_type() == "id" ? actual_type->val : actual_type->get_type();

	type_val += dynamic_cast<DimListNode*>(dimlist)->get_dims_str();

	node->record = new VariableSymbolTableRecord(node);
	node->record->name = id->val;
	dynamic_cast<VariableSymbolTableRecord*>(node->record)->type = type_val;
}

void CreatingVisitor::visit(IntLitNode *node) {
	// fixed for reordering
	ASTNode *parent = node->parent;
	while (parent != nullptr && parent->get_type() != "statement") {
		parent = parent->parent;
	}
	if (parent != nullptr) {
		ASTNode *funcdef = parent->parent->parent->parent;
		node->size = 4;
		node->record = new SymbolTableRecord(node);
		node->record->name = "t" + to_string(this->temp_count++);
		node->record->kind = "temp";
		node->record->type = "integer";
	}
}

void CreatingVisitor::visit(FloatLitNode *node) {
	// fixed for reordering
	ASTNode *parent = node->parent;
	while (parent != nullptr && parent->get_type() != "statement") {
		parent = parent->parent;
	}
	if (parent != nullptr) {
		ASTNode *funcdef = parent->parent->parent->parent;
		node->size = 8;
		node->record = new SymbolTableRecord(node);
		node->record->name = "t" + to_string(this->temp_count++);
		node->record->kind = "temp";
		node->record->type = "float";
	}
}

void CreatingVisitor::visit(StringLitNode *node) {
	// fixed for reordering
	ASTNode *parent = node->parent;
	while (parent != nullptr && parent->get_type() != "statement") {
		parent = parent->parent;
	}
	if (parent != nullptr) {
		ASTNode *funcdef = parent->parent->parent->parent;
		node->size = 4;
		node->record = new SymbolTableRecord(node);
		node->record->name = "t" + to_string(this->temp_count++);
		node->record->kind = "temp";
		node->record->type = "string";
	}
}

void CreatingVisitor::visit(StatementNode *node) {
}


/* Checking Visitor Definitions */

CheckingVisitor::CheckingVisitor() { }

void CheckingVisitor::visit(ClassDeclNode *node) {
	// TODO: check for circular dependencies
	if (node->record->base != "") {
		// check in global table
		bool found = node->parent->parent->table->search(node->record->base, "classdecl");
		if (!found) {
			cout << "undeclared class " << node->record->base << endl;
			SemanticAnalyzer::semantic_errors << "undeclared class " << node->record->base << endl;
		} else {
			// check if circular
			auto record = node->parent->parent->table->has_name(node->record->base);
			if (record->base == node->record->name) {
				cout << "circular dependency between " << node->record->base << " <-> " << node->record->name << endl;
				SemanticAnalyzer::semantic_errors << "circular dependency between " << node->record->base << " <-> " << node->record->name << endl;
			}
		}
	}
}

void CheckingVisitor::visit(FuncDefNode *node) {
	// TODO: if there's a scope specifier, check if that class was declared
	// TODO: if member func, check if respective funcdeclnode is present
	if (node->record->base != "") { // is a member function
		// check in parent (global table) if it has a `classdecl` with name `node->record->scope`
		bool assoc_class_declared = node->parent->parent->table->search(node->record->base, "classdecl");
		if (!assoc_class_declared) {
			cout << "undeclared class " << node->record->base << endl;
			SemanticAnalyzer::semantic_errors << "undeclared class " << node->record->base << endl;
		} else {
			auto table = node->parent->parent->table;
			bool funcdecl_found = table->search(node->record->name, "funcdecl");
			if (!funcdecl_found) {
				cout << "definition of undeclared function " << node->record->name << endl;
				SemanticAnalyzer::semantic_errors << "definition of undeclared function " << node->record->name << endl;
			}
		}
	}
}

void CheckingVisitor::visit(FuncDeclNode *node) {
	// TODO: check if its respective funcdefnode is present (is defined)
}

void CheckingVisitor::visit(MemberDeclNode *node) {
	// TODO: use visibility descriptor to check if identifier is accessible
}

void CheckingVisitor::visit(VariableNode *node) {
	// TODO: check if it was declared in parent
}

void CheckingVisitor::visit(FCallNode *node) {
	// TODO: check number of params match + parameter types match (use aparamslist to check each child)
}

void CheckingVisitor::visit(VarDeclNode *node) {
}

void CheckingVisitor::visit(AssignStmtNode *node) {
	// TODO: check that operand types match
}

void CheckingVisitor::visit(ReturnStmtNode *node) {
	// TODO: check if return type of parent function is same as that of statement
}

void CheckingVisitor::visit(DimListNode *node) {
	// TODO: check if type of indices is integer
}

void CheckingVisitor::visit(DotNode *node) {
	// TODO: check if left's type is declared + is a member of that (mind inherited members)
}

/* Size Checker Visitor definitions */

SizeSetterVisitor::SizeSetterVisitor() {
}

void SizeSetterVisitor::visit(IntegerNode *node) {
	node->size = 4;
}


void SizeSetterVisitor::visit(FloatNode *node) {
	node->size = 8;
}

void SizeSetterVisitor::visit(VarDeclNode *node) {
	DimListNode *dims = dynamic_cast<DimListNode*>(node->leftmost_child);
	vector<int> dimensions = dims->get_dims();
	int total_cells = 1;
	if (dimensions.size() >= 1) {
		for (const auto &dim : dimensions) {
			total_cells *= dim;
		}
	}

	node->size = total_cells * node->leftmost_child->right->right->leftmost_child->size;
}

void SizeSetterVisitor::visit(FuncDefNode *node) {
	ASTNode *func_body = node->leftmost_child;
	ASTNode *statblock = func_body->leftmost_child;
	ASTNode *vardecllist = statblock->right;

	ASTNode *stmt = statblock->leftmost_child;
	ASTNode *vardecl = vardecllist->leftmost_child;

	// set offsets
	int total = 0;
	for(const auto &record : node->table->records) {
		record->offset = total;
		total -= record->node->size;
	}

	if (node->record->name != "main") {
		// jump
		node->size -= 4;
		// return
		FunctionSymbolTableRecord *record = dynamic_cast<FunctionSymbolTableRecord*>(node->record);
		if (record->return_type != "" && record->return_type != "void") {
			node->size -= 4;
		}
		while (stmt != nullptr) {
			node->size += stmt->size;
			stmt = stmt->right;
		}
		while (vardecl != nullptr) {
			node->size += vardecl->size;
			vardecl = vardecl->right;
		}
	}
}

void SizeSetterVisitor::visit(StatBlockNode *node) {
	ASTNode *stmt = node->leftmost_child;
	while (stmt != nullptr) {
		node->size -= stmt->size;
		stmt = stmt->right;
	}
}

void SizeSetterVisitor::visit(VarDeclListNode *node) {
	ASTNode *vardecl = node->leftmost_child;
	while (vardecl != nullptr) {
		node->size -= vardecl->size;
		vardecl = vardecl->right;
	}
}


/* Code Generation Visitor Definitions */

CodeGenerationVisitor::CodeGenerationVisitor() {
	cout << "initialized" << endl;
	for(int i = REGISTER_COUNT; i > 0; --i) {
		registers.push_back("r" + to_string(i));
	}
}

void CodeGenerationVisitor::visit(ProgNode *node) {
	Compiler::moon_code << "hlt" << endl;
	Compiler::moon_code << "buf res 20" << endl;
}
void CodeGenerationVisitor::visit(ClassDeclNode *node) {
}
void CodeGenerationVisitor::visit(VarDeclNode *node) {
	
}
void CodeGenerationVisitor::visit(FuncDefNode *node) {
	cout << "gta be doin somethin" << endl;
}

void CodeGenerationVisitor::visit(AddOpNode *node) {
	cout << "in app op node!" << endl;
}

void CodeGenerationVisitor::visit(IntLitNode *node) {
	// loads the int into a register
	string &reg = this->registers.back();
	Compiler::moon_code << "addi " << reg << ",r0," << node->val << endl;
	Compiler::moon_code << "sw " << node->record->offset << "(r14)," << reg << endl;
}
void CodeGenerationVisitor::visit(AssignStmtNode *node) {
	//ASTNode *thing = node->leftmost_child;
	//Compiler::moon_code << "sw " << node->record->offset << "(r14)," << reg << endl;
}
void CodeGenerationVisitor::visit(WriteStmtNode *node) {
	/*
	ASTNode *parent = node->parent;
	while (parent->table == nullptr) {
		parent = parent->parent;
	}
	Compiler::moon_code << "addi r14,r14," << to_string(parent->table->compute_size()) << endl; 

	Compiler::moon_code << "addi " << registers.back(), << "r0,buf" << endl; 

	// put this in the expression
	Compiler::moon_code << "subi r14,r14," << to_string(parent->table->compute_size()) << endl; 
	// ... print the expression
	*/
}

