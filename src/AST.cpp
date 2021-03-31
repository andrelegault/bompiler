#include "AST.h"
#include <string>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <utility>

using std::string;
using std::cout;
using std::queue;
using std::endl;
using std::unordered_map;
using std::to_string;

ASTNode::ASTNode(string type, string val) : type(std::move(type)), val(std::move(val)) { }

ASTNode* ASTNode::make_siblings(ASTNode *y) {
	/* makes this a sibling of y where `this` becomes the leftmost sibling if y doesn't have a leftmode sibling.*/
	// `this` inherits the parents of y, not the other way around.
	//cout << this->type << ".make_siblings(" << y->type << ")" << endl;
    ASTNode *xsibs = this;
	while (xsibs->right != nullptr)
		xsibs = xsibs->right;


	ASTNode *ysibs = y->leftmost_sibling == nullptr ? y : y->leftmost_sibling;
	xsibs->right = ysibs;
    ysibs->leftmost_sibling = xsibs->leftmost_sibling == nullptr ? xsibs: xsibs->leftmost_sibling;
	ysibs->parent = xsibs->parent;

	while (ysibs->right != nullptr) {
		ysibs = ysibs->right;
		ysibs->leftmost_sibling = xsibs->leftmost_sibling == nullptr ? xsibs: xsibs->leftmost_sibling;
		ysibs->parent = xsibs->parent;
	}

	return ysibs;
}

void ASTNode::adopt_children(ASTNode *y) {
	if (this->leftmost_child != nullptr) {
		this->leftmost_child->make_siblings(y);
	} else {
		ASTNode *ysibs = y->leftmost_sibling == nullptr ? y : y->leftmost_sibling;
		this->leftmost_child = ysibs;
		while (ysibs != nullptr) {
			ysibs->parent = this;
			ysibs = ysibs->right;
		}
	}
}

string ASTNode::to_dot_notation() {
	string str = "digraph G {\n";
	queue<ASTNode*> container;
	unordered_map<string, int> children_count, parent_count;
	container.push(this);
	while (!container.empty()) {
		ASTNode *parent = container.front();
		container.pop();
		ASTNode *start = parent->leftmost_child;
		string parent_val = parent->type;
        parent_val = parent->type + to_string(parent_count[parent->type]++);
		while (start != nullptr) {
			container.push(start);
			cout << start->type << ":" << start->val << endl;
            string start_val = start->type + to_string(children_count[start->type]++);
			str +=  "\t" + parent_val + "->" + start_val + "\n";
			start = start->right;
		}
	}
	str += "}\n";
	return str;
}

ASTNode *ASTNode::make_family(string &op, const vector<ASTNode*> &children) {
//	cout << "make_family(" << op << ", {";
	ASTNode *node = ASTNode::make_node(op);
	ASTNode *base = children.front();
//	cout << base->type << ",";
	for(int i = 1; i < children.size(); ++i) {
//		cout << children[i]->type << ",";
		base->make_siblings(children[i]);
	}
//	cout << "})"<<endl;
	node->adopt_children(children.back());
	return node;
}

ASTNode *ASTNode::make_node() {
	return new ASTNode();
}

ASTNode *ASTNode::make_node(const string &type, const string &val) {
	return new ASTNode(type, val);
}
