#ifndef _SA_H_
#define _SA_H_

#include "TT.h"

extern int enp;
extern int ens;
using namespace std;

bool checkSyntax(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkBlock(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkVariable(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkType(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkLabel(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkDimension(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkSubVariable(const vector<pair<int, int>>& tokens, const TokenTable& tokentable, int& i);
bool checkStatement(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkStatementI(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkIO(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkVar(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkGto(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkCall(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkArg(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkIF(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkCond(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkAssign(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);

bool checkProgram(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkSubroutine(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkParGroup(const vector<pair<int, int>>& tokens, const TokenTable& tokentable);
bool checkPar(const vector<pair<int, int>>& tokens, const TokenTable& tokentable, int& i);

#endif