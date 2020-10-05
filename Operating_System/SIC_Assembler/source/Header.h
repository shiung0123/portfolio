#ifndef _HEADER_H_
#define _HEADER_H_

#include <string>
#include <sstream>
#include <algorithm>
#include <bitset>
using namespace std;

static string fillzero(string str, int size) {
    while (str.length() < size) {
        str.insert(0, "0");
    }
    return str;
}

static string b_strhex(string bin) {
    int num = bin.size() / 8;
    stringstream ss;
    string temp;
    if (num == 1) { 
        bitset<8> set(bin);
        ss << hex << set.to_ulong();
        temp = fillzero(ss.str(), 2);
    }
    else if (num == 2) {
        bitset<16> set(bin);
        ss << hex << set.to_ulong();
        temp = fillzero(ss.str(), 4);
    }
    else if (num == 3) {
        bitset<24> set(bin);
        ss << hex << set.to_ulong();
        temp = fillzero(ss.str(), 6);
    }
    else if (num == 4) {
        bitset<32> set(bin);
        ss << hex << set.to_ulong();
        temp = fillzero(ss.str(), 8);
    }
    transform(temp.begin(), temp.end(), temp.begin(), toupper);
    return temp;
}

static int strint_int(string str) {
    stringstream ss;
    ss << str;
    int value;
    ss >> value;
    return value;
}

static string strint_strhex(string str) {
    stringstream ss1;
    ss1 << str;
    int value;
    ss1 >> value;
    stringstream ss2;
    ss2 << hex << value;
    string temp = ss2.str();
    transform(temp.begin(), temp.end(), temp.begin(), toupper);
    return temp;
}

static string int_strb(int num, int digit) {
    string temp = "";
    for (int i = digit - 1; i >= 0; i--) {
        int k = num >> i;
        if (k & 1)
            temp += "1";
        else
            temp += "0";
    }
    return temp;
}

static string int_strhex(int num) {
    stringstream ss;
    ss << hex << num;
    string temp = ss.str();
    transform(temp.begin(), temp.end(), temp.begin(), toupper);
    return temp;
}

static int strhex_int(string str) {
    stringstream ss;
    ss << str;
    int value;
    ss >> hex >> value;
    return value;
}

static string strchar_strhex(string str) {
    stringstream ss;
    for (char c : str) {
        int value = int(c);
        ss << hex << value;
    }
    string temp = ss.str();
    transform(temp.begin(), temp.end(), temp.begin(), toupper);
    return temp;
}


#endif
