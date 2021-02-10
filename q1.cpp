#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <string>
using namespace std;


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
class B_Tree_Node
{

public:
    Info *keys; // An array of keys
    int t;     // Minimum degree (defines the range for number of keys)
    B_Tree_Node **chs; // An array of child pointers
    int key_num;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
    B_Tree_Node(int t, bool leaf); // Constructor


    void insert2(Info k); // when node is not full we call this func

    void split(int i, B_Tree_Node *y);

    void traverse();

};


class BTree
{

public:
    B_Tree_Node *root; // Pointer to root node
    int t; // Minimum degree

    BTree(int t)
    { root = NULL; this->t = t; }


    void traverse()
    { if (root != NULL) root->traverse(); }



    void insert(Info k);
};


B_Tree_Node::B_Tree_Node(int t1, bool leaf1) : key_num(0)
{

    t = t1;
    leaf = leaf1;

    keys = new Info[2*t-1];
    chs = new B_Tree_Node *[2*t];

    

}

void B_Tree_Node::traverse()
{

    for (int i = 0; i < key_num; i++)
    {
        cout << "(" << keys[i].x << ","<< keys[i].y << "," << keys[i].z <<")";
    }

    // Print the subtree rooted with last child
    if (leaf == false) {
        for (int i = 0; i < key_num+1; i++){
            cout << endl;
            chs[i]->traverse();
        }
    }
}

/*

B-TREE-INSERT(T,k)
1  r <- root[T]
2  if n[r] = 2t - 1 if root is full, add a new layer
3       thens<-ALLOCATE-NODE()
4               root[T] <- s
5               leaf[s] <- FALSE
6               n[s] <- 0
7               c1[s]<- r
8               B-TREE-SPLIT-CHILD(s,1,r)
9               B-TREE-INSERT-NONFULL(s,k)
10 elseB-Tree-INSERT-NONFULL(r,k)
*/
void BTree::insert(Info k)
{

    if (root == NULL)
    {

        root = new B_Tree_Node(t, true);
        root->keys[0] = k;
        root->key_num = 1; 
    }
    else // If tree is not empty
    {

        if (root->key_num == 2*t-1)
        {
            B_Tree_Node *newnode = new B_Tree_Node(t, false);


            newnode->chs[0] = root;
            newnode->leaf = false;
            newnode->key_num = 0;


            newnode->split(0, root);

            int i = 0;
            if (newnode->keys[0] < k)
                i++;
            newnode->chs[i]->insert2(k);


            root = newnode;
        }
        else // If root is not full, call insertNonFull for root
            root->insert2(k);
    }
}

/*
B-TREE-INSERT-NONFULL(x,k)  
1   i <- n[x]  
2   if leaf[x]  
3       then while i>= 1 and k < keyi[x]  
4                      do keyi+1[x]<- keyi[x]  
5                            i <- i -1  
6                keyi+1[x] <- k 
7                n[x]<-n[x]+1   
8                DISK-WRITE(x)  
9       else while i >= 1 and k < keyi[x]
10                     do i <- i - 1 
11                i <- i + 1 
12               DISK-READ(ci[x]) 
13               if n[ci[x]] = 2t-1 
14                    then B-TREE-SPLIT-CHILD(x,i,ci[x]) 
15                         if k > keyi[x]  
16                             then i <-i + 1 
17               B-TREE-INSERT-NONFULL(ci[x],k)
*/
void B_Tree_Node::insert2(Info k)
{
    int i = key_num-1;

    if (leaf == true)
    {

        while (i > -1 && k < keys[i] )
        {
            keys[i+1] = keys[i];
            i--;
        }

        keys[i+1] = k;
        key_num = key_num+1;
    }
    else // If this node is not leaf
    {
        while (i > -1 && k < keys[i])
            i--;

        i++;
        if (chs[i]->key_num == 2*t-1)
        {
            split(i, chs[i]);

            if (keys[i] < k)
                i++;
        }
        chs[i]->insert2(k);
    }
}

/*
B-TREE-SPLIT-CHILD(x,i,y)  1z<-Allocate_Node 
2   leaf[z] <- leaf[y]  
3   n[z] <- t - 1  
4   for j <- 1 to t - 1  
5        dokeyj[z]<- keyj+t[y]  
6   if not leaf[y]  
7        then for j <- 1 to t  
8              do cj[z] <- cj+t[y] 
9   n[y]<- t-1  
10  for j<- n[x]+1 downto i+1 
11        do cj+1[x] <- cj[x] 
12   ci+1[x] <- z 
13   for j <-n[x] downto i 
14        do keyj+1[x] <- keyj[x] 
15   keyi[x] <- keyt[y] 
16   n[x]<-n[x]+1
*/
void B_Tree_Node::split(int i, B_Tree_Node *fullnode)
{

    B_Tree_Node *z = new B_Tree_Node(fullnode->t, fullnode->leaf);
    z->key_num = t - 1;

    int j = 0;
    while(j < t-1){
        z->keys[j] = fullnode->keys[t+j];
        j++;
    }


    if (fullnode->leaf != true)
    {
        for (int j = 0; j < t; j++)
            z->chs[j] = fullnode->chs[j+t];
    }


    fullnode->key_num = t - 1;


    for (int j = key_num; j > i; j--)
        chs[j+1] = chs[j];

    
    chs[i+1] = z;

    for (int j = key_num-1; j > i-1; j--)
        keys[j+1] = keys[j];

    keys[i] = fullnode->keys[t-1];

    key_num = key_num + 1;
}

int main()
{
    char platform = 'h'; // h for hackerrank and l for local, read from file 

    if (platform == 'h'){
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
    t.traverse();
    }
    else {
        ifstream ip;
        ip.open("input.txt");

        BTree t(3);

        int count;
        string _count;
        string _degree;
        string _op;
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
    }
        


    return 0;
}
