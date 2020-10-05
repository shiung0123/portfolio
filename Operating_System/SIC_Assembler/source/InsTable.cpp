#include "InsTable.h"

int InsTable::Menu() {
    fstream fin;
    string name;
    do {
        read_success = true;
        cout << "Please give the instable name (XXX.csv), or [D]efalut (SIC_INS.csv), or [Q]uit : ";
        cin >> name;
        if (name == "Q") return 0;
        if (name == "D") name = "SIC_INS";
        string filename = name + ".csv";
        fin.open(filename, ios::in);
        
        if (!fin) {
            cout << "File \"" << filename << "\" does not exist!" << endl;
            read_success = false;
        }
        else {
            readTable(fin);
            cout << "Load \"" << filename << "\": success" << endl;
        }
        fin.close();
    } while (!read_success);
    return 0;
}

bool InsTable::readTable(fstream& fin) {
    string line;
    while (fin.peek() != EOF) {
        getline(fin, line);
        vector<string> tokens = split(line);
        ins_data.push_back(INS_Data(tokens[0], strhex_int(tokens[1]), strint_int(tokens[2])));
    }
    return true;
}

vector<string> InsTable::split(string str) {
    string temp = "";
    vector<string> tokens;
    for (char c : str) {
        if (c != ',') temp += c;
        else tokens.push_back(temp), temp = "";
    }
    tokens.push_back(temp);
    return tokens;
}

int InsTable::getType(int index) {
    return ins_data[index].type;
}

int InsTable::getOp(int index) {
    return ins_data[index].op_code/4;
}

int InsTable::getReg(int index) {
    return ins_data[60+index].op_code;
}
