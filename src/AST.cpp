/**
 * Deals with collecting the nodes while program is being parsed.
 * Difference b/w it and a Parse Tree is that it doens't include everything.
 * Mainly used as a way to decouple the syntax analysis code from the semantic checking phase.
 */

#include "AST.h"

#include <string>

using std::string;

ASTNode::ASTNode() {}

ASTNode::ASTNode(string &value) : value(value) {}

ASTNode *ASTNode::make_siblings(ASTNode *y) {
    ASTNode *xsibs = this;
    while (xsibs->right != nullptr) {
        xsibs = xsibs->right;
    }

    ASTNode *ysibs = y->leftmost_sibling;
    xsibs->right = ysibs;

    ysibs->leftmost_sibling = xsibs->leftmost_sibling;
    ysibs->parent = xsibs->parent;

    while (ysibs->right != nullptr) {
        ysibs = ysibs->right;
        ysibs->leftmost_sibling = xsibs->leftmost_sibling;
        ysibs->parent = xsibs->parent;
    }

    return ysibs;
}

void ASTNode::adopt_children(ASTNode *y) {
    if (this->leftmost_sibling != nullptr) {
        this->leftmost_sibling->make_siblings(y);
    } else {
        ASTNode *ysibs = y->leftmost_sibling;
        this->leftmost_sibling = ysibs;
        while (ysibs != nullptr) {
            ysibs->parent = this;
            ysibs = ysibs->right;
        }
    }
}

ASTNode *ASTNode::make_family(string op, ASTNode *kid1, ASTNode *kid2) {
    ASTNode *node = ASTNode::make_node(op);
    node->adopt_children(kid1->make_siblings(kid2));
    return node;
}

ASTNode *ASTNode::make_node() {
    return new ASTNode();
}

ASTNode *ASTNode::make_node(string type) {
    return new ASTNode(type);
}
