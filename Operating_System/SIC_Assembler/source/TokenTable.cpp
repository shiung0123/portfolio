#include "TokenTable.h"

int TokenTable::Menu() {
    fstream fin;
    string name;
    do {
        read_success = true;
        cout << "Please give the tokentable name (XXX[1-4].table), or [D]efalut (table[1-4].table), or [Q]uit : ";
        cin >> name;
        if (name == "Q") return 0;
        if (name == "D") name = "table";
        table[0].push_back(" ");
        table[0].push_back("\t");
        table[0].push_back("\n");
        table[0].push_back("\r");
        table[0].push_back("\0");
        for (int i = 1; i <= 4; i++) {
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
        for (int i = 5; i <= 7; i++) {
            table[i].clear();
            table[i].resize(100);
            string filename = name + to_string(i) + ".table";
            cout << "Create \"" << filename << "\": success" << endl;
        }
    } while (!read_success);
    return 0;
}

bool TokenTable::readTable(int i, fstream& fin) {
    string token;
    table[i].push_back("");
    while (fin.peek() != EOF) {
        fin >> token;
        transform(token.begin(), token.end(), token.begin(), toupper);
        table[i].push_back(token);
    }
    return true;
}

// type0:table1~5, type1:table6(int), type2:table7(str)
pair<int, int> TokenTable::findToken(string& str, int& type) {
    string str_up = str;
    transform(str_up.begin(), str_up.end(), str_up.begin(), toupper);
    switch (type) {
    case 0:
        for (int i = 1; i <= 5; i++) {
            for (int j = 0; j < table[i].size(); j++) {
                if (i <= 3 && str_up == table[i][j]) return make_pair(i, j);
                if (i > 3 && str == table[i][j]) return make_pair(i, j);
            }
        }
        break;
    case 1:
        for (int j = 0; j < table[6].size(); j++) {
            if (str_up == table[6][j]) return make_pair(6, j);
        }
        break;
    case 2:
        for (int j = 0; j < table[7].size(); j++) {
            if (str == table[7][j]) return make_pair(7, j);
        }
        break;
    }
    return make_pair(-1, -1);
}

int TokenTable::hash(string& str) {
    int sum = 0;
    for (char c : str) {
        sum += c;
    }
    return sum % 100;
}

pair<int, int> TokenTable::insertToken(string& str, int& type) {
    if (type <= 2) {
        if(type == 1) transform(str.begin(), str.end(), str.begin(), toupper);
        int hash_index = hash(str);
        for (int i = 0; i < 100; i++) {
            int index = (hash_index + i) % 100;
            if (table[type + 5][index] == "") {
                table[type + 5][index] = str;
                return make_pair(type + 5, index);
            }
        }
        cout << "Can't insert token, Table is full" << endl;
    }
    return make_pair(-1, -1);
}

pair<int, int> TokenTable::getTokenIndex(string str, int type) {
    pair<int, int> index = findToken(str, type);
    if (index.first != -1) return index;
    else return insertToken(str, type);
}

void TokenTable::printTable() {
    for (int i = 1; i <= 7; i++) {
        cout << "Table" << i << endl;
        for (int j = 0; j < table[i].size(); j++) {
            cout << '[' << j << "] " << table[i][j] << endl;
        }
    }
}

pair<int, int> TokenTable::findCharIndex(char c) {
    string str(1, c);
    for (int j = 0; j < table[0].size(); j++) {
        if (str == table[0][j]) return make_pair(0, j);
    } 
    for (int j = 0; j < table[4].size(); j++) {
        if (str == table[4][j]) return make_pair(4, j);
    }
    return make_pair(-1, -1);
}

string TokenTable::getStr(pair<int, int> index) {
    return table[index.first][index.second];
}
