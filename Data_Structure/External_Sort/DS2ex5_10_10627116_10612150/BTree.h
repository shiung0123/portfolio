// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _BPTREE_H_
#define _BPTREE_H_
#include "Data.h"
#include <string>
#include <vector>

#define MIN_KEY 0
#define MAX_KEY 1
#define TREE_SIZE 5
#define TREE_KEY_SIZE TREE_SIZE - 1
#define DATA_RMID_INDEX (TREE_SIZE + 1) / 2
#define KEY_RMID_INDEX DATA_RMID_INDEX - 1

using namespace std;

class BPTree {
public:
    class Node {
    public:
        Node(Node *);

        // key size + one buffer
        Data data[TREE_KEY_SIZE + 1];
        Node *subtree[TREE_SIZE + 1];
        int size;
        Node *pre;
        Node *left, *right;

        // operate func
        void addKey(const Data);
        void addKey(int id, const float &key);
        void sortLeaf(); // sort data
        void connect(Node *sub, int subIndex); // connect subtree
        void merge(Node *); // merge two nodes
        void remove(int); // remove data

        // status func
        bool isLeaf();
        int hasKey(const float &key);
    };

private:
    Node *root;

    // traversal
    Node *nextPtr(Node *, const float &key, int &); // traversal
    
    // insert
    void insertToNode(Node *, int id, const float &key);
    Node *splitleaf(Node *);
    Node *split(Node *);

    //remove
    void removeNode(Node *cur);

public:
    BPTree();

    // operate func
    void insert(int id, const float &key);
    void insert(const Data &data);
    void find(vector<int> &result, const float &);
    void clear();
    bool empty();

    // status func
    int getHeight();
    void getRoot(vector<int> &result);
    void printList();
    int getOffset(float key);
};

#endif

