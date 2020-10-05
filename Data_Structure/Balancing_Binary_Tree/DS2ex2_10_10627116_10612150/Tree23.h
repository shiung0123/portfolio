// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _TREE23_H_
#define _TREE23_H_
#include "Data.h"
#include <string>
#include <vector>

#define TREE23_SIZE 3
#define TREE23_KEY_SIZE TREE23_SIZE - 1

using namespace std;

class Tree23 {
public:
    class Data {
    public:
        string key;
        vector<int> id;

        void assign(const Data &);
    };

    class Node {
    public:
        // key size + one buffer
        Data data[TREE23_KEY_SIZE + 1];
        Node *subtree[TREE23_SIZE + 1];
        int size;
        Node *pre;

        void addKey(const Data &);
        void addKey(int id, const string &key);
        void connect(Node *sub, int subIndex);

        bool isLeaf();
        int isNodeN();
        int hasKey(const string &key);
        void sortLeaf();
        void remove(int);
        void merge(Node *);
        Node(Node *);
    };

private:
    Node *root;

    Node *split(Node *);

    void removeNode(Node *cur);
    Node *nextPtr(Node *, const string &key, int &);
    void insertToNode(Node *, int id, const string &key);

public:
    void insert(int id, const string &key);
    int height();
    void getRoot(vector<int> &result);
    Tree23();

    void find(vector<int> &result, const string &);
    void clear();
};

#endif

