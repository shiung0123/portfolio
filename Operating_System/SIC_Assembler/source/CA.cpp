#include "CA.h"

int CrossAssembler::Menu() {
    data.clear();
    symtable.clear();
    littable.clear();
    tokentable.Menu();
    while (!tokentable.read_success) {
        tokentable.Menu();
        cout << "Please give the tokentable first!";
    }
    instable.Menu();
    while (!instable.read_success) {
        instable.Menu();
        cout << "Please give the tokentable first!";
    }
    
    fstream fin;
    fstream fout;
    string name;
    int t;
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
    while (true) {
        cout << "Please give the language that you want to translate, [0]SIC or [1]SICXE : ";
        cin >> t;
        if (t == 0 || t == 1) break;
    }
    Pass1(fin, fout, t);
    Pass2(t);
    
    print(fout);

    fin.close();
    fout.close();
    
    return 0;
}

void CrossAssembler::print(fstream& fout) {
    fout << "Line\tLocation\tSource code\t\t\tObject code" << endl;
    for (INS ins : data) {
        fout << ins.line << "\t\t" << fillzero(int_strhex(ins.loc), 4) << '\t'
            << ins.str << "\t\t\t" << ins.code << endl;
    }
}

int CrossAssembler::Pass1(fstream& fin, fstream& fout, bool t) {
    int no = 5;
    while (fin.peek() != EOF) {
        string line;
        getline(fin, line);
        int address;
        int base;
        vector<pair<int, int>> tokens = getToken(line, tokentable);
        if (tokens.size() != 0) {
            if (tokens[tokens.size() - 1].first == 4 && tokens[tokens.size() - 1].second == 10) {
                tokens.pop_back();
            }
            string temp;
            if(t == false) temp = checkSyntax_SIC(tokens, tokentable, instable, symtable, littable, address, base);
            else temp = checkSyntax_SICXE(tokens, tokentable, instable, symtable, littable, address, base);

            if (tokens.size() == 0) base = -1;
            else if (tokens[0].first == 5) {
                if (tokens.size() == 1) {
                    temp = "X";
                    base = -1;
                }
                if (!symtable.insert(tokentable.getStr(tokens[0]), address)) {
                    temp = "X";
                    base = -1;
                }
            }
            data.push_back(INS(no, address, line, tokens, temp, base));
            no += 5;
        }
    }
    vector<pair<int, string>> lit = littable.getliteral();
    for (pair<int, string> i : lit) {
        vector<pair<int, int>> tokens;
        data.push_back(INS(no, i.first, "", tokens, i.second, -1));
        no += 5;
    }
    return 0;
}

string CrossAssembler::disp(int pc, int base, int loc, char c) {
    string temp;
    int disp = loc - pc;
    if (-2048 <= disp && disp < 2048) {
        temp = "01";
    }
    else {
        disp = loc - base;
        temp = "10";
    }
    temp += c;
    temp += int_strb(disp, 12);
    return temp;
}

int CrossAssembler::Pass2(bool t) {
    if (t == false) generate_SIC();
    else generate_SICXE();
    return 0;
}

void CrossAssembler::generate_SICXE() {
    int base = 0;
    for (int ins = 0; ins < data.size(); ins++) {
        string code = "";
        int i = 0;
        bool trans = true;
        if (data[ins].base != -1) {
            if (data[ins].tokens[i].first == 5) i++;
            if (data[ins].tokens[i].first == 4 && data[ins].tokens[i].second == 2) i++;
            code += int_strb(instable.getOp(data[ins].tokens[i].second), 6);
            if (data[ins].code == "00") {
                base = strint_int(tokentable.getStr(data[ins].tokens[i + 1]));
            }
            else if (data[ins].code == "01") {
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 1]));
                if (num != -1) base = num;
            }
            else if (data[ins].code == "1") {
                code += "00";
            }
            else if (data[ins].code[0] == '2') {
                code += "00";
                if (data[ins].code[1] == '1') {
                    code += int_strb(strint_int(tokentable.getStr(data[ins].tokens[i + 1])), 4);
                    code += "0000";
                }
                else if (data[ins].code[1] == '2') {
                    code += int_strb(instable.getReg(data[ins].tokens[i + 1].second), 4);
                    code += "0000";
                }
                else if (data[ins].code[1] == '3') {
                    code += int_strb(instable.getReg(data[ins].tokens[i + 1].second), 4);
                    code += int_strb(instable.getReg(data[ins].tokens[i + 3].second), 4);
                }
                else if (data[ins].code[1] == '4') {
                    code += int_strb(instable.getReg(data[ins].tokens[i + 1].second), 4);
                    code += int_strb(strint_int(tokentable.getStr(data[ins].tokens[i + 3]))-1, 4);
                }
            }
            else if (data[ins].code == "311") {
                code += "010";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 2]));
                if (num == -1) trans = false;
                code += disp(data[ins].loc + 3, base, num, '0');
            }
            else if (data[ins].code == "312") {
                code += "010000";
                code += int_strb(strint_int(tokentable.getStr(data[ins].tokens[i + 2])), 12);
            }
            else if (data[ins].code == "321") {
                code += "110";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 1]));
                if (num == -1) trans = false;
                code += disp(data[ins].loc + 3, base, num, '0');
            }
            else if (data[ins].code == "322") {
                code += "110000";
                code += int_strb(strint_int(tokentable.getStr(data[ins].tokens[i + 1])), 12);
            }
            else if (data[ins].code == "33") {
                code += "100";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 2]));
                if (num == -1) trans = false;
                code += disp(data[ins].loc + 3, base, num, '0');
            }
            else if (data[ins].code == "34") {
                code += "111";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 1]));
                if (num == -1) trans = false;
                code += disp(data[ins].loc + 3, base, num, '0');
            }
            else if (data[ins].code == "351") {
                code += "110";
                code += disp(data[ins].loc + 3, base, littable.get(tokentable.getStr(data[ins].tokens[i + 3]), 0), '0');
            }
            else if (data[ins].code == "352") {
                code += "110";
                code += disp(data[ins].loc + 3, base, littable.get(tokentable.getStr(data[ins].tokens[i + 3]), 1), '0');
            }
            else if (data[ins].code == "411") {
                code += "010001";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 2]));
                if (num == -1) trans = false;
                code += int_strb(num, 20);
            }
            else if (data[ins].code == "412") {
                code += "010001";
                code += int_strb(strint_int(tokentable.getStr(data[ins].tokens[i + 2])), 20);
            }
            else if (data[ins].code == "421") {
                code += "110001";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 1]));
                if (num == -1) trans = false;
                code += int_strb(num, 20);
            }
            else if (data[ins].code == "422") {
                code += "110001";
                code += int_strb(strint_int(tokentable.getStr(data[ins].tokens[i + 1])), 20);
            }
            else if (data[ins].code == "43") {
                code += "100001";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 2]));
                if (num == -1) trans = false;
                code += int_strb(num, 20);
            }
            else if (data[ins].code == "44") {
                code += "111001";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 1]));
                if (num == -1) trans = false;
                code += int_strb(num, 20);
            }
            else if (data[ins].code == "451") {
                code += "110001";
                code += int_strb(littable.get(tokentable.getStr(data[ins].tokens[i + 3]), 0), 20);
            }
            else if (data[ins].code == "452") {
                code += "110001";
                code += int_strb(littable.get(tokentable.getStr(data[ins].tokens[i + 3]), 1), 20);
            }

            if (trans) data[ins].code = b_strhex(code);
            else data[ins].code = "X";
        }
    }
}

void CrossAssembler::generate_SIC() {
    int base = 0;
    for (int ins = 0; ins < data.size(); ins++) {
        string code = "";
        int i = 0;
        bool trans = true;
        if (data[ins].base != -1) {
            if (data[ins].tokens[i].first == 5) i++;
            code += int_strb(instable.getOp(data[ins].tokens[i].second), 6);
            code += "00";
            if (data[ins].code == "00") {
                base = strint_int(tokentable.getStr(data[ins].tokens[i + 1]));
            }
            else if (data[ins].code == "01") {
                base = symtable.get(tokentable.getStr(data[ins].tokens[i + 1]));
            }
            else if (data[ins].code == "321") {
                code += "0";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 1]));
                if (num == -1) trans = false;
                code += int_strb(num, 15);
            }
            else if (data[ins].code == "34") {
                code += "1";
                int num = symtable.get(tokentable.getStr(data[ins].tokens[i + 1]));
                if (num == -1) trans = false;
                code += int_strb(num, 15);
            }

            if (trans) data[ins].code = b_strhex(code);
            else data[ins].code = "X";
        }
    }
}