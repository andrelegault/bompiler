#pragma once
#include <string>
#include <unordered_set>
#include <stdarg.h>
#include <vector>
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

struct ASTNode {
	ASTNode *parent = nullptr, *leftmost_child = nullptr, *leftmost_sibling = nullptr, *right = nullptr;
	string type, val;
	ASTNode(string type = "epsilon", string val = "");
	ASTNode* make_siblings(ASTNode *y);
	static ASTNode* make_family(string &op, const vector<ASTNode*> &children);
	void adopt_children(ASTNode *y);
	//ASTNode* make_family(string op, ASTNode *kid1, ASTNode *kid2);
    static ASTNode* make_node();
	static ASTNode* make_node(const string &type, const string &val = "");
	
	string to_dot_notation();
};

/* TODO: add node subclasses */
