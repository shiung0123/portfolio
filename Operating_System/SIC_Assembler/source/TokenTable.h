#ifndef _TOKENTABLE_H_
#define _TOKENTABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <algorithm>
using namespace std;

class TokenTable {
    vector<string> table[8];
    bool readTable(int i, fstream& fin);
    pair<int, int> findToken(string& str, int& type);
    int hash(string& str);
    pair<int, int> insertToken(string& str, int& type);
public:
    bool read_success;
    int Menu();
    void printTable();
    pair<int, int> getTokenIndex(string str, int type);
    pair<int, int> findCharIndex(char c);
    string getStr(pair<int, int> index);
    TokenTable() {
        read_success = false;
    }
};

#endif