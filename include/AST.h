#pragma once
#include <string>
#include <unordered_set>
#include <stdarg.h>
#include <vector>
#include "SymbolTable.h"
#include "SymbolTableRecord.h"
#include "Visitor.h"

/**
 * Deals with collecting the nodes while program is being parsed.
 * Difference b/w it and a Parse Tree is that it doens't include everything.
 * Mainly used as a way to decouple the syntax analysis code from the semantic checking phase.
 *
 * The tree SHOULD NOT contain the types of each node.
 */

using std::string;
using std::vector;
using std::unordered_set;

class Visitor;

class ASTNode {
	public:
		ASTNode *parent = nullptr, *leftmost_child = nullptr, *leftmost_sibling = nullptr, *right = nullptr;
		SymbolTable *table = nullptr;
		SymbolTableRecord *record = nullptr;
		string val;
		ASTNode(string val = "");
		ASTNode* make_siblings(ASTNode *y);
		static ASTNode* make_family(string &op, const vector<ASTNode*> &children);
		void adopt_children(ASTNode *y);
		static ASTNode* make_node(const string &type, const string &val = "");

		string to_dot_notation();
		virtual bool is_epsilon();

		void accept(Visitor &v);
		virtual string get_type() = 0;
};


class ProgNode : public ASTNode {
	public:
		ProgNode();
		virtual string get_type() override;
};
class ClassDeclListNode : public ASTNode {
	public:
		ClassDeclListNode();
		virtual string get_type() override;
};
class ClassDeclNode : public ASTNode {
	public:
		ClassDeclNode();
		virtual string get_type() override;
};
class MembListNode : public ASTNode {
	public:
		MembListNode();
		virtual string get_type() override;
};
class InherListNode : public ASTNode {
	public:
		InherListNode();
		virtual string get_type() override;
};
class FuncDefListNode : public ASTNode {
	public:
		FuncDefListNode();
		virtual string get_type() override;
};
class FuncDefNode : public ASTNode {
	public:
		FuncDefNode();
		virtual string get_type() override;
};
class FuncBodyNode : public ASTNode {
	public:
		FuncBodyNode();
		virtual string get_type() override;
};
class FuncHeadNode : public ASTNode {
	public:
		FuncHeadNode();
		virtual string get_type() override;
};
class FuncDeclNode : public ASTNode {
	public:
		FuncDeclNode();
		virtual string get_type() override;
};
class FParamListNode : public ASTNode {
	public:
		FParamListNode();
		virtual string get_type() override;
};
class FParamNode : public ASTNode {
	public:
		FParamNode();
		virtual string get_type() override;
};
class FCallNode : public ASTNode {
	public:
		FCallNode();
		virtual string get_type() override;
};
class VisibilityNode : public ASTNode {
	public:
		VisibilityNode();
		virtual string get_type() override;
};
class MemberDeclNode : public ASTNode {
	public:
		MemberDeclNode();
		virtual string get_type() override;
};
class IdNode : public ASTNode {
	public:
		IdNode(string val);
		virtual string get_type() override;
};
class VarDeclNode : public ASTNode {
	public:
		VarDeclNode();
		virtual string get_type() override;
};
class VarDeclListNode : public ASTNode {
	public:
		VarDeclListNode();
		virtual string get_type() override;
};
class TypeNode : public ASTNode {
	public:
		TypeNode();
		virtual string get_type() override;
};
class IntegerNode : public ASTNode {
	public:
		IntegerNode();
		virtual string get_type() override;
};
class FloatNode : public ASTNode {
	public:
		FloatNode();
		virtual string get_type() override;
};
class StringNode : public ASTNode {
	public:
		StringNode();
		virtual string get_type() override;
};
class DimListNode : public ASTNode {
	public:
		DimListNode();
		virtual string get_type() override;
};
class NumIntNode : public ASTNode {
	public:
		NumIntNode();
		virtual string get_type() override;
};
class StatementNode : public ASTNode {
	public:
		StatementNode();
		virtual string get_type() override;
};
class StatBlockNode : public ASTNode {
	public:
		StatBlockNode();
		virtual string get_type() override;
};
class VariableNode : public ASTNode {
	public:
		VariableNode();
		virtual string get_type() override;
};
class AssignStmtNode : public ASTNode {
	public:
		AssignStmtNode();
		virtual string get_type() override;
};
class IfStmtNode : public ASTNode {
	public:
		IfStmtNode();
		virtual string get_type() override;
};
class ReadStmtNode : public ASTNode {
	public:
		ReadStmtNode();
		virtual string get_type() override;
};
class WriteStmtNode : public ASTNode {
	public:
		WriteStmtNode();
		virtual string get_type() override;
};
class ReturnStmtNode : public ASTNode {
	public:
		ReturnStmtNode();
		virtual string get_type() override;
};
class ContinueNode : public ASTNode {
	public:
		ContinueNode();
		virtual string get_type() override;
};
class BreakNode : public ASTNode {
	public:
		BreakNode();
		virtual string get_type() override;
};
class ArithExprNode : public ASTNode {
	public:
		ArithExprNode();
		virtual string get_type() override;
};
class RelExprNode : public ASTNode {
	public:
		RelExprNode();
		virtual string get_type() override;
};
class WhileStmtNode : public ASTNode {
	public:
		WhileStmtNode();
		virtual string get_type() override;
};
class SignNode : public ASTNode {
	public:
		SignNode();
		virtual string get_type() override;
};
class PlusNode : public ASTNode {
	public:
		PlusNode();
		virtual string get_type() override;
};
class MinusNode : public ASTNode {
	public:
		MinusNode();
		virtual string get_type() override;
};
class RelOpNode : public ASTNode {
	public:
		RelOpNode();
		virtual string get_type() override;
};
class MultOpNode : public ASTNode {
	public:
		MultOpNode();
		virtual string get_type() override;
};
class MultNode : public ASTNode {
	public:
		MultNode();
		virtual string get_type() override;
};
class DivNode : public ASTNode {
	public:
		DivNode();
		virtual string get_type() override;
};
class AndNode : public ASTNode {
	public:
		AndNode();
		virtual string get_type() override;
};
class EqNode : public ASTNode {
	public:
		EqNode();
		virtual string get_type() override;
};
class GtNode : public ASTNode {
	public:
		GtNode();
		virtual string get_type() override;
};
class LtNode : public ASTNode {
	public:
		LtNode();
		virtual string get_type() override;
};
class NeqNode : public ASTNode {
	public:
		NeqNode();
		virtual string get_type() override;
};
class GeqNode : public ASTNode {
	public:
		GeqNode();
		virtual string get_type() override;
};
class LeqNode : public ASTNode {
	public:
		LeqNode();
		virtual string get_type() override;
};
class IntLitNode : public ASTNode {
	public:
		IntLitNode(string val);
		virtual string get_type() override;
};
class StringLitNode : public ASTNode {
	public:
		StringLitNode(string val);
		virtual string get_type() override;
};
class FloatLitNode : public ASTNode {
	public:
		FloatLitNode(string val);
		virtual string get_type() override;
};
class DotNode : public ASTNode {
	public:
		DotNode();
		virtual string get_type() override;
};
class EpsilonNode : public ASTNode {
	public:
		EpsilonNode();
		virtual string get_type() override;
		bool is_epsilon() override;
};
class ScopeSpecNode : public ASTNode {
	public:
		virtual string get_type() override;
		ScopeSpecNode();
};
class TermNode : public ASTNode {
	public:
		virtual string get_type() override;
		TermNode();
};
class IndiceListNode : public ASTNode {
	public:
		virtual string get_type() override;
		IndiceListNode();
};
class PrivateNode : public ASTNode {
	public:
		virtual string get_type() override;
		PrivateNode();
};
class PublicNode : public ASTNode {
	public:
		virtual string get_type() override;
		PublicNode();
};
