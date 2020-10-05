// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _DATA_H_
#define _DATA_H_

#include "Header.h"
#include <iostream>
#include <string>
using namespace std;

#define DATA_SIZE 3

typedef struct Column {
    char putID[10];
    char getID[10];
    float weight;
} Column;

#endif

