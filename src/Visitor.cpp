#include <iostream>
#include "AST.h"
#include "SymbolTable.h"
#include "Visitor.h"

using std::cout;
using std::endl;

Visitor::Visitor() { }
//void Visitor::visit(ASTNode *node) { cout << "ok" << endl;}

CreatingVisitor::CreatingVisitor() { }

void CreatingVisitor::visit(ProgNode *node) {
	node->table = new SymbolTable("Global");
	ASTNode *child = node->leftmost_child;
	cout << child->get_type() << endl; // funcbody
	cout << child->right->get_type() << endl; // funcdeflist
	cout << child->right->right->get_type() << endl; // classdecllist
}
void CreatingVisitor::visit(ClassDeclListNode *node) {
}
void CreatingVisitor::visit(ClassDeclNode *node) {
}
void CreatingVisitor::visit(MembListNode *node) {
}
void CreatingVisitor::visit(InherListNode *node) {
}
void CreatingVisitor::visit(FuncDefListNode *node) {
}
void CreatingVisitor::visit(FuncDefNode *node) {
}
void CreatingVisitor::visit(FuncBodyNode *node) {
}
void CreatingVisitor::visit(FuncHeadNode *node) {
}
void CreatingVisitor::visit(FuncDeclNode *node) {
}
void CreatingVisitor::visit(ParamListNode *node) {
}
void CreatingVisitor::visit(FParamNode *node) {
}
void CreatingVisitor::visit(FCallNode *node) {
}
void CreatingVisitor::visit(VisibilityNode *node) {
}
void CreatingVisitor::visit(MemberDeclNode *node) {
}
void CreatingVisitor::visit(IdNode *node) {
}
void CreatingVisitor::visit(VarDeclNode *node) {
}
void CreatingVisitor::visit(VarDeclListNode *node) {
}
void CreatingVisitor::visit(TypeNode *node) {
}
void CreatingVisitor::visit(IntegerNode *node) {
}
void CreatingVisitor::visit(FloatNode *node) {
}
void CreatingVisitor::visit(StringNode *node) {
}
void CreatingVisitor::visit(DimListNode *node) {
}
void CreatingVisitor::visit(NumIntNode *node) {
}
void CreatingVisitor::visit(StatementNode *node) {
}
void CreatingVisitor::visit(StatBlockNode *node) {
}
void CreatingVisitor::visit(VariableNode *node) {
}
void CreatingVisitor::visit(AssignStmtNode *node) {
}
void CreatingVisitor::visit(IfStmtNode *node) {
}
void CreatingVisitor::visit(ReadStmtNode *node) {
}
void CreatingVisitor::visit(WriteStmtNode *node) {
}
void CreatingVisitor::visit(ReturnStmtNode *node) {
}
void CreatingVisitor::visit(ContinueNode *node) {
}
void CreatingVisitor::visit(BreakNode *node) {
}
void CreatingVisitor::visit(ArithExprNode *node) {
}
void CreatingVisitor::visit(RelExprNode *node) {
}
void CreatingVisitor::visit(WhileStmtNode *node) {
}
void CreatingVisitor::visit(SignNode *node) {
}
void CreatingVisitor::visit(PlusNode *node) {
}
void CreatingVisitor::visit(MinusNode *node) {
}
void CreatingVisitor::visit(RelOpNode *node) {
}
void CreatingVisitor::visit(MultOpNode *node) {
}
void CreatingVisitor::visit(AddOpNode *node) {
}
void CreatingVisitor::visit(MultNode *node) {
}
void CreatingVisitor::visit(DivNode *node) {
}
void CreatingVisitor::visit(AndNode *node) {
}
void CreatingVisitor::visit(EqNode *node) {
}
void CreatingVisitor::visit(GtNode *node) {
}
void CreatingVisitor::visit(LtNode *node) {
}
void CreatingVisitor::visit(NeqNode *node) {
}
void CreatingVisitor::visit(GeqNode *node) {
}
void CreatingVisitor::visit(LeqNode *node) {
}
void CreatingVisitor::visit(IntLitNode *node) {
}
void CreatingVisitor::visit(StringLitNode *node) {
}
void CreatingVisitor::visit(FloatLitNode *node) {
}
void CreatingVisitor::visit(DotNode *node) {
}
void CreatingVisitor::visit(EpsilonNode *node) {
}
void CreatingVisitor::visit(ScopeSpecNode *node) {
}
void CreatingVisitor::visit(TermNode *node) {
}
void CreatingVisitor::visit(IndiceListNode *node) {
}
void CreatingVisitor::visit(PrivateNode *node) {
}
void CreatingVisitor::visit(PublicNode *node) {
}
void CreatingVisitor::visit(DataMemberNode *node) {
}


CheckingVisitor::CheckingVisitor() { }

void CheckingVisitor::visit(ProgNode *node) {
}
void CheckingVisitor::visit(ClassDeclListNode *node) {
}
void CheckingVisitor::visit(ClassDeclNode *node) {
}
void CheckingVisitor::visit(MembListNode *node) {
}
void CheckingVisitor::visit(InherListNode *node) {
}
void CheckingVisitor::visit(FuncDefListNode *node) {
}
void CheckingVisitor::visit(FuncDefNode *node) {
}
void CheckingVisitor::visit(FuncBodyNode *node) {
}
void CheckingVisitor::visit(FuncHeadNode *node) {
}
void CheckingVisitor::visit(FuncDeclNode *node) {
}
void CheckingVisitor::visit(ParamListNode *node) {
}
void CheckingVisitor::visit(FParamNode *node) {
}
void CheckingVisitor::visit(FCallNode *node) {
}
void CheckingVisitor::visit(VisibilityNode *node) {
}
void CheckingVisitor::visit(MemberDeclNode *node) {
}
void CheckingVisitor::visit(IdNode *node) {
}
void CheckingVisitor::visit(VarDeclNode *node) {
}
void CheckingVisitor::visit(VarDeclListNode *node) {
}
void CheckingVisitor::visit(TypeNode *node) {
}
void CheckingVisitor::visit(IntegerNode *node) {
}
void CheckingVisitor::visit(FloatNode *node) {
}
void CheckingVisitor::visit(StringNode *node) {
}
void CheckingVisitor::visit(DimListNode *node) {
}
void CheckingVisitor::visit(NumIntNode *node) {
}
void CheckingVisitor::visit(StatementNode *node) {
}
void CheckingVisitor::visit(StatBlockNode *node) {
}
void CheckingVisitor::visit(VariableNode *node) {
}
void CheckingVisitor::visit(AssignStmtNode *node) {
}
void CheckingVisitor::visit(IfStmtNode *node) {
}
void CheckingVisitor::visit(ReadStmtNode *node) {
}
void CheckingVisitor::visit(WriteStmtNode *node) {
}
void CheckingVisitor::visit(ReturnStmtNode *node) {
}
void CheckingVisitor::visit(ContinueNode *node) {
}
void CheckingVisitor::visit(BreakNode *node) {
}
void CheckingVisitor::visit(ArithExprNode *node) {
}
void CheckingVisitor::visit(RelExprNode *node) {
}
void CheckingVisitor::visit(WhileStmtNode *node) {
}
void CheckingVisitor::visit(SignNode *node) {
}
void CheckingVisitor::visit(PlusNode *node) {
}
void CheckingVisitor::visit(MinusNode *node) {
}
void CheckingVisitor::visit(RelOpNode *node) {
}
void CheckingVisitor::visit(MultOpNode *node) {
}
void CheckingVisitor::visit(AddOpNode *node) {
}
void CheckingVisitor::visit(MultNode *node) {
}
void CheckingVisitor::visit(DivNode *node) {
}
void CheckingVisitor::visit(AndNode *node) {
}
void CheckingVisitor::visit(EqNode *node) {
}
void CheckingVisitor::visit(GtNode *node) {
}
void CheckingVisitor::visit(LtNode *node) {
}
void CheckingVisitor::visit(NeqNode *node) {
}
void CheckingVisitor::visit(GeqNode *node) {
}
void CheckingVisitor::visit(LeqNode *node) {
}
void CheckingVisitor::visit(IntLitNode *node) {
}
void CheckingVisitor::visit(StringLitNode *node) {
}
void CheckingVisitor::visit(FloatLitNode *node) {
}
void CheckingVisitor::visit(DotNode *node) {
}
void CheckingVisitor::visit(EpsilonNode *node) {
}
void CheckingVisitor::visit(ScopeSpecNode *node) {
}
void CheckingVisitor::visit(TermNode *node) {
}
void CheckingVisitor::visit(IndiceListNode *node) {
}
void CheckingVisitor::visit(PrivateNode *node) {
}
void CheckingVisitor::visit(PublicNode *node) {
}
void CheckingVisitor::visit(DataMemberNode *node) {
}
