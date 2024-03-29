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
void Visitor::visit(IntegerNode *node) { }
void Visitor::visit(FloatNode *node) { }
void Visitor::visit(StringNode *node) { }
void Visitor::visit(DimListNode *node) { }
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
void Visitor::visit(IndiceListNode *node) { }
void Visitor::visit(PrivateNode *node) { }
void Visitor::visit(PublicNode *node) { }
void Visitor::visit(DataMemberNode *node) { }
void Visitor::visit(VoidNode *node) { }

/**************************************
* Creating Visitor Definitions *
**************************************/

CreatingVisitor::CreatingVisitor() { }

void CreatingVisitor::visit(ProgNode *node) {
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
		dynamic_cast<FunctionSymbolTableRecord*>(node->record)->return_type = type->get_type() == "id" ? type->val : type->get_type();
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
	node->record = new ClassSymbolTableRecord(node);
	node->table = new SymbolTable(node, "class");
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
	ASTNode *type = node->leftmost_child;
	ASTNode *id = type->right;
	ASTNode *dimlist = id->right;

	DimListNode *dims = dynamic_cast<DimListNode*>(dimlist);
	string types_str = type->get_type() == "id" ? type->val : type->get_type();
	types_str += dims->get_dims_str();

	node->record = new ParamSymbolTableRecord(node);
	node->record->name = id->val;
	dynamic_cast<ParamSymbolTableRecord*>(node->record)->type = types_str;
}

void CreatingVisitor::visit(FuncDeclNode *node) {
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
	ASTNode *type = node->leftmost_child;
	ASTNode *id = type->right;
	ASTNode *dimlist = id->right;

	string type_val = type->get_type() == "id" ? type->val : type->get_type();

	type_val += dynamic_cast<DimListNode*>(dimlist)->get_dims_str();

	node->record = new VariableSymbolTableRecord(node);
	node->record->name = id->val;
	dynamic_cast<VariableSymbolTableRecord*>(node->record)->type = type_val;
}

void CreatingVisitor::visit(IntLitNode *node) {
	// if its used to set a dimension, it doesnt need moon code
	// true for fparam or vardecl
	if (node->parent->get_type() == "dimlist" || node->parent->parent->get_type() == "indicelist")
		return;
	node->record = new SymbolTableRecord(node);
	node->record->name = "t" + to_string(this->temp_count++);
	node->record->kind = "temp";
	node->record->type = "integer";
}

void CreatingVisitor::visit(FloatLitNode *node) {
	node->record = new SymbolTableRecord(node);
	node->record->name = "t" + to_string(this->temp_count++);
	node->record->kind = "temp";
	node->record->type = "float";
}

void CreatingVisitor::visit(AddOpNode *node) {
	node->record = new SymbolTableRecord(node);
	node->record->name = "t" + to_string(this->temp_count++);
	node->record->kind = "temp";
	node->record->type = "integer";
}

void CreatingVisitor::visit(MultOpNode *node) {
	node->record = new SymbolTableRecord(node);
	node->record->name = "t" + to_string(this->temp_count++);
	node->record->kind = "temp";
	node->record->type = "integer";
}

void CreatingVisitor::visit(FCallNode *node) {
	// holds the result of the fcall
	node->record = new SymbolTableRecord(node);
	node->record->name = "t" + to_string(this->temp_count++);
	node->record->kind = "temp";
}

/**************************************
* Checking Visitor Definitions *
**************************************/

CheckingVisitor::CheckingVisitor() { }

void CheckingVisitor::visit(ClassDeclNode *node) {
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

/**************************************
* Size Setter Visitor Definitions *
**************************************/

SizeSetterVisitor::SizeSetterVisitor() {
}

void SizeSetterVisitor::visit(ClassDeclNode *node) {
	for(const auto &record : node->table->records)
		node->size += record->node->size;
}
void SizeSetterVisitor::visit(IntegerNode *node) {
	node->size = 4;
}
void SizeSetterVisitor::visit(FloatNode *node) {
	node->size = 8;
}
void SizeSetterVisitor::visit(IntLitNode *node) {
	node->size = 4;
}
void SizeSetterVisitor::visit(FloatLitNode *node) {
	node->size = 8;
}

void SizeSetterVisitor::visit(FCallNode *node) {
	// TODO: search for funcdef using id to get its return type's size
	ASTNode *id = node->leftmost_child;
		// match function's actual size and return type
		SymbolTableRecord* rec = node->record->link->find_record(id->val, "funcdef");
		FuncHeadNode *funchead = dynamic_cast<FuncHeadNode*>(rec->node->leftmost_child);
		// set for future use
		node->funchead = funchead;
		ASTNode *return_type = funchead->leftmost_child->right->right->right;
		node->record->type = return_type->get_type() == "id" ? return_type->val : return_type->get_type();
		node->size = return_type->size;
}


void SizeSetterVisitor::visit(FParamNode *node) {
	ASTNode *type = node->leftmost_child;
	ASTNode *id = type->right;
	ASTNode *dimlist = id->right;

	DimListNode *dims = dynamic_cast<DimListNode*>(dimlist);
	vector<int> dimensions = dims->get_dims();
	int total_cells = 1;
	if (dimensions.size() >= 1) {
		for (const auto &dim : dimensions) {
			total_cells *= dim;
		}
	}
	node->size = total_cells * type->size;
}


void SizeSetterVisitor::visit(VarDeclNode *node) {
	ASTNode *type = node->leftmost_child;
	ASTNode *id = type->right;
	ASTNode *dimlist = id->right;
	DimListNode *dims = dynamic_cast<DimListNode*>(dimlist);
	vector<int> dimensions = dims->get_dims();
	int total_cells = 1;
	if (dimensions.size() >= 1) {
		for (const auto &dim : dimensions) {
			total_cells *= dim;
		}
	}

	node->size = total_cells * type->size;
}

void SizeSetterVisitor::visit(FuncHeadNode *node) {
	// main function doesnt have a funchead
	string id = node->leftmost_child->val;
	int return_size = node->leftmost_child->right->right->right->size;
	// update the funcdef's size to include the return value + jump
	node->parent->size += 4 + return_size; // jl/jr return
	// declare function
	Compiler::moon_code << "\t% load address to jump back to (to be used once done)" << endl;
	Compiler::moon_code << id << "\tsw\t" << "-4(r14),r15" << endl;
}

void SizeSetterVisitor::visit(FuncDefNode *node) {
	// if its main, it doesnt have a returnval and params
	int total = node->size * -1;
	for(const auto &record : node->table->records) {
		record->offset = total;
		node->size += record->node->size;
		total -= record->node->size;
	}
}

void SizeSetterVisitor::visit(AddOpNode *node) {
	// should have a record from the creatingvisitor, assumes both operands are of the same type (and therefore have the same size)
	ASTNode *left_op = node->leftmost_child->get_first_child_with_record();
	ASTNode *right_op = node->leftmost_child->right->right->get_first_child_with_record();
	node->size = 4;
	node->record->type = "integer";
}

void SizeSetterVisitor::visit(MultOpNode *node) {
	// should have a record from the creatingvisitor, assumes both operands are of the same type (and therefore have the same size)
	ASTNode *left_op = node->leftmost_child->get_first_child_with_record();
	node->size = 4;
	node->record->type = "integer";
}

void SizeSetterVisitor::visit(VariableNode *node) {
	ASTNode *parent = node->parent;
	ASTNode *id = node->leftmost_child;

	while (parent != nullptr && parent->get_type() != "funcdef")
		parent = parent->parent;
	// here we assume the variable was declared
	SymbolTableRecord *record = parent->table->has_name(id->val);
	if (record == nullptr) {
		cout << "error";
		return;
	}
	if (record->node->get_type() == "vardecl") {
		VariableSymbolTableRecord *matching_vardecl = dynamic_cast<VariableSymbolTableRecord*>(record);
		node->record = matching_vardecl;
		node->record->type = matching_vardecl->type;
		node->record->link = matching_vardecl->link;
		node->size = matching_vardecl->node->size;
	} else if (record->node->get_type() == "fparam") {
		ParamSymbolTableRecord *matching_fparam = dynamic_cast<ParamSymbolTableRecord*>(record);
		node->record = matching_fparam;
		node->record->type = matching_fparam->type;
		node->record->link = matching_fparam->link;
		node->size = matching_fparam->node->size;
	}
}

/**************************************
* Code Generation Visitor Definitions *
**************************************/

CodeGenerationVisitor::CodeGenerationVisitor() {
	// r0 isnt in this list because it always contains 0
	for(int i = REGISTER_COUNT; i > 0; --i) {
		registers.push_back("r" + to_string(i));
	}
}

void CodeGenerationVisitor::visit(ProgNode *node) {
	Compiler::moon_code << endl << "hlt" << endl;
	Compiler::moon_code << "buf res 20" << endl;
}
void CodeGenerationVisitor::visit(ClassDeclNode *node) {
}
void CodeGenerationVisitor::visit(VarDeclNode *node) {
	// no need to do anything even if indices, because of offset
	// akin to reserving its memory
}
void CodeGenerationVisitor::visit(FuncDefNode *node) {
}

void CodeGenerationVisitor::visit(AddOpNode *node) {
	this->process_arith_op(node);
}

void CodeGenerationVisitor::process_arith_op(ASTNode *node) {
	string instruction = "";
	if (dynamic_cast<AddOpNode*>(node) != nullptr)
		instruction = dynamic_cast<AddOpNode*>(node)->get_instruction();
	else
		instruction = dynamic_cast<MultOpNode*>(node)->get_instruction();
	ASTNode *left_op = node->leftmost_child->get_first_child_with_record();
	ASTNode *right_op = node->leftmost_child->right->right->get_first_child_with_record();

	int left_rel_offset = left_op->record->offset;
	int right_rel_offset = right_op->record->offset;

	if (left_op->get_type() == "variable") {
		left_rel_offset -= dynamic_cast<VariableNode*>(left_op)->get_cell_index() * node->size;
	}
	if (right_op->get_type() == "variable") {
		right_rel_offset -= dynamic_cast<VariableNode*>(right_op)->get_cell_index() * node->size;
	}

	string result_reg = this->registers.back(); this->registers.pop_back();
	string left_op_reg = this->registers.back(); this->registers.pop_back();
	string right_op_reg = this->registers.back(); this->registers.pop_back();

	Compiler::moon_code << "lw\t" << left_op_reg << "," << left_rel_offset << "(r14)" << endl;
	Compiler::moon_code << "lw\t" << right_op_reg << "," << right_rel_offset << "(r14)" << endl;

	// assume each register contains the required data

	Compiler::moon_code << instruction << "\t" << result_reg << "," << left_op_reg << "," << right_op_reg << endl;
	Compiler::moon_code << "sw\t" << node->record->offset << "(r14)," << result_reg << endl;
	this->registers.push_back(right_op_reg);
	this->registers.push_back(left_op_reg);
	this->registers.push_back(result_reg);
}

void CodeGenerationVisitor::visit(MultOpNode *node) {
	this->process_arith_op(node);
}

void CodeGenerationVisitor::visit(IntLitNode *node) {
	// if its used to set a dimension, it doesnt need a temp var
	if (node->parent->get_type() == "dimlist" || node->parent->parent->get_type() == "indicelist")
		return;
	string &reg = this->registers.back();
	this->registers.pop_back();
	Compiler::moon_code << "\t%load int literal into register" << endl;
	Compiler::moon_code << "addi\t" << reg << ",r0," << node->val << endl;
	Compiler::moon_code << "sw\t" << node->record->offset << "(r14)," << reg << endl;
	this->registers.push_back(reg);
}
void CodeGenerationVisitor::visit(AssignStmtNode *node) {
	ASTNode *left_op = node->leftmost_child->get_first_child_with_record();
	ASTNode *right_op = node->leftmost_child->right->get_first_child_with_record();
	if (left_op == nullptr || right_op == nullptr) {
		string null_node = left_op == nullptr ? "left" : "right";
		cout << null_node << " is null " << endl;
	}

	int left_rel_offset = left_op->record->offset;
	if (left_op->get_type() == "variable") { // most likely
		left_rel_offset -= dynamic_cast<VariableNode*>(left_op)->get_cell_index() * 4;
	}

	string reg = this->registers.back(); this->registers.pop_back();

	Compiler::moon_code << "lw\t" << reg << "," << right_op->record->offset << "(r14)" << endl;
	Compiler::moon_code << "sw\t" << left_rel_offset << "(r14)," << reg << endl;
	this->registers.push_back(reg);
}
void CodeGenerationVisitor::visit(WriteStmtNode *node) {
	string param_reg = this->registers.back(); this->registers.pop_back();
	string buf_reg = this->registers.back(); this->registers.pop_back();
	ASTNode *child = node->get_first_child_with_record();
	int table_size = child->record->link->compute_size() * -1;
	int child_rel_offset = child->record->offset;
	if (child->get_type() == "variable") { // most likely
		child_rel_offset -= dynamic_cast<VariableNode*>(child)->get_cell_index() * 4;
	}

	Compiler::moon_code << "lw\t" << param_reg << "," << child_rel_offset << "(r14)" << endl;
	Compiler::moon_code << "\t%increase stack frame" << endl;
	Compiler::moon_code << "addi\tr14,r14," << table_size << endl;
	// put whatever u wanna print onto stack @ -8(r14), has to be -8... (using register from previous step)
	Compiler::moon_code << "sw\t-8(r14)," << param_reg << endl;
	// put buffer address in a register
	Compiler::moon_code << "addi\t" << buf_reg << "," << "r0,buf" << endl;
	// put buffer address on stack @ -12(r14), has to be -12...
	Compiler::moon_code << "sw\t-12(r14)," << buf_reg << endl;
	Compiler::moon_code << "\t%convert what's @ -8(r14) into a string, result will be in r13" << endl;
	Compiler::moon_code << "jl\tr15, intstr" << endl;
	Compiler::moon_code << "\t%put what's in r13 in -8(r14) due to call to putstr" << endl;
	Compiler::moon_code << "sw\t-8(r14),r13" << endl;
	Compiler::moon_code << "\t%print what's @ -8(r14)" << endl;
	Compiler::moon_code << "jl\tr15, putstr" << endl;
	Compiler::moon_code << "\t%decrease stack frame" << endl;
	Compiler::moon_code << "subi\tr14,r14," << table_size << endl;
	
	this->registers.push_back(buf_reg);
	this->registers.push_back(param_reg);
}

void CodeGenerationVisitor::visit(ReadStmtNode *node) {
	ASTNode *child = node->get_first_child_with_record();
	if (child->get_type() != "variable") { // must read into a variable
		return;
	}
	int child_rel_offset = child->record->offset - (dynamic_cast<VariableNode*>(child)->get_cell_index() * 4);
	string reg = this->registers.back(); this->registers.pop_back();
	int table_size = child->record->link->compute_size() * -1;
	Compiler::moon_code << "\t%load address of buffer into register" << endl;
	Compiler::moon_code << "addi\t" << reg << ",r0,buf" << endl;
	Compiler::moon_code << "\t%increase stack frame" << endl;
	Compiler::moon_code << "addi\tr14,r14," << table_size << endl;
	Compiler::moon_code << "\t%store address of buffer into -8(r14), required by getstr" << endl;
	Compiler::moon_code << "sw\t" << "-8(r14)," << reg << endl;
	Compiler::moon_code << "\t%jump to function, result will be in address that was in -8(r14)" << endl;
	Compiler::moon_code << "jl\tr15, getstr" << endl;
	Compiler::moon_code << "jl\tr15, strint" << endl;
	Compiler::moon_code << "\t%decrease stack frame" << endl;
	Compiler::moon_code << "subi\tr14,r14," << table_size << endl;
	Compiler::moon_code << "\t%get contents of buffer" << endl;
	Compiler::moon_code << "sw\t" << child_rel_offset << "(r14)," << "r13" << endl;
	this->registers.push_back(reg);
}

void CodeGenerationVisitor::visit(FCallNode *node) {
	// what do i need to make this function call? i need to get the offsets of the params im going to pass, how do i get those? i need the funchead
	vector<SymbolTableRecord*> temp_args;
	vector<SymbolTableRecord*> head_args;

	ASTNode *param = node->funchead->leftmost_child->right->right->leftmost_child;
	ASTNode *arg = node->leftmost_child->right->leftmost_child;
	int funcdef_size = node->record->link->compute_size() * -1;
	while (param != nullptr && arg != nullptr) {
		if (!param->is_epsilon() && !arg->is_epsilon()) {
			ASTNode *temp_arg = arg->get_first_child_with_record();
			SymbolTableRecord *param_record = param->record;
			int target_offset = funcdef_size + param_record->offset;
			cout << target_offset << endl;
			string reg = this->registers.back(); this->registers.pop_back();
			Compiler::moon_code << "\t%load value argument into register" << endl;
			Compiler::moon_code << "lw\t" << reg << "," << temp_arg->record->offset << "(r14)" << endl;
			Compiler::moon_code << "sw\t" << to_string(target_offset) << "(r14)," << reg << endl;
			this->registers.push_back(reg);
		}
		param = param->right;
		arg = arg->right;
	}

	int return_type_size = node->funchead->leftmost_child->right->right->right->size;
	int return_offset = funcdef_size - return_type_size;
	string reg = this->registers.back(); this->registers.pop_back();
	cout << return_type_size << endl;
	Compiler::moon_code << "\t%increase stack frame" << endl;
	Compiler::moon_code << "addi\tr14,r14," << to_string(funcdef_size) << endl;
	Compiler::moon_code << "\t%jump and link to function" << endl;
	Compiler::moon_code << "jl\tr15," << node->funchead->leftmost_child->val << endl;
	Compiler::moon_code << "\t%function done, decrease stack frame" << endl;
	Compiler::moon_code << "subi\tr14,r14," << to_string(funcdef_size) << endl;
	Compiler::moon_code << "\t%get return value from f" << endl;
	Compiler::moon_code << "lw\t" << reg << "," << to_string(return_offset) << "(r14)" << endl;
	Compiler::moon_code << "sw\t" << to_string(node->record->offset) << "(r14)," << reg << endl;
	this->registers.push_back(reg);
}

void CodeGenerationVisitor::visit(ReturnStmtNode *node) {
	ASTNode *retval = node->get_first_child_with_record();

	string reg = this->registers.back(); this->registers.pop_back();
	Compiler::moon_code << "\t%load the return value into register" << endl;
	Compiler::moon_code << "lw\t" << reg << "," << retval->record->offset << "(r14)" << endl;
	Compiler::moon_code << "\t%put return value at first cell of function" << endl;
	Compiler::moon_code << "sw\t" << "0(r14)," << reg << endl;
	Compiler::moon_code << "\t%load return address" << endl;
	Compiler::moon_code << "lw\t" "r15,-4(r14)" << endl;
	Compiler::moon_code << "\t%jump to return address" << endl;
	Compiler::moon_code << "jr\tr15" << endl << endl;
	this->registers.push_back(reg);
}
