#ifndef _SA_H_
#define _SA_H_

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "TokenTable.h"
#include "InsTable.h"
#include "Table.h"
#include "Header.h"
using namespace std;

string checkSyntax_SICXE(vector<pair<int, int>>& tokens, TokenTable& table, InsTable& instable, SymbolTable& symtable, LiteralTable& littable, int& add, int& b);
string checkSyntax_SIC(vector<pair<int, int>>& tokens, TokenTable& table, InsTable& instable, SymbolTable& symtable, LiteralTable& littable, int& add, int& b);

#endif
