// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version encoding=utf-8
#include "Data.h"
#include <cstring>

istream &operator>>(istream &in, Data &data)
{
    string input, temp;
    getline(in, input);

    if (!in)
        return in;
    // drop \r if the program running on unix
    // or unix like system, the string may be
    // contained '\r'
    if (input.back() == '\r')
        input.pop_back();

    // put \t for split easily
    input += '\t';

    // splitting
    int count = 0;
    inputSuccess = true;
    for (char c : input) {
        if (c != '\t')
            temp += c;
        else {
            switch (count) {
            case 0:
                strcpy(data.column.sid, temp.c_str());
                break;
            case 1:
                strcpy(data.column.sname, temp.c_str());
                break;
            case 8:
                data.column.average = stof(temp);
                break;
            default:
                if (count < 8)
                    data.column.score[count - 2] = stoi(temp);
                break;
            }
            count++;
            temp = "";
        }
    }

    if (count != DATA_SIZE)
        inputSuccess = false;

    return in;
}

ostream &operator<<(ostream &out, Data &data)
{
    for (int i = 0; i < DATA_SIZE; i++) {
        switch (i) {
        case 0:
            out << data.column.sid;
            break;
        case 1:
            out << data.column.sname;
            break;
        case 8:
            out << data.column.average;
            break;
        default:
            out << int(data.column.score[i - 2]);
            break;
        }
        out << (i < DATA_SIZE - 1 ? '\t' : '\n');
    }

    return out;
}

char *Data::getId() { return column.sid; }

struct Column Data::getColumn() { return column; }
