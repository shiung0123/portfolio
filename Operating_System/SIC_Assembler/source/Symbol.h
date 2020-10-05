#ifndef _TABLE_H_
#define _TABLE_H_

#include <vector>
#include <string>
#include "Header.h"
using namespace std;

class SymbolTable {
    class Data {
    public:
        int address;
        string label;
        Data(string str, int add) {
            label = str;
            address = add;
        }
    };
    vector<Data> sym;
public:
    void insert(string name, int add) {
        if(get(name) == -1) sym.push_back(Data(name, add));
    }
    int get(string name) {
        for (Data i : sym) {
            if (i.label == name) return i.address;
        }
        return -1;
    }
};

class LiteralTable {
    class Data {
    public:
        string data;
        bool xoc;
        int address;
        string strhex;
        Data(string str, int boo, int add) {
            data = str;
            xoc = boo;
            address = add;
        }
    };
    vector<Data> littable;
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
        for (Data i : littable) i.address == -1;
    }
    void allocate(int& pc) {
        for (Data i : littable) {
            if (i.data != "" && i.address == -1) {
                i.address = pc;
                if (i.xoc == 0) {
                    i.strhex = fillzero(i.data, 6);
                    pc += 3;
                }
                else {
                    i.strhex = fillzero(strchar_strhex(i.data), 6);
                    pc += i.strhex.length() / 2;
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
                }
            }
        }
    }
    int get(string name, int xoc) {
        for (Data i : littable) {
            if (i.data == name && i.xoc == xoc) return i.address;
        }
        return -1;
    }
};

#endif



