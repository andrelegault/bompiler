/**
 * Deals with collecting the nodes while program is being parsed.
 * Difference b/w it and a Parse Tree is that it doens't include everything.
 * Mainly used as a way to decouple the syntax analysis code from the semantic checking phase.
 */

#include "AST.h"

struct ASTNode {
	ASTNode *parent, *leftmost_child, *leftmost_sibling, *next;
	ASTNode(ASTNode *parent, ASTNode *leftmost_child, ASTNode *leftmost_sibling, ASTNode *next);
};
