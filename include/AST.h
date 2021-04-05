#pragma once
#include <string>
#include <unordered_set>
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
		int num_children = 0;
		string val;
		ASTNode(string val = "");
		ASTNode* make_siblings(ASTNode *y);
		static ASTNode* make_family(string &op, const vector<ASTNode*> &children);
		void adopt_children(ASTNode *y);
		static ASTNode* make_node(const string &type, const string &val = "");

		string to_dot_notation();
		virtual bool is_epsilon();
		virtual string to_str();

		virtual void accept(Visitor *v)=0;
		virtual string get_type() = 0;
		virtual string get_dims() const;
};


class ProgNode : public ASTNode {
	public:
		ProgNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class ClassDeclListNode : public ASTNode {
	public:
		ClassDeclListNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class ClassDeclNode : public ASTNode {
	public:
		ClassDeclNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class MembListNode : public ASTNode {
	public:
		MembListNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class InherListNode : public ASTNode {
	public:
		InherListNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class FuncDefListNode : public ASTNode {
	public:
		FuncDefListNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class FuncDefNode : public ASTNode {
	public:
		FuncDefNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class FuncBodyNode : public ASTNode {
	public:
		FuncBodyNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class FuncHeadNode : public ASTNode {
	public:
		FuncHeadNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class FuncDeclNode : public ASTNode {
	public:
		FuncDeclNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class ParamListNode : public ASTNode {
	public:
		ParamListNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class FParamNode : public ASTNode {
	public:
		FParamNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class FCallNode : public ASTNode {
	public:
		FCallNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class VisibilityNode : public ASTNode {
	public:
		VisibilityNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class MemberDeclNode : public ASTNode {
	public:
		MemberDeclNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class IdNode : public ASTNode {
	public:
		IdNode(string val);
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class VarDeclNode : public ASTNode {
	public:
		VarDeclNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class VarDeclListNode : public ASTNode {
	public:
		VarDeclListNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class TypeNode : public ASTNode {
	public:
		TypeNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class IntegerNode : public ASTNode {
	public:
		IntegerNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class FloatNode : public ASTNode {
	public:
		FloatNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class StringNode : public ASTNode {
	public:
		StringNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class DimListNode : public ASTNode {
	public:
		DimListNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;
		virtual string get_dims() const override;

};
class NumIntNode : public ASTNode {
	public:
		NumIntNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class StatementNode : public ASTNode {
	public:
		StatementNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class StatBlockNode : public ASTNode {
	public:
		StatBlockNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class VariableNode : public ASTNode {
	public:
		VariableNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class AssignStmtNode : public ASTNode {
	public:
		AssignStmtNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class IfStmtNode : public ASTNode {
	public:
		IfStmtNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class ReadStmtNode : public ASTNode {
	public:
		ReadStmtNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class WriteStmtNode : public ASTNode {
	public:
		WriteStmtNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class ReturnStmtNode : public ASTNode {
	public:
		ReturnStmtNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class ContinueNode : public ASTNode {
	public:
		ContinueNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class BreakNode : public ASTNode {
	public:
		BreakNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class ArithExprNode : public ASTNode {
	public:
		ArithExprNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class RelExprNode : public ASTNode {
	public:
		RelExprNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class WhileStmtNode : public ASTNode {
	public:
		WhileStmtNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class SignNode : public ASTNode {
	public:
		SignNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class PlusNode : public ASTNode {
	public:
		PlusNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class MinusNode : public ASTNode {
	public:
		MinusNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class RelOpNode : public ASTNode {
	public:
		RelOpNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class MultOpNode : public ASTNode {
	public:
		MultOpNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class AddOpNode : public ASTNode {
	public:
		AddOpNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class MultNode : public ASTNode {
	public:
		MultNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class DivNode : public ASTNode {
	public:
		DivNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class AndNode : public ASTNode {
	public:
		AndNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class EqNode : public ASTNode {
	public:
		EqNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class GtNode : public ASTNode {
	public:
		GtNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class LtNode : public ASTNode {
	public:
		LtNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class NeqNode : public ASTNode {
	public:
		NeqNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class GeqNode : public ASTNode {
	public:
		GeqNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class LeqNode : public ASTNode {
	public:
		LeqNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class IntLitNode : public ASTNode {
	public:
		IntLitNode(string val);
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class StringLitNode : public ASTNode {
	public:
		StringLitNode(string val);
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class FloatLitNode : public ASTNode {
	public:
		FloatLitNode(string val);
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class DotNode : public ASTNode {
	public:
		DotNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class EpsilonNode : public ASTNode {
	public:
		EpsilonNode();
		bool is_epsilon() override;
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class ScopeSpecNode : public ASTNode {
	public:
		ScopeSpecNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class TermNode : public ASTNode {
	public:
		TermNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class IndiceListNode : public ASTNode {
	public:
		IndiceListNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class PrivateNode : public ASTNode {
	public:
		PrivateNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class PublicNode : public ASTNode {
	public:
		PublicNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
class DataMemberNode : public ASTNode {
	public:
		DataMemberNode();
		virtual string get_type() override;
		virtual void accept(Visitor *v) override;

};
