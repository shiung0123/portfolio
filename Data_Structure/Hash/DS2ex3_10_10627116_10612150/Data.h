// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _DATA_H_
#define _DATA_H_

#include "Header.h"
#include <iostream>
#include <string>
using namespace std;

# define DATA_SIZE 9

typedef struct Column {
    char sid[10];
    char sname[10];
    unsigned char score[6];
    float average;
} Column;

class Data {
    struct Column column;

public:
    friend istream &operator>>(istream &in, Data &data);

    friend ostream &operator<<(ostream &out, Data &data);

    void setData(int index, string data);

    bool operator>(Data &b);

    bool operator>=(Data &b);

    bool operator<=(Data &b);

    bool operator<(Data &b);

    char* getId();

    struct Column getColumn();

};

#endif

