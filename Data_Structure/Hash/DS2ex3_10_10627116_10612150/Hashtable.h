// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "Data.h"
#include "Prime.h"
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

class Table {
public:
    int hashcode;
    Column column;

    friend ostream &operator<<(ostream &out, Table &table);
};

class Hashtable {
protected:
    int rows;
    int size;    // prime
    int maxStep; // prime

    double successful;
    double unsuccessful;
    Table *hashtable;

    int hash(char *str, int num);
    virtual void setMaxStep();
    virtual int getStep(char *str, int collisions);
    void setSize();

public:
    Hashtable(int size);
    void clear();

    virtual void insert(Data &data);
    virtual void save(fstream &fout);
};

class Hashtable_Linear : public Hashtable {
public:
    Hashtable_Linear(int size) : Hashtable(size) { setMaxStep(); };
    virtual void save(fstream &fout);
};

class Hashtable_Double : public Hashtable {
    virtual void setMaxStep();
    virtual int getStep(char *str, int collisions);

public:
    Hashtable_Double(int size) : Hashtable(size) { setMaxStep(); };
    virtual void save(fstream &fout);
};

class Hashtable_Quadratic : public Hashtable {
public:
    Hashtable_Quadratic(int size) : Hashtable(size) { setMaxStep(); };

    virtual int getStep(char *str, int collisions);
    virtual void save(fstream &fout);
};

#endif

