// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _HANDLEFILE_H_
#define _HANDLEFILE_H_

#include "Data.h"
#include "Hashtable.h"
#include "Header.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
using namespace std;

#define EXIT -1
#define NORMAL 0
#define BINARY 1

class HandleFile {
    fstream fin;
    fstream fout;
    string fileName;

    // common function
    int numberInput(string message, string errorMsg);

    void save(Hashtable &table, string prefix);

    int fileInput(string message, string prefix);

    int getRow();

    bool txtToBin(string prefix);

public:
    bool task1();
    bool task2();
    bool task3();
};

#endif
