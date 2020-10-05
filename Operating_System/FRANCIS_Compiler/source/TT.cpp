#include "TT.h"

int TokenTable::Menu() {
    fstream fin;
    string name;
    bool read_success;
    do {
        read_success = true;
        cout << "Please give the tokentable name (XXX[1-2].table), or [D]efalut (table[1-2].table), or [Q]uit : ";
        cin >> name;
        if (name == "Q") return 1;
        if (name == "D") name = "table";

        table[0].push_back(" ");
        table[0].push_back("\t");
        table[0].push_back("\n");
        table[0].push_back("\r");
        table[0].push_back("\0");
        table[0].resize(100);
        for (int i = 1; i <= 2; i++) {
            string filename = name + to_string(i) + ".table";
            fin.open(filename, ios::in);
            if (!fin) {
                cout << "File \"" << filename << "\" does not exist!" << endl;
                read_success = false;
            }
            else {
                readTable(i, fin);
                cout << "Load \"" << filename << "\": success" << endl;
            }
            fin.close();
        }
        table[1][9] = "↑";
        for (int i = 3; i <= 5; i++) {
            table[i].resize(100);
            table[i][0] = " ";
            string filename = name + to_string(i) + ".table";
            cout << "Create \"" << filename << "\": success" << endl;
        }
        id_table.resize(100);
        id_table[0].id = " ";
        cout << "Create \"ID table\": success" << endl;
    } while (!read_success);
    return 0;
}

bool TokenTable::readTable(int i, fstream& fin) {
    string token;
    table[i].push_back("");
    while (fin.peek() != EOF) {
        fin >> token;
        table[i].push_back(token);
    }
    return true;
}

void TokenTable::printTable_12345() {
    for (int i = 1; i <= 5; i++) {
        cout << "Table" << i << endl;
        for (int j = 0; j < table[i].size(); j++) {
            cout << '[' << j << "] " << table[i][j] << endl;
        }
    }
}

void TokenTable::printTable_ID() {
    cout << "Table ID" << endl;
    for (int j = 0; j < id_table.size(); j++) {
        cout << '[' << j << "] " << '\t' << id_table[j].id 
            << '\t' << id_table[j].scope << '\t' << id_table[j].type
            << '\t' << id_table[j].pointer << endl;
    }
}

void TokenTable::clear() {
    for (int i = 0; i <= 5; i++) {
        table[i].clear();
    }
    id_table.clear();
}

vector<pair<int, int>> TokenTable::getToken(string line) {
    line += '\n';
    vector<pair<int, int>> tokens;
    string temp = "";
    int type = 0;
    int mode = 0;
    for (int i = 0; i < line.length(); i++) {
        char cur = line[i];
        pair<int, int> index = getCharIndex(cur);
        switch (mode) {
        case 0: // 讀Token開頭
            if (index.first == 0 || cur < 0 && index.first == -1); // isWhiteSpace
            else {
                if (index.first == 1) {
                    if (index.second != 14) {
                        tokens.push_back(index); // isDelimiter
                        if (index.second == 1) i = line.length(); // isComment
                    }
                    else { // 讀real數字
                        temp += cur;
                        type = 4;
                        mode = 2;
                    }
                }
                else {
                    if (isdigit(cur)) { // 讀數字
                        temp += cur;
                        type = 3;
                        mode = 2;
                    }
                    else { // 讀指令/標籤
                        temp += cur;
                        type = 5;
                        mode = 1;
                    }
                }
            }
            break;
        case 1: // 讀指令/標籤
            if (index.first == 0 || index.first == 1) { // isWhiteSpace or isDelimiter
                pair<int, int> index_temp = getTokenIndex(temp, type);
                tokens.push_back(index_temp);
                if (index.first == 1) tokens.push_back(index);
                temp = "";
                mode = 0;
            }
            else temp += cur;
            break;
        case 2: // 讀數字
            if (type == 3 && index.first == 1 && index.second == 14) { // '.'
                temp += cur;
                type = 4;
                mode = 2;
            }
            else {
                if (index.first == 0 || index.first == 1) { // isWhiteSpace or isDelimiter
                    pair<int, int> index_temp = getTokenIndex(temp, type);
                    tokens.push_back(index_temp);
                    if (index.first == 1) tokens.push_back(index);
                    temp = "";
                    mode = 0;
                }
                else temp += cur;
            }
            break;
        }
    }
    return tokens;
}

pair<int, int> TokenTable::getCharIndex(char c) {
    string str(1, c);
    if(c == -30) return make_pair(1, 9);
    for (int j = 0; j < table[0].size(); j++) {
        if (str == table[0][j]) return make_pair(0, j);
    }
    for (int j = 0; j < table[1].size(); j++) {
        if (str == table[1][j]) return make_pair(1, j);
    }
    return make_pair(-1, -1);
}

pair<int, int> TokenTable::getTokenIndex(string str, int type) {
    pair<int, int> index = findToken(str, type);
    if (index.first != -1) return index;
    else return insertToken(str, type);
}

pair<int, int> TokenTable::findToken(string& str, int& type) {
    string str_up = str;
    transform(str_up.begin(), str_up.end(), str_up.begin(), toupper);
    for (int i = 1; i <= 5; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            if (i == 2 && str_up == table[i][j]) return make_pair(i, j);
            if (i != 2 && str == table[i][j]) return make_pair(i, j);
        }
    }
    return make_pair(-1, -1);
}

pair<int, int> TokenTable::insertToken(string& str, int& type) {
    int hash_index = hash(str);
    for (int i = 0; i < 100; i++) {
        int index = (hash_index + i) % 100;
        if (table[type][index] == "") {
            table[type][index] = str;
            return make_pair(type, index);
        }
    }
    cout << "Can't insert token, Table is full" << endl;
    /*
    table[type].push_back(str);
    return make_pair(type, table[type].size()-1);
    */
}

int TokenTable::hash(string& str) {
    int sum = 0;
    for (char c : str) {
        sum += c;
    }
    return sum % 100;
}

string TokenTable::getStr(pair<int, int> index) const {
    return table[index.first][index.second];
}

pair<int, int> TokenTable::insertID(string a, int b, int c, int d) {
    pair<int, int> index = findID(a, b);
    if (index.first != -1) return make_pair(-1, -1);
    else {
        int hash_index = hash(a);
        for (int i = 0; i < 100; i++) {
            int index = (hash_index + i) % 100;
            if (id_table[index].id == "") {
                id_table[index].id = a;
                id_table[index].scope = b;
                id_table[index].type = c;
                id_table[index].pointer = d;
                return make_pair(5, index);
            }
        }
        cout << "Can't insert token, Table is full" << endl;
    }
    return make_pair(-1, -1);
}

pair<int, int> TokenTable::findID(string a, int b) {
    for (int i = 0; i < 100; i++) {
        if (id_table[i].id == a && id_table[i].scope == b) {
            return make_pair(5, i);
        }
    }
    return make_pair(-1, -1);
}

string TokenTable::getID(int index) {
    return id_table[index].id;
}

int TokenTable::updateID(string a, int b, int d) {
    for (int i = 0; i < 100; i++) {
        if (id_table[i].id == a && id_table[i].scope == b) {
            id_table[i].pointer = d;
            return 0;
        }
    }
    return 1;
}

int TokenTable::getPtr(string a, int b) {
    for (int i = 0; i < 100; i++) {
        if (id_table[i].id == a && id_table[i].scope == b) {
             return id_table[i].pointer;
        }
    }
    return -1;
}

int TokenTable::getPtr(int index) {
    return id_table[index].pointer;
}