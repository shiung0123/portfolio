#include "LA.h"

vector<pair<int, int>> getToken(string line, TokenTable& table) {
    line += '\n';
    vector<pair<int, int>> tokens;
    string temp = "";
    char peek;
    int type = 0;
    int mode = 0;
    for (int i = 0; i < line.length(); i++) {
        char cur = line[i];
        pair<int, int> index = table.findCharIndex(cur);
        switch (mode) {
        case 0: // 讀Token開頭
            if (index.first == 0); // isWhiteSpace
            else {
                if (index.first == 4) {
                    tokens.push_back(index); // isDelimiter
                    if (index.second == 10) i = line.length(); // isComment
                }
                else {
                    if (isdigit(cur) ||
                        (cur == 'X' || cur == 'x' || cur == 'C' || cur == 'c') && (i + 1 < line.length()) && line[i + 1] == '\'') {
                        if (isdigit(cur)) {
                            temp += cur;
                            type = 1;
                            mode = 1;
                        }
                        else {
                            index = table.getTokenIndex("\'", 0);
                            tokens.push_back(index);
                            i++;
                            if (cur == 'X' || cur == 'x') type = 1;
                            else type = 2; // if (cur == 'C')
                            mode = 2;
                        }

                    }
                    else { // 讀指令/標籤
                        temp += cur;
                        type = 0;
                        mode = 1;
                    }
                }
            }
            break;
        case 1: // 讀一般Token
            if (index.first == 0 || index.first == 4) { // isWhiteSpace or isDelimiter
                pair<int, int> index_temp = table.getTokenIndex(temp, type);
                tokens.push_back(index_temp);
                if (index.first == 4) tokens.push_back(index);
                temp = "";
                mode = 0;
            }
            else temp += cur;
            break;
        case 2: // 讀''內Token
            if (cur == '\'') {
                if (temp != "") {
                    pair<int, int> index_temp = table.getTokenIndex(temp, type);
                    tokens.push_back(index_temp);
                }
                tokens.push_back(index);
                temp = "";
                mode = 0;
            }
            else temp += cur;
            break;
        }
    }
    return tokens;
}