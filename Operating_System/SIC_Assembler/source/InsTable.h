#ifndef _INSTABLE_H_
#define _INSTABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Header.h"
using namespace std;

class InsTable {
    class INS_Data {
    public:
        string ins;
        int op_code;
        int type;
        INS_Data(string str, int op, int t) {
            ins = str;
            op_code = op;
            type = t;
        }
    };
    vector<INS_Data> ins_data;
    vector<string> split(string str);
    bool readTable(fstream& fin);

public:
    bool read_success;
    int Menu();
    int getType(int index);
    int getOp(int index);
    int getReg(int index);
    InsTable() {
        read_success = false;
    }
};

#endif
