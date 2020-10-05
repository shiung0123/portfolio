// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _MERGE_H_
#define _MERGE_H_
#include "Buffer.h"
#include <fstream>

using namespace std;

// return sorted file
string mergeSort(string nameA, string nameB);
void mergeSortBlock(BufferRead &b1, BufferRead &b2, BufferWrite &w);

#endif

