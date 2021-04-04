#include "AST.h"
#include "SymbolTable.h"
#include "Visitor.h"

Visitor::Visitor() { }
void Visitor::visit(ASTNode *node) { }

CreationVisitor::CreationVisitor() { }

void CreationVisitor::visit(ProgNode *node) {
	node->table = new SymbolTable("Global");
	node->accept(*this);
	ASTNode *child = node->leftmost_child;
}
void CreationVisitor::visit(ClassDeclListNode *node) {
}
void CreationVisitor::visit(ClassDeclNode *node) {
}
void CreationVisitor::visit(MembListNode *node) {
}
void CreationVisitor::visit(InherListNode *node) {
}
void CreationVisitor::visit(FuncDefListNode *node) {
}
void CreationVisitor::visit(FuncDefNode *node) {
}
void CreationVisitor::visit(FuncBodyNode *node) {
}
void CreationVisitor::visit(FuncHeadNode *node) {
}
void CreationVisitor::visit(FuncDeclNode *node) {
}
void CreationVisitor::visit(ParamListNode *node) {
}
void CreationVisitor::visit(FParamNode *node) {
}
void CreationVisitor::visit(FCallNode *node) {
}
void CreationVisitor::visit(VisibilityNode *node) {
}
void CreationVisitor::visit(MemberDeclNode *node) {
}
void CreationVisitor::visit(IdNode *node) {
}
void CreationVisitor::visit(VarDeclNode *node) {
}
void CreationVisitor::visit(VarDeclListNode *node) {
}
void CreationVisitor::visit(TypeNode *node) {
}
void CreationVisitor::visit(IntegerNode *node) {
}
void CreationVisitor::visit(FloatNode *node) {
}
void CreationVisitor::visit(StringNode *node) {
}
void CreationVisitor::visit(DimListNode *node) {
}
void CreationVisitor::visit(NumIntNode *node) {
}
void CreationVisitor::visit(StatementNode *node) {
}
void CreationVisitor::visit(StatBlockNode *node) {
}
void CreationVisitor::visit(VariableNode *node) {
}
void CreationVisitor::visit(AssignStmtNode *node) {
}
void CreationVisitor::visit(IfStmtNode *node) {
}
void CreationVisitor::visit(ReadStmtNode *node) {
}
void CreationVisitor::visit(WriteStmtNode *node) {
}
void CreationVisitor::visit(ReturnStmtNode *node) {
}
void CreationVisitor::visit(ContinueNode *node) {
}
void CreationVisitor::visit(BreakNode *node) {
}
void CreationVisitor::visit(ArithExprNode *node) {
}
void CreationVisitor::visit(RelExprNode *node) {
}
void CreationVisitor::visit(WhileStmtNode *node) {
}
void CreationVisitor::visit(SignNode *node) {
}
void CreationVisitor::visit(PlusNode *node) {
}
void CreationVisitor::visit(MinusNode *node) {
}
void CreationVisitor::visit(RelOpNode *node) {
}
void CreationVisitor::visit(MultOpNode *node) {
}
void CreationVisitor::visit(AddOpNode *node) {
}
void CreationVisitor::visit(MultNode *node) {
}
void CreationVisitor::visit(DivNode *node) {
}
void CreationVisitor::visit(AndNode *node) {
}
void CreationVisitor::visit(EqNode *node) {
}
void CreationVisitor::visit(GtNode *node) {
}
void CreationVisitor::visit(LtNode *node) {
}
void CreationVisitor::visit(NeqNode *node) {
}
void CreationVisitor::visit(GeqNode *node) {
}
void CreationVisitor::visit(LeqNode *node) {
}
void CreationVisitor::visit(IntLitNode *node) {
}
void CreationVisitor::visit(StringLitNode *node) {
}
void CreationVisitor::visit(FloatLitNode *node) {
}
void CreationVisitor::visit(DotNode *node) {
}
void CreationVisitor::visit(EpsilonNode *node) {
}
void CreationVisitor::visit(ScopeSpecNode *node) {
}
void CreationVisitor::visit(TermNode *node) {
}
void CreationVisitor::visit(IndiceListNode *node) {
}
void CreationVisitor::visit(PrivateNode *node) {
}
void CreationVisitor::visit(PublicNode *node) {
}
void CreationVisitor::visit(DataMemberNode *node) {
}


SemanticCheckingVisitor::SemanticCheckingVisitor() { }

void SemanticCheckingVisitor::visit(ProgNode *node) {
}
void SemanticCheckingVisitor::visit(ClassDeclListNode *node) {
}
void SemanticCheckingVisitor::visit(ClassDeclNode *node) {
}
void SemanticCheckingVisitor::visit(MembListNode *node) {
}
void SemanticCheckingVisitor::visit(InherListNode *node) {
}
void SemanticCheckingVisitor::visit(FuncDefListNode *node) {
}
void SemanticCheckingVisitor::visit(FuncDefNode *node) {
}
void SemanticCheckingVisitor::visit(FuncBodyNode *node) {
}
void SemanticCheckingVisitor::visit(FuncHeadNode *node) {
}
void SemanticCheckingVisitor::visit(FuncDeclNode *node) {
}
void SemanticCheckingVisitor::visit(ParamListNode *node) {
}
void SemanticCheckingVisitor::visit(FParamNode *node) {
}
void SemanticCheckingVisitor::visit(FCallNode *node) {
}
void SemanticCheckingVisitor::visit(VisibilityNode *node) {
}
void SemanticCheckingVisitor::visit(MemberDeclNode *node) {
}
void SemanticCheckingVisitor::visit(IdNode *node) {
}
void SemanticCheckingVisitor::visit(VarDeclNode *node) {
}
void SemanticCheckingVisitor::visit(VarDeclListNode *node) {
}
void SemanticCheckingVisitor::visit(TypeNode *node) {
}
void SemanticCheckingVisitor::visit(IntegerNode *node) {
}
void SemanticCheckingVisitor::visit(FloatNode *node) {
}
void SemanticCheckingVisitor::visit(StringNode *node) {
}
void SemanticCheckingVisitor::visit(DimListNode *node) {
}
void SemanticCheckingVisitor::visit(NumIntNode *node) {
}
void SemanticCheckingVisitor::visit(StatementNode *node) {
}
void SemanticCheckingVisitor::visit(StatBlockNode *node) {
}
void SemanticCheckingVisitor::visit(VariableNode *node) {
}
void SemanticCheckingVisitor::visit(AssignStmtNode *node) {
}
void SemanticCheckingVisitor::visit(IfStmtNode *node) {
}
void SemanticCheckingVisitor::visit(ReadStmtNode *node) {
}
void SemanticCheckingVisitor::visit(WriteStmtNode *node) {
}
void SemanticCheckingVisitor::visit(ReturnStmtNode *node) {
}
void SemanticCheckingVisitor::visit(ContinueNode *node) {
}
void SemanticCheckingVisitor::visit(BreakNode *node) {
}
void SemanticCheckingVisitor::visit(ArithExprNode *node) {
}
void SemanticCheckingVisitor::visit(RelExprNode *node) {
}
void SemanticCheckingVisitor::visit(WhileStmtNode *node) {
}
void SemanticCheckingVisitor::visit(SignNode *node) {
}
void SemanticCheckingVisitor::visit(PlusNode *node) {
}
void SemanticCheckingVisitor::visit(MinusNode *node) {
}
void SemanticCheckingVisitor::visit(RelOpNode *node) {
}
void SemanticCheckingVisitor::visit(MultOpNode *node) {
}
void SemanticCheckingVisitor::visit(AddOpNode *node) {
}
void SemanticCheckingVisitor::visit(MultNode *node) {
}
void SemanticCheckingVisitor::visit(DivNode *node) {
}
void SemanticCheckingVisitor::visit(AndNode *node) {
}
void SemanticCheckingVisitor::visit(EqNode *node) {
}
void SemanticCheckingVisitor::visit(GtNode *node) {
}
void SemanticCheckingVisitor::visit(LtNode *node) {
}
void SemanticCheckingVisitor::visit(NeqNode *node) {
}
void SemanticCheckingVisitor::visit(GeqNode *node) {
}
void SemanticCheckingVisitor::visit(LeqNode *node) {
}
void SemanticCheckingVisitor::visit(IntLitNode *node) {
}
void SemanticCheckingVisitor::visit(StringLitNode *node) {
}
void SemanticCheckingVisitor::visit(FloatLitNode *node) {
}
void SemanticCheckingVisitor::visit(DotNode *node) {
}
void SemanticCheckingVisitor::visit(EpsilonNode *node) {
}
void SemanticCheckingVisitor::visit(ScopeSpecNode *node) {
}
void SemanticCheckingVisitor::visit(TermNode *node) {
}
void SemanticCheckingVisitor::visit(IndiceListNode *node) {
}
void SemanticCheckingVisitor::visit(PrivateNode *node) {
}
void SemanticCheckingVisitor::visit(PublicNode *node) {
}
void SemanticCheckingVisitor::visit(DataMemberNode *node) {
}
