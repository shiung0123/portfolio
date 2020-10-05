#include "TokenFile.h"

int TokenFile::Menu(TokenTable& table) {
    if (!table.read_success) cout << "Please give the table first!" << endl;
    else {
        fstream fin;
        fstream fout;
        string name;
        while (true) {
            cout << "Please give the file name (XXX_input.txt), or [D]efalut (MY_input.txt), or [Q]uit : ";
            cin >> name;
            if (name == "Q") return 0;
            if (name == "D") name = "MY";
            string filename = name + "_input.txt";
            fin.open(filename, ios::in);
            if (!fin) cout << "File \"" << filename << "\" does not exist!" << endl;
            else {
                fout.open(name + "_output.txt", ios::out | ios::trunc);
                break;
            }
        }
        getToken(fin, fout, table);
        fin.close();
        fout.close();
    }
    return 0;
}

void TokenFile::printIndex(fstream& fout, const pair<int, int>& index) {
    fout << '(' << index.first << ',' << index.second << ')';
}

int TokenFile::getToken(fstream& fin, fstream& fout, TokenTable& table) {
    while (fin.peek() != EOF) {
        string line;
        getline(fin, line);
        bool isempty = true;
        fout << line << endl;
        line += '\n';
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
                    isempty = false;
                    if (index.first == 4) {
                        printIndex(fout, index); // isDelimiter
                        if (index.second == 10) i = line.length(); // isComment
                    }
                    else {
                        if (isdigit(cur) ||
                            (cur == 'X' || cur == 'C') && (i + 1 < line.length()) && line[i + 1] == '\'') {
                            if (isdigit(cur)) {
                                temp += cur;
                                type = 1;
                                mode = 1;
                            }
                            else {
                                index = table.getTokenIndex("\'", 0);
                                printIndex(fout, index);
                                i++;
                                if (cur == 'X') type = 1;
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
                    printIndex(fout, index_temp);
                    if (index.first == 4) printIndex(fout, index);
                    temp = "";
                    mode = 0;
                }
                else temp += cur;
                break;
            case 2: // 讀''內Token
                if (cur == '\'') {
                    if (temp != "") {
                        pair<int, int> index_temp = table.getTokenIndex(temp, type);
                        printIndex(fout, index_temp);
                    }
                    printIndex(fout, index);
                    temp = "";
                    mode = 0;
                }
                else temp += cur;
                break;
            }  
        }
        if (!isempty) fout << endl;
    }
    return 0;
}