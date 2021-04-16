#pragma once
#include "AST.h"
#include "AST.fwd.h"

class Visitor {
	public:
		Visitor();

		virtual void visit(ProgNode *node);
		virtual void visit(ClassDeclListNode *node);
		virtual void visit(ClassDeclNode *node);
		virtual void visit(MembListNode *node);
		virtual void visit(InherListNode *node);
		virtual void visit(FuncDefListNode *node);
		virtual void visit(FuncDefNode *node);
		virtual void visit(FuncBodyNode *node);
		virtual void visit(FuncHeadNode *node);
		virtual void visit(FuncDeclNode *node);
		virtual void visit(FParamListNode *node);
		virtual void visit(AParamListNode *node);
		virtual void visit(FParamNode *node);
		virtual void visit(FCallNode *node);
		virtual void visit(VisibilityNode *node);
		virtual void visit(MemberDeclNode *node);
		virtual void visit(IdNode *node);
		virtual void visit(VarDeclNode *node);
		virtual void visit(VarDeclListNode *node);
		virtual void visit(TypeNode *node);
		virtual void visit(IntegerNode *node);
		virtual void visit(FloatNode *node);
		virtual void visit(StringNode *node);
		virtual void visit(DimListNode *node);
		virtual void visit(NumIntNode *node);
		virtual void visit(StatementNode *node);
		virtual void visit(StatBlockNode *node);
		virtual void visit(VariableNode *node);
		virtual void visit(AssignStmtNode *node);
		virtual void visit(IfStmtNode *node);
		virtual void visit(ReadStmtNode *node);
		virtual void visit(WriteStmtNode *node);
		virtual void visit(ReturnStmtNode *node);
		virtual void visit(ContinueNode *node);
		virtual void visit(BreakNode *node);
		virtual void visit(ArithExprNode *node);
		virtual void visit(RelExprNode *node);
		virtual void visit(WhileStmtNode *node);
		virtual void visit(SignNode *node);
		virtual void visit(PlusNode *node);
		virtual void visit(MinusNode *node);
		virtual void visit(RelOpNode *node);
		virtual void visit(MultOpNode *node);
		virtual void visit(AddOpNode *node);
		virtual void visit(MultNode *node);
		virtual void visit(DivNode *node);
		virtual void visit(AndNode *node);
		virtual void visit(EqNode *node);
		virtual void visit(GtNode *node);
		virtual void visit(LtNode *node);
		virtual void visit(NeqNode *node);
		virtual void visit(GeqNode *node);
		virtual void visit(LeqNode *node);
		virtual void visit(IntLitNode *node);
		virtual void visit(StringLitNode *node);
		virtual void visit(FloatLitNode *node);
		virtual void visit(DotNode *node);
		virtual void visit(EpsilonNode *node);
		virtual void visit(ScopeSpecNode *node);
		virtual void visit(TermNode *node);
		virtual void visit(IndiceListNode *node);
		virtual void visit(PrivateNode *node);
		virtual void visit(PublicNode *node);
		virtual void visit(DataMemberNode *node);
		virtual void visit(VoidNode *node);
};

class CreatingVisitor : public Visitor {
	public:
		int temp_count = 0;
		CreatingVisitor();
		void visit(ProgNode *node);
		void visit(FuncDefNode *node);
		void visit(FParamNode *node);
		void visit(ClassDeclNode *node);
		void visit(InherListNode *node);
		void visit(FuncDeclNode *node);
		void visit(VarDeclNode *node);
		void visit(IntLitNode *node);
		void visit(FloatLitNode *node);
		void visit(StringLitNode *node);
		void visit(StatementNode *node);
};

class CheckingVisitor : public Visitor {
	public:
		CheckingVisitor();
		void visit(ClassDeclNode *node);
		void visit(FuncDefNode *node);
		void visit(FuncDeclNode *node);
		void visit(MemberDeclNode *node);
		void visit(VarDeclNode *node);
		void visit(AssignStmtNode *node);
		void visit(FCallNode *node);
		void visit(ReturnStmtNode *node);
		void visit(DimListNode *node);
		void visit(DotNode *node);
		void visit(VariableNode *node);
};

class SizeSetterVisitor : public Visitor {
	public:
		SizeSetterVisitor();
		void visit(IntegerNode *node);
		void visit(FloatNode *node);
		void visit(FuncDefNode *node);
		void visit(VarDeclNode *node);
		void visit(StatBlockNode *node);
		void visit(VarDeclListNode *node);
};

class CodeGenerationVisitor : public Visitor {
	public:
		CodeGenerationVisitor();
		void visit(ClassDeclNode *node);
		void visit(VarDeclNode *node);
		void visit(FuncDefNode *node);
};
