
//used for keeping scores

class BST {
	class Node {
	public:
		int val;
		Node *left;
		Node *right;
		Node *parent;
	};
	Node *root;
	Node* findMin();
	Node* successor(Node *n);
public:
	BST();
	~BST();
	void insert(int el);
	bool find(int el);
};