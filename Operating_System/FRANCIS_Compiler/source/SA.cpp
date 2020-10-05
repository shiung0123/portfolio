#include "SA.h"

int enp = 0;
int ens = 0;

bool checkSyntax(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    if (tokentable.getStr(tokens.back()) == ";") {
        vector<pair<int, int>> token(tokens.begin(), tokens.end()-1);
        return checkBlock(token, tokentable);
    }
    else return false;
}

bool checkBlock(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    if (checkProgram(tokens, tokentable)) {
        enp++;
        if (enp == 1) return true;
        else {
            enp--;
            return false;
        }
    }
    // enp == 2 &&
    if ( checkSubroutine(tokens, tokentable)) {
        ens++;
        if (ens == 1) return true;
        else {
            ens--;
            return false;
        }
    }
    if (enp == 1 || ens == 1) {
        if (enp == 1 && tokens.size() == 1 && tokentable.getStr(tokens.front()) == "ENP" 
            || tokens.size() == 2 && tokens.front().first == 5 && tokentable.getStr(tokens.back()) == "ENP") {
            enp++;
            return true;
        }
        else if (ens == 1 && tokens.size() == 1 && tokentable.getStr(tokens.front()) == "ENS"
            || tokens.size() == 2 && tokens.front().first == 5 && tokentable.getStr(tokens.back()) == "ENS") {
            ens--;
            return true;
        }
        else if (checkVariable(tokens, tokentable) || checkLabel(tokens, tokentable) ||
            checkDimension(tokens, tokentable) || checkStatement(tokens, tokentable)) {
            return true;
        }
    }
    else return false;
}

bool checkVariable(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int ii = 0;
    while (i+ii < tokens.size()) {
        switch (block)
        {
        case 1: 
        {
            if (tokentable.getStr(tokens[i]) != "VARIABLE") return false;
            i++, block++;
            break;
        }
        case 2:
        {
            vector<pair<int, int>> token(tokens.begin() + i, tokens.begin() + i + 1);
            if (!checkType(token, tokentable)) return false;
            i++, block++;
            break;
        }
        case 3:
        {
            if (tokentable.getStr(tokens[i]) != ":") return false;
            i++, block++;
            break;
        }
        case 4:
        {
            if (tokens[i].first != 5) return false;
            i++, block++;
            ii = 1;
            if (i == tokens.size()) return true;
            break;
        }
        case 5:
        {
            if (tokentable.getStr(tokens[i]) != "," || tokens[i+1].first != 5) return false;
            i += 2;
            ii = 1;
            if (i == tokens.size()) return true;
            break;
        }
        }
    }
    return false;
}

bool checkType(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    string type = tokentable.getStr(tokens.back());
    if (type == "INTEGER" || type == "REAL" || type == "BOOLEAN") return true;
    else return false;
}

bool checkLabel(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            if (tokentable.getStr(tokens[i]) != "LABEL") return false;
            i++, block++;
            break;
        }
        case 2:
        {
            if (tokens[i].first != 5) return false;
            i++, block++;
            ii = 1;
            if (i == tokens.size()) return true;
            break;
        }
        case 3:
        {
            if (tokentable.getStr(tokens[i]) != "," || tokens[i + 1].first != 5) return false;
            i += 2;
            ii = 1;
            if (i == tokens.size()) return true;
            break;
        }
        }
    }
    return false;
}

bool checkDimension(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            if (tokentable.getStr(tokens[i]) != "DIMENSION") return false;
            i++, block++;
            break;
        }
        case 2:
        {
            vector<pair<int, int>> token(tokens.begin() + i, tokens.begin() + i + 1);
            if (!checkType(token, tokentable)) return false;
            i++, block++;
            break;
        }
        case 3:
        {
            if (tokentable.getStr(tokens[i]) != ":") return false;
            i++, block++;
            break;
        }
        case 4:
        {
            vector<pair<int, int>> token(tokens.begin() + i, tokens.end());
            int disp;
            if (!checkSubVariable(token, tokentable, disp)) return false;
            i += disp;
            block++;
            ii = 4;
            if (i == tokens.size()) return true;
            break;
        }
        case 5:
        {
            vector<pair<int, int>> token(tokens.begin() + i + 1, tokens.end());
            int disp;
            if (tokentable.getStr(tokens[i]) != "," || !checkSubVariable(token, tokentable, disp)) return false;
            i += disp + 1;
            ii = 4;
            if (i == tokens.size()) return true;
            break;
        }
        }
    }
    return false;
}

bool checkSubVariable(const vector<pair<int, int>>& tokens, const TokenTable& tokentable, int& i) {
    int block = 1;
    i = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            if (tokens[i].first != 5) return false;
            i++, block++;
            break;
        }
        case 2:
        {
            if (tokentable.getStr(tokens[i]) != "(") return false;
            i++, block++;
            break;
        }
        case 3:
        {
            if (tokens[i].first != 3) return false;
            i++, block++;
            break;
        }
        case 4:
        {
            if (tokentable.getStr(tokens[i]) == ")") {
                i++;
                return true;
            }
            else if (tokentable.getStr(tokens[i]) == ",") {
                i++;
                block--;
            }
            else return false;
            break;
        }
        }
    }
    return false;
}

bool checkStatement(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    if (tokens.size() != 0 && tokens.front().first == 5) {
        vector<pair<int, int>> token(tokens.begin() + 1, tokens.end());
        if (checkIF(token, tokentable)) return true;
        else {
            return (checkStatementI(token, tokentable) || checkStatementI(tokens, tokentable));
        }
    }
    else {
        if (checkIF(tokens, tokentable)) return true;
        else return checkStatementI(tokens, tokentable);
    }
}

bool checkStatementI(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    /*
    empty || checkIO(tokens, tokentable) ||
    checkGto(tokens, tokentable) || checkCall(tokens, tokentable) || 
    checkAssign(tokens, tokentable)
    */
    if (tokens.size() == 0 || checkIO(tokens, tokentable) ||
        checkGto(tokens, tokentable) || checkCall(tokens, tokentable) ||
        checkAssign(tokens, tokentable)) {
        return true;
    }
    else return false;
}

bool checkIO(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    if (tokentable.getStr(tokens.front()) == "INPUT" || tokentable.getStr(tokens.front()) == "OUTPUT") {
        vector<pair<int, int>> token(tokens.begin() + 1, tokens.end());
        return checkVar(token, tokentable);
    }
    else return false;
}

bool checkVar(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            if (tokens[i].first != 5) return false;
            i++, block++;
            if (i == tokens.size()) return true;
            break;
        }
        case 2:
        {
            if (tokentable.getStr(tokens[i]) != "(") return false;
            i++, block++;
            break;
        }
        case 3:
        {
            if (tokens[i].first != 3 && tokens[i].first != 5) return false;
            i++, block++;
            break;
        }
        case 4:
        {
            if (tokentable.getStr(tokens[i]) == ")") {
                i++;
                if(i == tokens.size()) return true;
            }
            else if (tokentable.getStr(tokens[i]) == ",") {
                i++;
                block--;
            }
            else return false;
            break;
        }
        }
    }
    return false;
}

bool checkGto(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    if (tokens.size() == 2 &&
        tokentable.getStr(tokens.front()) == "GTO" &&
        tokens.back().first == 5) return true;
    else return false;
}

bool checkCall(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            if (tokentable.getStr(tokens[i]) != "CALL") return false;
            i++, block++;
            break;
        }
        case 2:
        {
            if (tokens[i].first != 5) return false;
            i++, block++;
            break;
        }
        case 3:
        {
            if (tokentable.getStr(tokens[i]) != "(" ||
                tokentable.getStr(tokens.back()) != ")") return false;
            i++, block++;
            break;
        }
        case 4:
        {
            vector<pair<int, int>> token(tokens.begin() + i, tokens.end() - 1);
            return checkArg(token, tokentable);
            break;
        }
        }
    }
    return false;
}

bool checkArg(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            if (tokens[i].first == 5 || tokens[i].first == 3 || tokens[i].first == 4) {
                i++, block++;
                if (i == tokens.size()) return true;
            }
            else if (tokentable.getStr(tokens[i]) == "-") {
                i++, block = 3;
            }
            else return false;
            break;
        }
        case 2:
        {
            if (tokentable.getStr(tokens[i]) != ",") return false;
            i++, block--;
            break;
        }
        case 3:
        {
            if (tokens[i].first != 3 && tokens[i].first != 4) return false;
            i++, block--;
            if (i == tokens.size()) return true;
            break;
        }
        }
    }
    return false;
}

bool checkIF(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int ii = 0;
    int start;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            if (tokentable.getStr(tokens[i]) != "IF") return false;
            i++, block++;
            start = i;
            break;
        }
        case 2:
        {
            if (tokentable.getStr(tokens[i]) != "THEN") i++;
            else {
                vector<pair<int, int>> token(tokens.begin() + start, tokens.begin() + i);
                if (!checkCond(token, tokentable)) return false;
                i++, block++;
                start = i;
                if (i == tokens.size()) return true;
            }
            break;
        }
        case 3:
        {
            if (tokentable.getStr(tokens[i]) == "ELSE") {
                vector<pair<int, int>> token(tokens.begin() + start, tokens.begin() + i);
                if (!checkStatementI(token, tokentable)) return false;
                i++, block++;
                if (i == tokens.size()) return true;
            }
            else {
                i++;
                if (i == tokens.size()) {
                    vector<pair<int, int>> token(tokens.begin() + start, tokens.end());
                    if (checkStatementI(token, tokentable)) return true;
                    else return false;
                }
            }
            break;
        }
        case 4:
        {
            vector<pair<int, int>> token(tokens.begin() + i, tokens.end());
            if (checkStatementI(token, tokentable)) return true;
            else return false;
            break;
        }
        }
    }
    return false;
}

bool checkCond(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int cond_t = 0;
    int start = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            string cond = tokentable.getStr(tokens[i]);
            if (cond == "EQ" || cond == "NE" || cond == "GT" || cond == "GE" || cond == "LT" || cond == "LE" || cond == "OR" || cond == "AND") block++;
            else i++;
            break;
        }
        case 2:
        {
            vector<pair<int, int>> token1(tokens.begin() + start, tokens.begin() + i);
            vector<pair<int, int>> token2(tokens.begin() + i + 1, tokens.end());
            bool b1 = (token1.size() == 1 && token1.front().first == 3 ||
                token1.size() == 2 && tokentable.getStr(token1.front()) == "-" && token1.back().first == 3 ||
                checkVar(token1, tokentable));
            bool b2 = ( token2.size() == 1 && token2.front().first == 3 ||
                token2.size() == 2 && tokentable.getStr(token2.front()) == "-" && token2.back().first == 3 ||
                checkVar(token2, tokentable));
            if ( b1 && b2 ) return true;
            else return false;
            break;
        }
        }
    }
    return false;
}

bool checkAssign(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int start;
    int p = 0;
    int sign = 0;
    vector<pair<int, int>> token_var;
    int i = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            if (tokentable.getStr(tokens[i]) != "=") i++;
            else {
                vector<pair<int, int>> token(tokens.begin(), tokens.begin() + i);
                if (!checkVar(token, tokentable)) return false;
                i++, block++;
                start = i;
            }
            break;
        }
        case 2:
        {
            if (p < 0) return false;
            else {
                if (tokentable.getStr(tokens[i]) == "(") {
                    if(i == 0 || i > 0 && tokens[i-1].first != 5) sign = 0;
                    p++;
                }
                else if (tokentable.getStr(tokens[i]) == ")") p--;

                if (tokens[i].first == 2 &&
                    (tokens[i].second == 1 || (tokens[i].second >= 8 && tokens[i].second <= 10) || (tokens[i].second >= 16 && tokens[i].second <= 19)) ||
                    tokens[i].first == 1 &&
                    (tokens[i].second >= 5 && tokens[i].second <= 9)) {
                    while (!token_var.empty() && tokentable.getStr(token_var.front()) == "(") token_var.erase(token_var.begin());
                    while (!token_var.empty() && tokentable.getStr(token_var.back()) == ")") token_var.pop_back();
                    vector<pair<int, int>>::iterator it = find(token_var.begin(), token_var.end(), make_pair(1, 2));
                    if (it != token_var.end()) token_var.push_back(make_pair(1, 3));

                    if (token_var.empty()) {
                        if (sign == 0 && tokens[i].second >= 5 && tokens[i].second <= 6) {
                            sign++;
                        }
                        else return false;
                    }
                    else {
                        if (!checkVar(token_var, tokentable) && !token_var.empty() &&
                            (token_var.front().first != 3 && token_var.front().first != 4)) return false;
                        sign = 0;
                    }
                    token_var.clear();
                }
                else token_var.push_back(tokens[i]);
                i++;
                
                if (i == tokens.size() && p == 0) {
                    while (!token_var.empty() && tokentable.getStr(token_var.front()) == "(") token_var.erase(token_var.begin());
                    while (!token_var.empty() && tokentable.getStr(token_var.back()) == ")") token_var.pop_back();
                    vector<pair<int, int>>::iterator it = find(token_var.begin(), token_var.end(), make_pair(1, 2));
                    if (it != token_var.end()) token_var.push_back(make_pair(1, 3));

                    if (token_var.empty()) return false;
                    else {
                        if (!token_var.empty() && checkVar(token_var, tokentable) || 
                            !token_var.empty() && (token_var.front().first == 3 || token_var.front().first == 4)) return true;
                    }
                }
            }
            break;
        }
        /*
        case 2:
        {
            if (p < 0) return false;
            else {
                if (tokentable.getStr(tokens[i]) == "(") p++;
                else if (tokentable.getStr(tokens[i]) == ")") p--;

                if (tokens[i].first == 2 &&
                    (tokens[i].second == 1 || (tokens[i].second >= 8 && tokens[i].second <= 10) || (tokens[i].second >= 16 && tokens[i].second <= 19)) ||
                    tokens[i].first == 1 &&
                    (tokens[i].second >= 5 && tokens[i].second <= 9)) {
                    if (token_var.empty()) {
                        if (sign == 0 && tokens[i].second >= 5 && tokens[i].second <= 6) {
                            sign++;
                        }
                        else return false;
                    }
                    else {
                        if (!token_var.empty() && tokentable.getStr(token_var.front()) == "(") token_var.erase(token_var.begin());
                        if (!token_var.empty() && tokentable.getStr(token_var.back()) == ")") token_var.pop_back();
                        if (!checkVar(token_var, tokentable) && !token_var.empty() &&
                            (token_var.front().first != 3 && token_var.front().first != 4)) return false;
                        sign = 0;
                    }
                    token_var.clear();
                }
                else token_var.push_back(tokens[i]);
                i++;
                
                if (i == tokens.size() && p == 0) {
                    if (token_var.empty()) return false;
                    else {
                        if (!token_var.empty() && tokentable.getStr(token_var.front()) == "(") token_var.erase(token_var.begin());
                        if (!token_var.empty() && tokentable.getStr(token_var.back()) == ")") token_var.pop_back();
                        if (!token_var.empty() && checkVar(token_var, tokentable) || 
                            !token_var.empty() && (token_var.front().first == 3 || token_var.front().first == 4)) return true;
                    }
                }
            }
            break;
        }
        */
        }
    }
    return false;
}

bool checkProgram(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    if (tokens.size() == 2 &&
        tokentable.getStr(tokens.front()) == "PROGRAM" &&
        tokens.back().first == 5) return true;
    else return false;
}

bool checkSubroutine(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            if (tokentable.getStr(tokens[i]) != "SUBROUTINE") return false;
            i++, block++;
            break;
        }
        case 2:
        {
            if (tokens[i].first != 5) return false;
            i++, block++;
            break;
        }
        case 3:
        {
            if (tokentable.getStr(tokens[i]) == "(" && tokentable.getStr(tokens.back()) == ")") {
                vector<pair<int, int>> token(tokens.begin() + i + 1, tokens.end() - 1);
                if (checkParGroup(token, tokentable)) return true;
                else return false;
            }
            else return false;
            break;
        }
        }
    }
    return false;
}

bool checkParGroup(const vector<pair<int, int>>& tokens, const TokenTable& tokentable) {
    int block = 1;
    int i = 0;
    int ii = 0;
    while (i + ii < tokens.size()) {
        switch (block)
        {
        case 1:
        {
            vector<pair<int, int>> token(tokens.begin() + i, tokens.begin() + i + 1);
            if (!checkType(token, tokentable)) return false;
            i++, block++;
            break;
        }
        case 2:
        {
            if (tokentable.getStr(tokens[i]) != ":") return false;
            i++, block++;
            ii = 0;
            break;
        }
        case 3:
        {
            vector<pair<int, int>> token(tokens.begin() + i, tokens.end());
            int disp;
            if (!checkPar(token, tokentable, disp)) return false;
            i += disp;
            block++;
            ii = 1;
            if (i == tokens.size()) return true;
            break;
        }
        case 4:
        {
            if (tokentable.getStr(tokens[i]) != ",") return false;
            i++;
            ii = 0;
            block++;
            break;
        }
        case 5:
        {
            vector<pair<int, int>> token(tokens.begin() + i, tokens.end());
            vector<pair<int, int>> token2(tokens.begin() + i, tokens.begin() + i + 1);
            int disp;
            if (checkPar(token, tokentable, disp)) {
                i += disp;
                ii = 1;
                block = 4;
                if (i == tokens.size()) return true;
            }
            else if (checkType(token2, tokentable)) {
                i++;
                block = 2;
            }
            else return false;
            break;
        }
        }
    }
    return false;
}

bool checkPar(const vector<pair<int, int>>& tokens, const TokenTable& tokentable, int& i) {
    if (tokens.size() >= 1 && tokens.front().first == 5) {
        if (tokens.size() >= 3 && tokentable.getStr(tokens[1]) == "(" && tokentable.getStr(tokens[2]) == ")") i = 3;
        else i = 1;
        return true;
    }
    return false;
}