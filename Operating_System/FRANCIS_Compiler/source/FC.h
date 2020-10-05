#ifndef _FC_H_
#define _FC_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "TT.h"
#include "SA.h"
using namespace std;

std::ostream& operator << (std::ostream& out, const vector<pair<int, int>>& vec_index);

class DATA {
public:
    string code;
    vector<pair<int, int>> tokens;
    int check;
    DATA(string c, vector<pair<int, int>> d, int e) {
        code = c;
        tokens = d;
        check = e;
    }
};

class INS {
public:
    int no;
    vector<pair<int, int>> inter;
    string str;
    int done; // 0 check error, 1 done, 2 GTO
    INS(int a, vector<pair<int, int>> b, string c, int e) {
        no = a;
        inter = b;
        str = c;
        done = e;
    }
};

class FCompiler {
    fstream fin;
    fstream fout;
    vector<DATA> data;
    vector<INS> table6;
    vector<int> table7;
    TokenTable tokentable;

    int Pass1();
    int Pass2();
    void printInter(vector<pair<int, int>> tokens);
    vector<vector<pair<int, int>>> split(vector<pair<int, int>> tokens, string c);
    string intertos(vector<pair<int, int>> tokens);
    bool cmp(pair<int, int> index1, pair<int, int> index2);
    int cmp_num(string str);

    void generate(DATA d);
    void generate_PROGRAM(vector<pair<int, int>> tokens);
    void generate_SUBROUTINE(vector<pair<int, int>> tokens);
    void generate_VARIABLE(vector<pair<int, int>> tokens);
    void generate_DIMENSION(vector<pair<int, int>> tokens);
    void generate_LABEL(vector<pair<int, int>> tokens);
    int generate_GTO(vector<pair<int, int>> tokens, string pre_label);
    int generate_CALL(vector<pair<int, int>> tokens, string str);
    int generate_INPUT(vector<pair<int, int>> tokens, string line);
    int generate_OUTPUT(vector<pair<int, int>> tokens, string line);
    int generate_ASSIGN(vector<pair<int, int>> tokens, string line);
    void generate_IF(vector<pair<int, int>> tokens, string line);
    int generate_stamentI(vector<pair<int, int>> tokens, string line);
    pair<int, int> generate_ELEMENT(vector<pair<int, int>> tokens, string& line, int& temp);
    pair<int, int> generate_ARRAYELEMENT(vector<vector<pair<int, int>>> element, int table7_index, int& temp);
    pair<int, int> generate_SENTENCE(vector<pair<int, int>> tokens, string& line, int& temp);

public:
    int Menu();
    int Entry();
    void print();
    void printTable7();
};

#endif