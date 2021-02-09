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

class B_Tree_Node {

	Info *keys; // struct array that stores keys
	int t; // min degree
	B_Tree_Node **chs;// array of children pointers
	int num_key; // number of keys
	bool leaf;
	B_Tree_Node(int t, bool leaf);

};
B_Tree_Node::B_Tree_Node(int t,bool leaf): num_key(0) // initialize num_keys 0
{
	t = t;
	leaf = leaf;

	keys = new Info[2 * t - 1]; // keys
	chs = new B_Tree_Node *[2 * t]; // childrens

	void insert(Info key);
	void split(int i, B_Tree_Node *y);
	void traverse();
}




class BTree
{
	
}