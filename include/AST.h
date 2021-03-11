#pragma once
#include <string>
/**
 * Deals with collecting the nodes while program is being parsed.
 * Difference b/w it and a Parse Tree is that it doens't include everything.
 * Mainly used as a way to decouple the syntax analysis code from the semantic checking phase.
 *
 * The tree SHOULD NOT contain the types of each node.
 */

using std::string;

struct ASTNode {
	const ASTNode *parent, *leftmost_child, *leftmost_sibling, *next;
	ASTNode();
	//ASTNode(ASTNode *parent, ASTNode *leftmost_child, ASTNode *leftmost_sibling, ASTNode *next);
	ASTNode* make_siblings(ASTNode *y);
	void adopt_children(ASTNode *y);
	ASTNode* make_family(ASTNode *op, ASTNode *kid1, ASTNode *kid2);
};

static ASTNode* make_node();
static ASTNode* make_node(string type);
