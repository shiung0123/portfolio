#ifndef _TOKENFILE_H_
#define _TOKENFILE_H_

#include <iostream>
#include <fstream>
#include "TokenTable.h"
using namespace std;

class TokenFile {
    int getToken(fstream& fin, fstream& fout, TokenTable& table);
    void printIndex(fstream& fout, const pair<int, int>& index);
public:
    int Menu(TokenTable& table);

};

#endif