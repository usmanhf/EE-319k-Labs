#include <stdlib.h>
#include "BST.hpp"

BST::BST() {
	this->root = nullptr;
}

void BST::insert(int el) {
	Node *n = new Node(el);

	if (this->root == nullptr) {
		this->root = n;
		return;
	}

	Node *parent = nullptr;
	Node *current = this->root;

	while (current != nullptr) {
		parent = current;
		if (current->val > el) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	// Include new node into the tree.
	if (parent->val > el) {
		parent->left = n;
	}
	else {
		parent->right = n;
	}
	n->parent = parent;
}

bool BST::find(int el) {
	Node *current = root;
	while (current != nullptr) {
		if (current->val == el) {
			return true;
		}

		if (current->val > el) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}
	return false;
}

BST::Node* BST::findMin() {
	if (this->root == nullptr) {
		return nullptr;
	}

	Node *current = root;
	while (current->left != nullptr) {
		current = current->left;
	}
	return current;
}

BST::Node* BST::successor(Node *n) {
	if (n->right != nullptr) {
		Node *current = n->right;
		while (current->left != nullptr) {
			current = current->left;
		}
		return current;
	}
	// TODO
}