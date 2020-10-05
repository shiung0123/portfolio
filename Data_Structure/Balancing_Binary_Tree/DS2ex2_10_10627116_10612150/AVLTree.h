// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _AVLTREE_H_
#define _AVLTREE_H_
#include "Data.h"
#include <vector>

using namespace std;

class AVLTree {
public:
    typedef struct Node {
        Node *left, *right;
        int height;
        vector<int> ids;
        string key;
    } Node;

private:
    Node *root;

    Node *LR_roate(Node *);
    Node *RL_roate(Node *);
    Node *LL_roate(Node *);
    Node *RR_roate(Node *);
    void removeNode(Node *cur);
    void newHeight(Node *, int);
    int getBF(Node *);

    Node *insert(Node *node, int id, const string &key);
    void inorder(vector<string> &, Node *);
    void preorder(vector<string> &, Node *);

    void find(vector<int> &result, const string &, Node *);

public:
    void insert(int id, const string &key);
    void inorder(vector<string> &);
    void preorder(vector<string> &);
    void clear();
    void find(vector<int> &result, const string &);
    int height();
    void getRoot(vector<int> &result);
    AVLTree();
};

#endif

