#include <iostream>
#include "AST.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "Visitor.h"

using std::cout;
using std::endl;

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

CreatingVisitor::CreatingVisitor() { }

// do i pass the symboltable in the accept method or do i add to it after its done
// difference b/w main function and another free function is that you just don't have parameters

// there is a bug where the dot operator takes precedence over the assign statement:
//
// somevariable.a = B; // would have the dot operator be the parent operator when it should be the assignment operator
void CreatingVisitor::visit(ProgNode *node) {
	node->record = new SymbolTableRecord(node);
	node->record->link = new SymbolTable(node, node->get_type());
	ASTNode *main = node->leftmost_child;

	node->record->link->insert(main->record);

	ASTNode *funcdeflist = main->right;
	ASTNode *funcdef = funcdeflist->leftmost_child;
	while (funcdef != nullptr) {
		node->record->link->insert(funcdef->record);
		funcdef = funcdef->right;
	}
	ASTNode *classdecllist = funcdeflist->right;
	ASTNode *classdecl = classdecllist->leftmost_child;
	while (classdecl != nullptr) {
		node->record->link->insert(classdecl->record);
		classdecl = classdecl->right;
	}
	node->record->link->print();
}

void CreatingVisitor::visit(FuncDefNode *node) {
	
	node->record = new SymbolTableRecord(node);
	node->record->link = new SymbolTable(node, "function");
	node->record->kind = "function";

	ASTNode *funcbody = node->leftmost_child;
	ASTNode *statblock = funcbody->leftmost_child;
	ASTNode *vardecllist = statblock->right;

	ASTNode *vardecl = vardecllist->leftmost_child;

	while (vardecl != nullptr) {
		node->record->link->insert(vardecl->record);
		vardecl = vardecl->right;
	}

	if (funcbody->right != nullptr) { // is there a funchead
		ASTNode *funchead = funcbody->right;
		ASTNode *type = funchead->leftmost_child;
		node->record->types.push_back(type->leftmost_child->get_type());
		ASTNode *fparamlist = type->right;

		ASTNode *scopespec = fparamlist->right;
		ASTNode *id = nullptr;
		if (scopespec->leftmost_child->is_epsilon()) {
			id = scopespec->right;
		} else {
			id = scopespec->leftmost_child;
			node->record->base = scopespec->right->val;
		}

		ASTNode *fparam = fparamlist->leftmost_child;
		while (fparam != nullptr) {
			node->record->link->insert(fparam->record);
			fparam = fparam->right;
		}
		node->record->name = id->val;
		node->record->link->name = id->val;
	} else { // main function doesnt have a function head
		node->record->link->name = "main";
		node->record->name = "main";
	}
}

void CreatingVisitor::visit(ClassDeclNode *node) {
	node->record = new SymbolTableRecord(node);
	node->record->link = new SymbolTable(node, "class");
	// memberlist, inherlist, id
	ASTNode *memberlist = node->leftmost_child;
	ASTNode *inherlist = memberlist->right;
	ASTNode *id = inherlist->right;

	ASTNode *memberdecl = memberlist->leftmost_child;

	node->record->link->insert(inherlist->record);
	while (memberdecl != nullptr && !memberdecl->is_epsilon()) {
		ASTNode *funcdeclorvardecl = memberdecl->leftmost_child;
		ASTNode *visibility = funcdeclorvardecl->right;
		funcdeclorvardecl->record->types.push_back(visibility->get_type());
		node->record->link->insert(funcdeclorvardecl->record);
		memberdecl = memberdecl->right;
	}

	node->record->base = inherlist->leftmost_child->val;
	node->record->link->name = id->val;
	node->record->name = id->val;
	node->record->kind = "class";
}

void CreatingVisitor::visit(InherListNode *node) {
	node->record = new SymbolTableRecord(node);
	node->record->kind = "inherit";
	string inherval = "";
	ASTNode *inher_id = node->leftmost_child;
	while (inher_id != nullptr && !inher_id->is_epsilon()) {
		inherval += inher_id->val + ", ";
		inher_id = inher_id->right;
	}

	if (inherval == "") {
		inherval = "none";
	}

	node->record->name = inherval;
}

void CreatingVisitor::visit(FParamNode *node) {
	ASTNode *dimlist = node->leftmost_child;
	string id = dimlist->right->val;
	string type = dimlist->right->right->leftmost_child->get_type();

	ASTNode *dim = dimlist->leftmost_child;
	string dims = dim->get_dims();
	type += dims;

	node->record = new SymbolTableRecord(node);
	node->record->name = id;
	node->record->kind = "parameter";
	node->record->types.push_back(type);
}

void CreatingVisitor::visit(FuncDeclNode *node) {
	ASTNode *type = node->leftmost_child;
	ASTNode *fparamlist = type->right;
	ASTNode *id = fparamlist->right;
	node->record = new SymbolTableRecord(node);
	node->record->name = id->val;
	node->record->kind = "function";
	node->record->types.push_back(type->get_type());

	ASTNode *fparam = fparamlist->leftmost_child;

	while (fparam != nullptr && !fparam->is_epsilon()) {
		node->record->types.push_back(fparam->record->types[0]);
		fparam = fparam->right;
	}

}

void CreatingVisitor::visit(VarDeclNode *node) {
	ASTNode *dimlist = node->leftmost_child;
	string id = dimlist->right->val;
	string type = dimlist->right->right->leftmost_child->get_type();
	if (type == "id") {
		type = dimlist->right->right->leftmost_child->val;
	}

	string dims = dimlist->get_dims();
	//cout << dims << endl;
	type += dims;

	node->record = new SymbolTableRecord(node);
	node->record->name = id;
	node->record->kind = "variable";
	node->record->types.push_back(type);
}


CheckingVisitor::CheckingVisitor() { }

void CheckingVisitor::visit(ClassDeclNode *node) {
	// TODO: check for circular dependencies
	if (node->record->base != "") {
		// check in global table
		bool found = node->parent->parent->record->link->search(node->record->base, "classdecl");
		if (!found) {
			cout << "undeclared class " << node->record->base << endl;
			SemanticAnalyzer::semantic_errors << "undeclared class " << node->record->base << endl;
		} else {
			// check if circular
			auto record = node->parent->parent->record->link->records[node->record->base];
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
		bool assoc_class_declared = node->parent->parent->record->link->search(node->record->base, "classdecl");
		if (!assoc_class_declared) {
			cout << "undeclared class " << node->record->base << endl;
			SemanticAnalyzer::semantic_errors << "undeclared class " << node->record->base << endl;
		} else {
			auto table = node->parent->parent->record->link;
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
