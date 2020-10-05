#ifndef _CA_H_
#define _CA_H_

#include <iostream>
#include <fstream>
#include "TokenTable.h"
#include "InsTable.h"
#include "Header.h"
#include "Table.h"
#include "LA.h"
#include "SA.h"
using namespace std;

class INS {
public:
    int line;
    int loc; // print hex
    string str;
    vector<pair<int, int>> tokens;
    string code;
    int base;
    INS(int a, int b, string c, vector<pair<int, int>> d, string e, int f) {
        line = a;
        loc = b;
        str = c;
        tokens = d;
        code = e;
        base = f;
    }
};

class CrossAssembler {
    vector<INS> data;
    TokenTable tokentable;
    InsTable instable;
    SymbolTable symtable;
    LiteralTable littable;
    int Pass1(fstream& fin, fstream& fout, bool t);
    int Pass2(bool t);
    string disp(int pc, int base, int loc, char c);
    void generate_SICXE();
    void generate_SIC();
public:
    int Menu();
    void print(fstream& fout);
};

#endif