//  EEN318
//  AVL Tree ! ! ! !
//  Created by Andrew Stockton on 11/25/14.
//  Copyright (c) 2014 Andrew Stockton. All rights reserved.
//



#include <string>
#include <iostream>
#include <fstream>

using namespace std;

const bool debugging = false;

struct person {
    string fname, lname, state;
    int social, dob, zip;
    person(){
        social = NULL;
        dob = NULL;
        zip = NULL;
        fname = "";
        lname = "";
        state = "";
    }
    
    person(int d, int SSN, string f, string l, string s, int z) {
        social = SSN;
        fname = f;
        lname = l;
        dob = d;
        state = s;
        zip = z; }
    
    void print() {
        cout << dob << " " << fname << " " << lname << " " << endl; }
    
    bool lessthan(person * c) {
        int lcomp = lname.compare(c->lname);
        if(lcomp < 0 ) {
            return true;
        }
        else if(lcomp > 0) {
            return false;
        }
        else {
            int fcomp = fname.compare(c->fname);
            if(fcomp < 0 ) {
                return true;
            }
            else if(fcomp > 0) {
                return false;
            }
            else {
                if(dob < c->dob) {
                    return true;
                }
                else
                    return false;
            }
        }
    }
};

struct avlnode {
    person * data;
    struct avlnode * left, * right;
};

struct avltree {
    avlnode * root;
    avltree() { root = NULL; }
    int height(avlnode * n);
    int difference(avlnode * n);
    avlnode * rightright(avlnode * n);
    avlnode * leftleft(avlnode * n);
    avlnode * leftright(avlnode * n);
    avlnode * rightleft(avlnode * n);
    avlnode * balance(avlnode * n);
    avlnode * insert(avlnode *, person * p);
    void inorder(avlnode * n);
    void readfile(string f);
    void output_file();
    void output_tree(ofstream & output, avlnode * n);
};

int main( int argc, char *argv[] ) {
    avltree avl;
    avl.readfile( argv[1] );
    avl.inorder(avl.root);
    avl.output_file();
    return 0;
}

int avltree::height(avlnode * n) {
    int h = 0;
    if (n != NULL) {
        int left_height = height(n->left);
        int right_height = height(n->right);
        int maxheight;
        if(left_height > right_height)
            maxheight = left_height;
        else
            maxheight = right_height;
        h = maxheight + 1;
    }
    return h;
}

int avltree::difference(avlnode * n) {
    int left_height = height(n->left);
    int right_height = height(n->right);
    int dif = left_height - right_height;
    return dif;
}

avlnode * avltree::rightright(avlnode * n) {
    avlnode * temp;
    temp = n->right;
    n->right = temp->left;
    temp->left = n;
    return temp;
}

avlnode * avltree::leftleft(avlnode * n) {
    avlnode * temp;
    temp = n->left;
    n->left = temp->right;
    temp->right = n;
    return temp;
}

avlnode * avltree::leftright(avlnode * n) {
    avlnode * temp;
    temp = n->left;
    n->left = rightright(temp);
    return leftleft(n);
}

avlnode * avltree::rightleft(avlnode * n) {
    avlnode * temp;
    temp = n->right;
    n->right = leftleft(temp);
    return rightright(n);
}

avlnode * avltree::balance(avlnode * n) {
    int b = difference(n);
    if (b > 1) {
        if (difference(n->left) > 0)
            n = leftleft(n);
        else
            n = leftright(n);
    }
    else if (b < -1) {
        if (difference(n->right) > 0)
            n = rightleft(n);
        else
            n = rightright(n);
    }
    return n;
}

avlnode * avltree::insert(avlnode * root, person * p) {
    if (root == NULL) {
        root = new avlnode;
        root->data = p;
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    else if (p->lessthan(root->data)) {
        root->left = insert(root->left, p);
        root = balance(root);
    }
    else if (!p->lessthan(root->data)) {
        root->right = insert(root->right, p);
        root = balance(root);
    }
    return root;
}

void avltree::inorder(avlnode * tree) {
    if (tree == NULL) return;
    inorder (tree->left);
    tree->data->print();
    inorder (tree->right);
}

void avltree::output_file() {
    ofstream fout("avlTreeSorted.txt");
    if(fout.fail()) {
        cout << "can't create avlTreeSorted.txt" << endl;
        exit(1); }
    output_tree(fout, root);
}

void avltree::output_tree(ofstream & output, avlnode * n){
    if(n -> left != NULL) output_tree(output, n->left);
    output  << n->data->dob << " "
    << n->data->social << " "
    << n->data->fname << " "
    << n->data->lname << " "
    << n->data->state << " "
    << n->data->zip << endl;
    if (n -> right != NULL) output_tree(output, n->right);
}

void avltree::readfile(string fileinput) {
    ifstream fin(fileinput.c_str());
    if (fin.fail())
    { cout << "Failed to open file\n"; exit(1); }
    while (!fin.fail())
    {   string first, last, state;
        int soc, dob, zipc;
        if ( fin.fail() ) {  break; }
        fin >> dob >> soc >> first >> last >> state >> zipc;
        person * p = new person(dob, soc, first, last, state, zipc);
        if(debugging) if(root == NULL) cout << "so null\n";
        if(fin.fail() ) { break; }
        root = insert(root, p);
    }
    fin.close();
}
