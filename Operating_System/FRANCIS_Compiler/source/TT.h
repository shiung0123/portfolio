#ifndef _TT_H_
#define _TT_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <algorithm>
using namespace std;

class TokenTable {
    class ID {
    public:
        string id;
        int scope;
        int type;
        int pointer;
    };
    vector<string> table[6];
    vector<ID> id_table;
    bool readTable(int i, fstream& fin);
    pair<int, int> getCharIndex(char c);
    pair<int, int> findToken(string& str, int& type);
    pair<int, int> insertToken(string& str, int& type);
    int hash(string& str);
public:
    int Menu();
    void printTable_12345();
    void printTable_ID();
    void clear();
    vector<pair<int, int>> getToken(string line);
    pair<int, int> getTokenIndex(string str, int type);
    string getStr(pair<int, int> index) const;
    pair<int, int> insertID(string a, int b, int c, int d);
    pair<int, int> findID(string a, int b);
    int updateID(string a, int b, int d);
    int getPtr(string a, int b);
    int getPtr(int index);
    string getID(int index);
};

#endif