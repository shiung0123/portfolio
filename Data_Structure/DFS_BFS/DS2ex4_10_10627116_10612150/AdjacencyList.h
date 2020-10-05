// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _ADJACENCY_LISTS_H_
#define _ADJACENCY_LISTS_H_

#include "Data.h"
#include <fstream>
#include <map>
#include <vector>

using namespace std;

#define EXIT -1
#define BINARY 0

typedef char ID[10];

typedef struct link {
    string id;
    float weight;
} Link;

typedef struct node {
    string id;
    vector<Link> linked;
} Node;

typedef struct InfluenceNode {
    string id;
    vector<string> list;
} INode;

class AdjacencyList {
    vector<node> nodes;
    map<string, int> indexMapping;

    fstream fin;
    fstream fout;
    string fileName;

    void isExistID(ID);

    // common function
    int numberInput(const string &message, const string &errorMsg);

    int fileInput(const string &message, const string &prefix);

    // Add node if not exist
    vector<node>::iterator addNode(const string &id);

    // build map
    void buildMapping();

    void BFS(const string &, vector<string> &);

    // connect A to B, weight
    void connect(ID A, ID B, float weight);

    // test function
    void debug_buildMapping();

public:
    void clear();
    bool task1();
    bool task2();
};

#endif

