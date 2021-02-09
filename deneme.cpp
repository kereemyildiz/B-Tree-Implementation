// C++ program for B-Tree insertion 
#include <iostream>
#include <typeinfo>
#include <vector>
#include <fstream>
#include <string>
using namespace std; 

// A BTree node

char op;

struct Info {
    int x;
    int y;
    char z;

    bool operator<(Info temp);
    bool operator>(Info temp);
    bool operator==(Info temp);


};
bool Info::operator<(Info temp) {
    if (op == 'x') {
        return x < temp.x;
    }
    else if (op == 'y') {
        return y < temp.y;
    }
    else if (op == 'z') {
        return z < temp.z;
    }
}
bool Info::operator>(Info temp) {
    if (op == 'x') {
        return x > temp.x;
    }
    else if (op == 'y') {
        return y > temp.y;
    }
    else if (op == 'z') {
        return z > temp.z;
    }
}
bool Info::operator==(Info temp) {
    if (op == 'x') {
        return x == temp.x;
    }
    else if (op == 'y') {
        return y == temp.y;
    }
    else if (op == 'z') {
        return z == temp.z;
    }
}
class Node 
{ 

public: 
	Info *keys; // An array of keys 
	int t;	 // Minimum degree (defines the range for number of keys) 
	Node **C; // An array of child pointers 
	int n;	 // Current number of keys 
	bool leaf; // Is true when node is leaf. Otherwise false 
	Node(int _t, bool _leaf); // Constructor 

	// A utility function to insert a new key in the subtree rooted with 
	// this node. The assumption is, the node must be non-full when this 
	// function is called 
	void insertNonFull(Info k); 

	// A utility function to split the child y of this node. i is index of y in 
	// child array C[]. The Child y must be full when this function is called 
	void splitChild(int i, Node *y); 

	// A function to traverse all nodes in a subtree rooted with this node 
	void traverse(); 

	// A function to search a key in the subtree rooted with this node. 
	Node *search(Info k); // returns NULL if k is not present. 

// Make BTree friend of this so that we can access private members of this 
// class in BTree functions 
}; 

// A BTree 
class BTree 
{ 

public: 
	Node *root; // Pointer to root node 
	int t; // Minimum degree 
	// Constructor (Initializes tree as empty) 
	BTree(int _t) 
	{ root = NULL; t = _t; } 

	// function to traverse the tree 
	void traverse() 
	{ if (root != NULL) root->traverse(); } 

	// function to search a key in this tree 
	Node* search(Info k) 
	{ return (root == NULL)? NULL : root->search(k); } 

	// The main function that inserts a new key in this B-Tree 
	void insert(Info k); 
}; 

// Constructor for BTreeNode class 
Node::Node(int t1, bool leaf1) :n(0)
{ 
	// Copy the given minimum degree and leaf property 
	t = t1; 
	leaf = leaf1; 

	// Allocate memory for maximum number of possible keys 
	// and child pointers 
	keys = new Info[2*t-1]; 
	C = new Node *[2*t]; 

	// Initialize the number of keys as 0 

} 

// Function to traverse all nodes in a subtree rooted with this node 
void Node::traverse() 
{ 
	// There are n keys and n+1 children, traverse through n keys 
	// and first n children 
	for (int i = 0; i < n; i++) 
	{ 
		cout << "(" << keys[i].x << ","<< keys[i].y << "," << keys[i].z <<")"; 

		// If this is not leaf, then before printing key[i], 
		// traverse the subtree rooted with child C[i].
	} 

	// Print the subtree rooted with last child 
	if (leaf == false) {
		for (int i = 0; i < n+1; i++){
            cout << endl;
            C[i]->traverse();
        }
    } 
} 

// Function to search key k in subtree rooted with this node 
Node *Node::search(Info k) 
{ 
	// Find the first key greater than or equal to k 
	int i = 0; 
	while (i < n && k > keys[i]) 
		i++; 

	// If the found key is equal to k, return this node 
	if (keys[i] == k) 
		return this; 

	// If key is not found here and this is a leaf node 
	if (leaf == true) 
		return NULL; 

	// Go to the appropriate child 
	return C[i]->search(k); 
} 

// The main function that inserts a new key in this B-Tree 

/*

B-TREE-INSERT(T,k)
1  r ← root[T]  
2  if n[r] = 2t - 1⊳if root is full, add a new layer 
3       thens←ALLOCATE-NODE()  
4               root[T] ← s  
5               leaf[s] ← FALSE  
6               n[s] ← 0  
7               c1[s]← r  
8               B-TREE-SPLIT-CHILD(s,1,r)  
9               B-TREE-INSERT-NONFULL(s,k) 
10 elseB-Tree-INSERT-NONFULL(r,k)
*/
void BTree::insert(Info k) 
{ 
	// If tree is empty 
	if (root == NULL) 
	{ 
		// Allocate memory for root 
		root = new Node(t, true); 
		root->keys[0] = k; // Insert key 
		root->n = 1; // Update number of keys in root 
	} 
	else // If tree is not empty 
	{ 
		// If root is full, then tree grows in height 
		if (root->n == 2*t-1) 
		{ 
			// Allocate memory for new root 
			Node *s = new Node(t, false); 

			// Make old root as child of new root 
			s->C[0] = root; 

			// Split the old root and move 1 key to the new root 
			s->splitChild(0, root); 

			// New root has two children now. Decide which of the 
			// two children is going to have new key 
			int i = 0; 
			if (s->keys[0] < k) 
				i++; 
			s->C[i]->insertNonFull(k); 

			// Change root 
			root = s; 
		} 
		else // If root is not full, call insertNonFull for root 
			root->insertNonFull(k); 
	} 
} 

// A utility function to insert a new key in this node 
// The assumption is, the node must be non-full when this 
// function is called 
void Node::insertNonFull(Info k) 
{ 
	// Initialize index as index of rightmost element 
	int i = n-1; 

	// If this is a leaf node 
	if (leaf == true) 
	{ 
		// The following loop does two things 
		// a) Finds the location of new key to be inserted 
		// b) Moves all greater keys to one place ahead 
		while (i > -1 && keys[i] > k) 
		{ 
			keys[i+1] = keys[i]; 
			i--; 
		} 

		// Insert the new key at found location 
		keys[i+1] = k; 
		n = n+1; 
	} 
	else // If this node is not leaf 
	{ 
		// Find the child which is going to have the new key 
		while (i > -1 && keys[i] > k) 
			i--; 

		// See if the found child is full 
		if (C[i+1]->n == 2*t-1) 
		{ 
			// If the child is full, then split it 
			splitChild(i+1, C[i+1]); 

			// After split, the middle key of C[i] goes up and 
			// C[i] is splitted into two. See which of the two 
			// is going to have the new key 
			if (keys[i+1] < k) 
				i++; 
		} 
		C[i+1]->insertNonFull(k); 
	} 
} 

// A utility function to split the child y of this node 
// Note that y must be full when this function is called 
void Node::splitChild(int i, Node *y) 
{ 
	// Create a new node which is going to store (t-1) keys 
	// of y 
	Node *z = new Node(y->t, y->leaf); 
	z->n = t - 1; 

	// Copy the last (t-1) keys of y to z 
	for (int j = 0; j < t-1; j++) 
		z->keys[j] = y->keys[j+t]; 

	// Copy the last t children of y to z 
	if (y->leaf == false) 
	{ 
		for (int j = 0; j < t; j++) 
			z->C[j] = y->C[j+t]; 
	} 

	// Reduce the number of keys in y 
	y->n = t - 1; 

	// Since this node is going to have a new child, 
	// create space of new child 
	for (int j = n; j > i; j--) 
		C[j+1] = C[j]; 

	// Link the new child to this node 
	C[i+1] = z; 

	// A key of y will move to this node. Find the location of 
	// new key and move all greater keys one space ahead 
	for (int j = n-1; j > i-1; j--) 
		keys[j+1] = keys[j]; 

	// Copy the middle key of y to this node 
	keys[i] = y->keys[t-1]; 

	// Increment count of keys in this node 
	n = n + 1; 
} 

// Driver program to test above functions 
int main() 
{ 
    ifstream ip;
    ip.open("input.txt");

    BTree t(3);

    int count;
    string _count;
    string _degree;
    string _op;
    int degree;
    getline(ip, _count);
    getline(ip, _degree);
    ip.get(op);
    count = stod(_count);
    cout << _count << endl;
    cout << _degree << endl;
    cout << op;
    cout << endl;
    int x;
    int y;
    char dummy;
    char z;

    for (int i = 0; i < count; i++)
    {
        ip >> x;
        ip >> y;
        ip.get(dummy); // space
        ip.get(z);
        Info temp;
        temp.x = x;
        temp.y = y;
        temp.z = z;
        t.insert(temp);

    } 
	t.traverse(); 

	// int k = 6; 
	// (t.search(k) != NULL)? cout << "\nPresent" : cout << "\nNot Present"; 

	// k = 15; 
	// (t.search(k) != NULL)? cout << "\nPresent" : cout << "\nNot Present"; 

	return 0; 
} 
