#include "AST.h"
#include "stdarg.h"
#include <string>
#include <queue>
#include <iostream>

using std::string;
using std::cout;
using std::queue;
using std::endl;

ASTNode::ASTNode() {}

ASTNode::ASTNode(const string &value) : value(value) {}

ASTNode::ASTNode(string &value, string &par) : value(value), par(par) {}

ASTNode* ASTNode::make_siblings(ASTNode *y) {
	/* makes this a sibling of y where `this` becomes the leftmost sibling if y doesn't have a leftmode sibling.*/
	// `this` inherits the parents of y, not the other way around.
    ASTNode *xsibs = this;
	while (xsibs->right != nullptr)
		xsibs = xsibs->right;


	ASTNode *ysibs = y->leftmost_sibling == nullptr ? y : y->leftmost_sibling;
	xsibs->right = ysibs;
    ysibs->leftmost_sibling = xsibs->leftmost_sibling != nullptr ? xsibs->leftmost_sibling : xsibs;
	ysibs->parent = xsibs->parent;

	while (ysibs->right != nullptr) {
		ysibs = ysibs->right;
		ysibs->leftmost_sibling = xsibs->leftmost_sibling != nullptr ? xsibs->leftmost_sibling: xsibs;
		ysibs->parent = xsibs->parent;
	}

	return ysibs;
}

void ASTNode::adopt_children(ASTNode *y) {
	if (this->leftmost_child != nullptr) {
		this->leftmost_child->make_siblings(y);
	} else {
		ASTNode *ysibs = y->leftmost_sibling;
		this->leftmost_child = ysibs;
		while (ysibs != nullptr) {
			ysibs->parent = this;
			ysibs = ysibs->right;
		}
	}
}

void ASTNode::to_dot_notation() {
	queue<ASTNode*> container;
	string output = "";
	container.push(this);
	while (!container.empty()) {
		ASTNode *parent = container.front();
		//cout << parent->value << "->";
		container.pop();
		ASTNode *start = parent->leftmost_child;
		while (start != nullptr) {
			container.push(start);
			//cout << start->value;
			start = start->right;
		}
		//cout << endl;
	}
}

ASTNode *ASTNode::make_family(string &op, const vector<ASTNode*> &children) {
	ASTNode *node = ASTNode::make_node(op);
	ASTNode *base = children.front();
	for(int i = 1; i < children.size(); ++i)
		base->make_siblings(children[i]);
	node->adopt_children(base);
	return node;
}

ASTNode *ASTNode::make_node() {
	return new ASTNode();
}

ASTNode *ASTNode::make_node(const string &type) {
	return new ASTNode(type);
}

ASTNode *ASTNode::make_node(string &type, string &par) {
	return new ASTNode(type, par);
}
