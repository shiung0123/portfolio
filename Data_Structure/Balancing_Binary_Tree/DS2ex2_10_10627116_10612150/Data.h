// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _DATA_H_
#define _DATA_H_

#include "Header.h"
#include <iostream>
#include <string>
using namespace std;

// clang-format off
#define DATA_SIZE               11
#define DATA_ID                 0
#define DATA_NAME               1
#define DATA_DEPARTMENT_ID      2
#define DATA_DEPARTMENT_NAME    3
#define DATA_DAY_EXT_EDU        4
#define DATA_STAGE              5
#define DATA_STUDENTS           6
#define DATA_TEACHERS           7
#define DATA_GRADUATES          8
#define DATA_CITY               9
#define DATA_TYPE               10
// clang-format on

static vector<int> selectOrder = {DATA_STUDENTS};
static vector<int> selectPrint = {DATA_NAME, DATA_DEPARTMENT_NAME,
                                  DATA_DAY_EXT_EDU, DATA_STAGE, DATA_STUDENTS};

class Data {
    string column[DATA_SIZE];
    int cache[DATA_SIZE];
    void updateCache();

public:
    friend istream &operator>>(istream &in, Data &data);

    friend ostream &operator<<(ostream &out, Data &data);

    string getData(int index) const { return column[index]; }

    void setData(int index, string data);

    int convertToInt(int num) { return stringToInt(column[num]); }

    bool operator>(Data &b);

    bool operator>=(Data &b);

    bool operator<=(Data &b);

    bool operator<(Data &b);

    void print();

    void println();
};

#endif

