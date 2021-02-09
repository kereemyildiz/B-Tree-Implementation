#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <typeinfo>
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
    else
        throw "Invalid key identifier";
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
    else
        throw "Invalid key identifier ";
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
    else
        throw "Invalid key identifier";
}
class Node
{

public:
    Info *keys; // An array of keys
    int t;     // Minimum degree (defines the range for number of keys)
    Node **C; // An array of child pointers
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
    Node(int _t, bool _leaf); // Constructor


    void insertNonFull(Info k);


    void splitChild(int i, Node *y);

 
    void traverse();

    
    Node *search(Info k); 

s
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

        if (root->n == 2*t-1)
        {
            Node *newnode = new Node(t, false);


            newnode->C[0] = root;
            newnode->leaf = false;
            newnode->n = 0;


            newnode->splitChild(0, root);

            int i = 0;
            if (newnode->keys[0] < k)
                i++;
            newnode->C[i]->insertNonFull(k);


            root = newnode;
        }
        else // If root is not full, call insertNonFull for root
            root->insertNonFull(k);
    }
}


void Node::insertNonFull(Info k)
{
    int i = n-1;

    if (leaf == true)
    {

        while (i > -1 && k < keys[i] )
        {
            keys[i+1] = keys[i];
            i--;
        }

        keys[i+1] = k;
        n = n+1;
    }
    else // If this node is not leaf
    {
        while (i > -1 && k < keys[i])
            i--;

        i++;
        if (C[i]->n == 2*t-1)
        {
            splitChild(i, C[i]);

            if (keys[i] < k)
                i++;
        }
        C[i]->insertNonFull(k);
    }
}
void Node::splitChild(int i, Node *y)
{

    Node *z = new Node(y->t, y->leaf);
    z->n = t - 1;

    int j = 0;
    while(j < t-1){
        z->keys[j] = y->keys[j+t];
        j++;
    }


    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }


    y->n = t - 1;


    for (int j = n; j > i; j--)
        C[j+1] = C[j];

    
    C[i+1] = z;

    for (int j = n-1; j > i-1; j--)
        keys[j+1] = keys[j];

    keys[i] = y->keys[t-1];

    n = n + 1;
}

int main()
{


    int count;
    int degree;
    cin >> count;
    cin >> degree;
    cin >> op;

    BTree t(degree);
    for (int i = 0; i < count; i++){
        Info temp;
        cin >> temp.x >> temp.y >> temp.z;
        t.insert(temp);
    }

        // ifstream ip;
        // ip.open("input.txt");

        // BTree t(3);

        // int count;
        // string _count;
        // string _degree;
        // string _op;
        // int degree;
        // getline(ip, _count);
        // getline(ip, _degree);
        // ip.get(op);
        // count = stod(_count);
        // cout << _count << endl;
        // cout << _degree << endl;
        // cout << op;
        // cout << endl;
        // int x;
        // int y;
        // char dummy;
        // char z;

        // for (int i = 0; i < count; i++)
        // {
        //     ip >> x;
        //     ip >> y;
        //     ip.get(dummy); // space
        //     ip.get(z);
        //     Info temp;
        //     temp.x = x;
        //     temp.y = y;
        //     temp.z = z;
        //     t.insert(temp);

        // }
        t.traverse();


    return 0;
}
