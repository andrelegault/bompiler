#include <string>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <utility>
#include "Visitor.h"
#include "AST.h"

using std::string;
using std::cout;
using std::queue;
using std::endl;
using std::unordered_map;
using std::to_string;

ASTNode::ASTNode(string val) : val(std::move(val)) { }

ASTNode* ASTNode::make_siblings(ASTNode *y) {
	/* makes this a sibling of y where `this` becomes the leftmost sibling if y doesn't have a leftmode sibling.*/
	// `this` inherits the parents of y, not the other way around.
	ASTNode *xsibs = this;
	while (xsibs->right != nullptr)
		xsibs = xsibs->right;

	ASTNode *ysibs = y->leftmost_sibling == nullptr ? y : y->leftmost_sibling;
	xsibs->right = ysibs;
	ysibs->leftmost_sibling = xsibs->leftmost_sibling == nullptr ? xsibs: xsibs->leftmost_sibling;
	ysibs->parent = xsibs->parent;

	while (ysibs->right != nullptr) {
		ysibs = ysibs->right;
		ysibs->leftmost_sibling = xsibs->leftmost_sibling == nullptr ? xsibs: xsibs->leftmost_sibling;
		ysibs->parent = xsibs->parent;
	}

	return ysibs;
}

/*
void ASTNode::accept(CreatingVisitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void ASTNode::accept(CheckingVisitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}
*/

void ASTNode::adopt_children(ASTNode *y) {
	if (this->leftmost_child != nullptr) {
		this->leftmost_child->make_siblings(y);
	} else {
		ASTNode *ysibs = y->leftmost_sibling == nullptr ? y : y->leftmost_sibling;
		this->leftmost_child = ysibs;
		while (ysibs != nullptr) {
			ysibs->parent = this;
			ysibs = ysibs->right;
		}
	}
}

string ASTNode::to_dot_notation() {
	string str = "digraph G {\n";
	queue<ASTNode*> container;
	unordered_map<string, int> children_count, parent_count;
	container.push(this);
	while (!container.empty()) {
		ASTNode *parent = container.front();
		container.pop();
		string parent_val = parent->get_type();
		parent_val += to_string(parent_count[parent_val]++);
		ASTNode *start = parent->leftmost_child;
		while (start != nullptr) {
			container.push(start);
			string start_val = start->get_type();
			start_val += to_string(children_count[start_val]++);
			str +=  "\t" + parent_val + "->" + start_val + "\n";
			start = start->right;
		}
	}
	str += "}\n";
	return str;
}

ASTNode *ASTNode::make_family(string &op, const vector<ASTNode*> &children) {
	ASTNode *node = ASTNode::make_node(op);
	ASTNode *base = children.front();
	for(int i = 1; i < children.size(); ++i) {
		base->make_siblings(children[i]);
	}
	node->adopt_children(children.back());
	return node;
}

bool ASTNode::is_epsilon() {
	return false;
}

ASTNode *ASTNode::make_node(const string &type, const string &val) {
	ASTNode *result = nullptr;
	if (type == "prog")
		result = new ProgNode();
	else if (type == "classdecllist")
		result = new ClassDeclListNode();
	else if (type == "classdecl")
		result = new ClassDeclNode();
	else if (type == "memblist")
		result = new MembListNode();
	else if (type == "inherlist")
		result = new InherListNode();
	else if (type == "funcdeflist")
		result = new FuncDefListNode();
	else if (type == "funcdef")
		result = new FuncDefNode();
	else if (type == "funcbody")
		result = new FuncBodyNode();
	else if (type == "funchead")
		result = new FuncHeadNode();
	else if (type == "funcdecl")
		result = new FuncDeclNode();
	else if (type == "paramlist")
		result = new ParamListNode();
	else if (type == "fparam")
		result = new FParamNode();
	else if (type == "fcall")
		result = new FCallNode();
	else if (type == "visibility")
		result = new VisibilityNode();
	else if (type == "memberdecl")
		result = new MemberDeclNode();
	else if (type == "id")
		result = new IdNode(val);
	else if (type == "vardecl")
		result = new VarDeclNode();
	else if (type == "vardecllist")
		result = new VarDeclListNode();
	else if (type == "type")
		result = new TypeNode();
	else if (type == "dimlist")
		result = new DimListNode();
	else if (type == "integer")
		result = new IntegerNode();
	else if (type == "float")
		result = new FloatNode();
	else if (type == "string")
		result = new StringNode();
	else if (type == "numint")
		result = new NumIntNode();
	else if (type == "statement")
		result = new StatementNode();
	else if (type == "statblock")
		result = new StatBlockNode();
	else if (type == "variable")
		result = new VariableNode();
	else if (type == "assignstmt")
		result = new AssignStmtNode();
	else if (type == "ifstmt")
		result = new IfStmtNode();
	else if (type == "readstmt")
		result = new ReadStmtNode();
	else if (type == "writestmt")
		result = new WriteStmtNode();
	else if (type == "returnstmt")
		result = new ReturnStmtNode();
	else if (type == "continue")
		result = new ContinueNode();
	else if (type == "break")
		result = new BreakNode();
	else if (type == "arithexpr")
		result = new ArithExprNode();
	else if (type == "relexpr")
		result = new RelExprNode();
	else if (type == "whilestmt")
		result = new WhileStmtNode();
	else if (type == "sign")
		result = new SignNode();
	else if (type == "plus")
		result = new PlusNode();
	else if (type == "minus")
		result = new MinusNode();
	else if (type == "relop")
		result = new RelOpNode();
	else if (type == "multop")
		result = new MultOpNode();
	else if (type == "addop")
		result = new AddOpNode();
	else if (type == "mult")
		result = new MultNode();
	else if (type == "div")
		result = new DivNode();
	else if (type == "and")
		result = new AndNode();
	else if (type == "eq")
		result = new EqNode();
	else if (type == "gt")
		result = new GtNode();
	else if (type == "lt")
		result = new LtNode();
	else if (type == "neq")
		result = new NeqNode();
	else if (type == "geq")
		result = new GeqNode();
	else if (type == "leq")
		result = new LeqNode();
	else if (type == "intlit")
		result = new IntLitNode(val);
	else if (type == "stringlit")
		result = new StringLitNode(val);
	else if (type == "floatlit")
		result = new FloatLitNode(val);
	else if (type == "dot")
		result = new DotNode();
	else if (type == "epsilon")
		result = new EpsilonNode();
	else if (type == "scopespec")
		result = new ScopeSpecNode();
	else if (type == "term")
		result = new TermNode();
	else if (type == "indicelist")
		result = new IndiceListNode();
	else if (type == "private")
		result = new PrivateNode();
	else if (type == "public")
		result = new PublicNode();
	else if (type == "datamember")
		result = new DataMemberNode();
	else {
		cout << type << endl;
		throw type;
	}
	return result;
}

ProgNode::ProgNode() { }
ClassDeclListNode::ClassDeclListNode() { }
ClassDeclNode::ClassDeclNode() { }
MembListNode::MembListNode() { }
InherListNode::InherListNode() { }
FuncDefListNode::FuncDefListNode() { }
FuncDefNode::FuncDefNode() { }
FuncBodyNode::FuncBodyNode() { }
FuncHeadNode::FuncHeadNode() { }
FuncDeclNode::FuncDeclNode() { }
ParamListNode::ParamListNode() { }
FParamNode::FParamNode() { }
FCallNode::FCallNode() { }
VisibilityNode::VisibilityNode() { }
MemberDeclNode::MemberDeclNode() { }
IdNode::IdNode(string val) : ASTNode(val) { }
VarDeclNode::VarDeclNode() { }
VarDeclListNode::VarDeclListNode() { }
TypeNode::TypeNode() { }
IntegerNode::IntegerNode() { }
FloatNode::FloatNode() { }
StringNode::StringNode() { }
DimListNode::DimListNode() { }
NumIntNode::NumIntNode() { }
StatementNode::StatementNode() { }
StatBlockNode::StatBlockNode() { }
VariableNode::VariableNode() { }
AssignStmtNode::AssignStmtNode() { }
IfStmtNode::IfStmtNode() { }
ReadStmtNode::ReadStmtNode() { }
WriteStmtNode::WriteStmtNode() { }
ReturnStmtNode::ReturnStmtNode() { }
ContinueNode::ContinueNode() { }
BreakNode::BreakNode() { }
ArithExprNode::ArithExprNode() { }
RelExprNode::RelExprNode() { }
WhileStmtNode::WhileStmtNode() { }
SignNode::SignNode() { }
PlusNode::PlusNode() { }
MinusNode::MinusNode() { }
RelOpNode::RelOpNode() { }
MultOpNode::MultOpNode() { }
AddOpNode::AddOpNode() { }
MultNode::MultNode() { }
DivNode::DivNode() { }
AndNode::AndNode() { }
EqNode::EqNode() { }
GtNode::GtNode() { }
LtNode::LtNode() { }
NeqNode::NeqNode() { }
GeqNode::GeqNode() { }
LeqNode::LeqNode() { }
IntLitNode::IntLitNode(string val) : ASTNode(val) { }
StringLitNode::StringLitNode(string val) : ASTNode(val) { }
FloatLitNode::FloatLitNode(string val) : ASTNode(val) { }
DotNode::DotNode() { }
EpsilonNode::EpsilonNode() { }
ScopeSpecNode::ScopeSpecNode() { }
TermNode::TermNode() { }
IndiceListNode::IndiceListNode() { }
PrivateNode::PrivateNode() { }
PublicNode::PublicNode() { }
DataMemberNode::DataMemberNode() { }

bool EpsilonNode::is_epsilon() {
	return true;
}

string ProgNode::get_type() {return "ProgNode"; }
string ClassDeclListNode::get_type() {return "ClassDeclListNode"; }
string ClassDeclNode::get_type() {return "ClassDeclNode"; }
string MembListNode::get_type() {return "MembListNode"; }
string InherListNode::get_type() {return "InherListNode"; }
string FuncDefListNode::get_type() {return "FuncDefListNode"; }
string FuncDefNode::get_type() {return "FuncDefNode"; }
string FuncBodyNode::get_type() {return "FuncBodyNode"; }
string FuncHeadNode::get_type() {return "FuncHeadNode"; }
string FuncDeclNode::get_type() {return "FuncDeclNode"; }
string ParamListNode::get_type() {return "ParamListNode"; }
string FParamNode::get_type() {return "FParamNode"; }
string FCallNode::get_type() {return "FCallNode"; }
string VisibilityNode::get_type() {return "VisibilityNode"; }
string MemberDeclNode::get_type() {return "MemberDeclNode"; }
string IdNode::get_type() {return "IdNode"; }
string VarDeclNode::get_type() {return "VarDeclNode"; }
string VarDeclListNode::get_type() {return "VarDeclListNode"; }
string TypeNode::get_type() {return "TypeNode"; }
string IntegerNode::get_type() {return "IntegerNode"; }
string FloatNode::get_type() {return "FloatNode"; }
string StringNode::get_type() {return "StringNode"; }
string DimListNode::get_type() {return "DimListNode"; }
string NumIntNode::get_type() {return "NumIntNode"; }
string StatementNode::get_type() {return "StatementNode"; }
string StatBlockNode::get_type() {return "StatBlockNode"; }
string VariableNode::get_type() {return "VariableNode"; }
string AssignStmtNode::get_type() {return "AssignStmtNode"; }
string IfStmtNode::get_type() {return "IfStmtNode"; }
string ReadStmtNode::get_type() {return "ReadStmtNode"; }
string WriteStmtNode::get_type() {return "WriteStmtNode"; }
string ReturnStmtNode::get_type() {return "ReturnStmtNode"; }
string ContinueNode::get_type() {return "ContinueNode"; }
string BreakNode::get_type() {return "BreakNode"; }
string ArithExprNode::get_type() {return "ArithExprNode"; }
string RelExprNode::get_type() {return "RelExprNode"; }
string WhileStmtNode::get_type() {return "WhileStmtNode"; }
string SignNode::get_type() {return "SignNode"; }
string PlusNode::get_type() {return "PlusNode"; }
string MinusNode::get_type() {return "MinusNode"; }
string RelOpNode::get_type() {return "RelOpNode"; }
string MultOpNode::get_type() {return "MultOpNode"; }
string AddOpNode::get_type() {return "AddOpNode"; }
string MultNode::get_type() {return "MultNode"; }
string DivNode::get_type() {return "DivNode"; }
string AndNode::get_type() {return "AndNode"; }
string EqNode::get_type() {return "EqNode"; }
string GtNode::get_type() {return "GtNode"; }
string LtNode::get_type() {return "LtNode"; }
string NeqNode::get_type() {return "NeqNode"; }
string GeqNode::get_type() {return "GeqNode"; }
string LeqNode::get_type() {return "LeqNode"; }
string IntLitNode::get_type() {return "IntLitNode"; }
string StringLitNode::get_type() {return "StringLitNode"; }
string FloatLitNode::get_type() {return "FloatLitNode"; }
string DotNode::get_type() {return "DotNode"; }
string EpsilonNode::get_type() {return "EpsilonNode"; }
string ScopeSpecNode::get_type() {return "ScopeSpecNode"; }
string TermNode::get_type() {return "TermNode"; }
string IndiceListNode::get_type() {return "IndiceListNode"; }
string PrivateNode::get_type() {return "PrivateNode"; }
string PublicNode::get_type() {return "PublicNode"; }
string DataMemberNode::get_type() {return "DataMemberNode"; }

void ProgNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void ClassDeclListNode::accept(Visitor *v) { 
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void ClassDeclNode::accept(Visitor *v) { 
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void MembListNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void InherListNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void FuncDefListNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void FuncDefNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void FuncBodyNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void FuncHeadNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void FuncDeclNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void ParamListNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void FParamNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void FCallNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void VisibilityNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void MemberDeclNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void IdNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void VarDeclNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void VarDeclListNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void TypeNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void IntegerNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void FloatNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void StringNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void DimListNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void NumIntNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void StatementNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void StatBlockNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void VariableNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void AssignStmtNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void IfStmtNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void ReadStmtNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void WriteStmtNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void ReturnStmtNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void ContinueNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void BreakNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void ArithExprNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void RelExprNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void WhileStmtNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void SignNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void PlusNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void MinusNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void RelOpNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void MultOpNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void AddOpNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void MultNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void DivNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void AndNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void EqNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void GtNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void LtNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void NeqNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void GeqNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void LeqNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void IntLitNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void StringLitNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void FloatLitNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void DotNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void EpsilonNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void ScopeSpecNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void TermNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void IndiceListNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void PrivateNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void PublicNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

void DataMemberNode::accept(Visitor *v) {
	ASTNode *left = this->leftmost_child;
	while (left != nullptr) {
		left->accept(v);
		left = left->right;
	}
	v->visit(this);
}

