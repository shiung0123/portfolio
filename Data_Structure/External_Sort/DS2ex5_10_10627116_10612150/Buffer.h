// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#pragma once
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Data.h"
#include <fstream>
#include <iostream>

using namespace std;

#define BUFFER_READ_SIZE 50
#define BUFFER_WRITE_SIZE 100
#define BUFFER_MAX_SIZE 200

class BufferRead {
    fstream *fs;
    int index;
    int head;
    int readLimit;
    int size, readSize;

    void load();

public:
    operator bool();
    Column *buffer;
    BufferRead(fstream *in, int limit, int bufferSize);
    void resetLimit(int);
    void setIn(fstream *in, int newHead);
    Column *read();
    Column *getCurrent();
    ~BufferRead();
};

class BufferWrite {
    fstream *fs;
    int count;
    Column *buffer;
    int size;

public:
    BufferWrite(fstream *out, int bufferSize);
    void setOut(fstream *out);
    void write(Column *data);
    void flush();
    ~BufferWrite();
};

#endif
