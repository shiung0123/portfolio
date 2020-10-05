#ifndef _SYM_H_
#define _SYM_H_

#include <vector>
#include <string>
#include "Header.h"
using namespace std;

class SymbolTable {
    class SYM_Data {
    public:
        int address;
        string label;
        SYM_Data(string str, int add) {
            label = str;
            address = add;
        }
    };
    vector<SYM_Data> sym;
public:
    bool insert(string name, int add) {
        if (get(name) == -1) {
            sym.push_back(SYM_Data(name, add));
            return true;
        }
        else return false;
    }
    int get(string name) {
        for (SYM_Data i : sym) {
            if (i.label == name) return i.address;
        }
        return -1;
    }
    void clear() {
        sym.clear();
    }
};

class LiteralTable {
    class LIT_Data {
    public:
        string data;
        bool xoc;
        int address;
        string strhex;
    };
    vector<LIT_Data> littable;
    int hash(string& str) {
        int sum = 0;
        for (char c : str) {
            sum += c;
        }
        return sum % 100;
    }
public:
    LiteralTable() {
        littable.resize(100);
    }
    void allocate(int& pc) {
        for (int i = 0; i < 100; i++) {
            if (littable[i].data != "" && littable[i].address == 0) {
                littable[i].address = pc;
                if (littable[i].xoc == 0) {
                    if (littable[i].data.size() <= 2) {
                        littable[i].strhex = fillzero(littable[i].data, 2);
                        pc++;
                    }
                    else {
                        int num = (littable[i].data.length() + 2) / 3;
                        littable[i].strhex = fillzero(littable[i].data, num*6);
                        pc += num*3;
                    }
                }
                else {
                    littable[i].strhex = fillzero(strchar_strhex(littable[i].data), 6);
                    pc += littable[i].strhex.length() / 2;
                }
            }
        }
    }
    void insert(string name, int xoc) {
        if (get(name, xoc) == -1) {
            int sum = hash(name);
            for (int i = 0; i < 100; i++) {
                int index = (sum + i) % 100;
                if (littable[index].data == "") {
                    littable[index].data = name;
                    littable[index].xoc = xoc;
                    return;
                }
            }
        }
    }
    int get(string name, int xoc) {
        for (LIT_Data i : littable) {
            if (i.data == name && i.xoc == xoc) return i.address;
        }
        return -1;
    }
    void clear() {
        littable.clear();
        littable.resize(100);
    }
    vector<pair<int,string>> getliteral() {
        vector<pair<int, string>> temp;
        for (LIT_Data i : littable) {
            if (i.address != 0) {
                temp.push_back(make_pair(i.address, i.strhex));
            }
        }
        return temp;
    }
};

#endif



