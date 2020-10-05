#include "FC.h"

int no;
int scope;
int offset;
# define NONE 0
# define ARRAY 1
# define BOOLEAN 2
# define CHARACTER 3
# define INTEGER 4
# define LABEL 5
# define REAL 6

int FCompiler::Entry() {
    if (Menu()) return 0;
    if (tokentable.Menu()) return 0;

    Pass1();
    Pass2();

    print();
    tokentable.printTable_ID();
    printTable7();

    tokentable.clear();
    data.clear();
    table6.clear();
    table7.clear();
    fin.close();
    fout.close();
    return 0;
}

int FCompiler::Menu() {
    string name;
    while (true) {
        cout << "Please give the file name (XXX_input.txt), or [D]efalut (MY_input.txt), or [Q]uit : ";
        cin >> name;
        if (name == "Q") return 1;
        if (name == "D") name = "MY";
        string filename = name + "_input.txt";
        fin.open(filename, ios::in);
        if (!fin) cout << "File \"" << filename << "\" does not exist!" << endl;
        else {
            fout.open(name + "_output.txt", ios::out | ios::trunc);
            break;
        }
    }
    return 0;
}

void FCompiler::printTable7() {
    cout << "Table7" << endl;
    for (int i : table7) cout << i << endl;
}

void FCompiler::print() {
    /*
    fout << "SA Check\tInput Code\tTokens\n" << endl;
    for (DATA i : data) {
        fout << i.check << "\t\t" << i.code << '\t' << i.tokens << endl;
    }
    */
    fout << "\nLine\tIntermediate Code\t\t\t\t\tInput Code\n" << endl;
    for (INS i : table6) {
        fout << i.no << "\t\t";
        printInter(i.inter);
        fout << "\t\t" << i.str << endl;
    }
}

void FCompiler::printInter(vector<pair<int, int>> tokens) {
    fout << '(';
    switch (tokens.size()) {
    case 0:
        fout << "\t\t" << ',' 
            << "\t\t" << ',' 
            << "\t\t" << ',' 
            << "\t\t";
        break;
    case 1:
        fout << '(' << tokens[0].first << ", " << tokens[0].second << ')' << ','
            << "\t\t" << ',' 
            << "\t\t" << ',' 
            << "\t\t";
        break;
    case 2:
        fout << '(' << tokens[0].first << ", " << tokens[0].second << ')' << ','
            << "\t\t" << ',' 
            << "\t\t" << ',' 
            << '(' << tokens[1].first << ", " << tokens[1].second << ')';
        break;
    case 3:
        fout << '(' << tokens[0].first << ", " << tokens[0].second << ')' << ','
            << '(' << tokens[1].first << ", " << tokens[1].second << ')' << ','
            << "\t\t" << ','
            << '(' << tokens[2].first << ", " << tokens[2].second << ')';
        break;
    case 4:
        fout << '(' << tokens[0].first << ", " << tokens[0].second << ')' << ','
            << '(' << tokens[1].first << ", " << tokens[1].second << ')' << ','
            << '(' << tokens[2].first << ", " << tokens[2].second << ')' << ','
            << '(' << tokens[3].first << ", " << tokens[3].second << ')';
        break;
    }
    fout << ')';
}

std::ostream& operator << (std::ostream& out, const vector<pair<int, int>>& vec_index) {
    out << '(';
    for (int i = 0; i < vec_index.size(); i++) {
        out << '(' << vec_index[i].first << ", " << vec_index[i].second << ')';
        if (i + 1 != vec_index.size()) out << ", ";
    }
    out << ')';
    return out;
}

int FCompiler::Pass1() {
    enp = 0;
    ens = 0;
    while (fin.peek() != EOF) {
        string line;
        getline(fin, line);
        vector<pair<int, int>> tokens = tokentable.getToken(line);
        if (tokens.size() != 0) {
            bool check = checkSyntax(tokens, tokentable);
            data.push_back(DATA(line, tokens, check));
        }
    }
    return 0;
}

int FCompiler::Pass2() {
    vector<pair<int, int>> inter;
    table6.push_back(INS(NONE, inter, "", NONE));
    table7.push_back(0);
    no = 1;
    offset = 0;
    for (DATA i : data) {
        if (i.check) {
            generate(i);
        }
        else {
            table6.push_back(INS(-1, inter, i.code, 0));
        }
    }
    for (int i = 0; i < table6.size(); i++) {
        if (offset != 0 && table6[i].no > 0) {
            table6[i].no += offset;
            for (int j = 0; j < table6[i].inter.size(); j++)
                if (table6[i].inter[j].first == 6) table6[i].inter[j].second += offset;
        }
        if (table6[i].done == 2) {
            pair<int, int> pointer = table6[i].inter.back();
            table6[i].inter.pop_back();
            int address = tokentable.getPtr(pointer.second);
            if (address != 0) {
                table6[i].inter.push_back(make_pair(6, address));
                table6[i].done = 1;
            }
            else {
                cout << "LABEL not exist!" << endl;
                offset--;
                table6[i].no = 0;
                table6[i].inter.clear();
                table6[i].done = 0;
            }
        }
        if (table6[i].done == 3) {
            string sub = tokentable.getStr(table6[i].inter.back());
            table6[i].inter.pop_back();
            pair<int, int> index = tokentable.findID(sub, NONE);
            if (index.first != -1) {
                table6[i].inter.insert(table6[i].inter.begin()+1,index);
                table6[i].done = 1;
            }
            else {
                cout << "The SUB undefine!" << endl;
                offset--;
                table6[i].no = 0;
                table6[i].inter.clear();
                table6[i].done = 0;
            }
        }
    }
    return 0;
}

void FCompiler::generate(DATA d) {
    int i = 0;
    string line = d.code;
    line.pop_back();
    string pre_label;
    if (d.tokens[i].first == 5 && d.tokens.size() > 1 && d.tokens[i + 1].first == 2) {
        pre_label = tokentable.getStr(d.tokens[i]);
        if (tokentable.updateID(pre_label, scope, no)) cout << "The Label is undefine!" << endl;
        i++;
    }

    if (d.tokens.size() >= 2) {
        vector<pair<int, int>> token(d.tokens.begin() + i + 1, d.tokens.end() - 1);
        if (tokentable.getStr(d.tokens[i]) == "PROGRAM") {
            generate_PROGRAM(token);
        }
        else if (tokentable.getStr(d.tokens[i]) == "SUBROUTINE") {
            generate_SUBROUTINE(token);
        }
        else if (tokentable.getStr(d.tokens[i]) == "VARIABLE") {
            generate_VARIABLE(token);
        }
        else if (tokentable.getStr(d.tokens[i]) == "DIMENSION") {
            generate_DIMENSION(token);
        }
        else if (tokentable.getStr(d.tokens[i]) == "LABEL") {
            generate_LABEL(token);
        }
        else if (tokentable.getStr(d.tokens[i]) == "GTO") {
            generate_GTO(token, line);
        }
        else if (tokentable.getStr(d.tokens[i]) == "CALL") {
            generate_CALL(token, line);
        }
        else if (tokentable.getStr(d.tokens[i]) == "INPUT") {
            generate_INPUT(token, line);
        }
        else if (tokentable.getStr(d.tokens[i]) == "OUTPUT") {
            generate_OUTPUT(token, line);
        }
        else if (tokentable.getStr(d.tokens[i]) == "IF") {
            generate_IF(token, line);
        }
        else if (tokentable.getStr(d.tokens[i]) == "ENP") {
            vector<pair<int, int>> inter;
            inter.push_back(make_pair(2, 6));
            table6.push_back(INS(no, inter, "ENP", 1));
            no++;
        }
        else if (tokentable.getStr(d.tokens[i]) == "ENS") {
            vector<pair<int, int>> inter;
            inter.push_back(make_pair(2, 7));
            table6.push_back(INS(no, inter, "ENS", 1));
            no++;
        }
        else {
            if (d.tokens[i].first == 5 && d.tokens[i + 1].first == 5) {
                pre_label = tokentable.getStr(d.tokens[i]);
                if (tokentable.updateID(pre_label, scope, no)) cout << "The Label is undefine!" << endl;
                i++;
            }
            vector<pair<int, int>> token_a(d.tokens.begin() + i, d.tokens.end() - 1);
            generate_ASSIGN(token_a, line);
        }
    }
}

void FCompiler::generate_PROGRAM(vector<pair<int, int>> tokens) {
    string str = tokentable.getStr(tokens[0]);
    pair<int, int> index = tokentable.insertID(str, NONE, NONE, no);
    if (index.first != -1) {
        scope = index.second;
    }
    else {
        cout << "ID already exist!" << endl;
        vector<pair<int, int>> inter;
        table6.push_back(INS(NONE, inter, str, 0));
    }
}

void FCompiler::generate_SUBROUTINE(vector<pair<int, int>> tokens) {
    string str = tokentable.getStr(tokens[0]);
    pair<int, int> index = tokentable.insertID(str, NONE, NONE, no);
    if (index.first != -1) {
        scope = index.second;
    }
    else {
        cout << "ID already exist!" << endl;
        vector<pair<int, int>> inter;
        table6.push_back(INS(NONE, inter, str, 0));
    }

    vector<pair<int, int>> token(tokens.begin() + 2, tokens.end() - 1);
    vector<vector<pair<int, int>>> element = split(token, ",");
    int ele_type = NONE;
    for (vector<pair<int, int>> e : element) {
        int i = 0;
        if (e[i].first == 2) {
            if (e[i].second == 2) ele_type = BOOLEAN;
            else if (e[i].second == 14) ele_type = INTEGER;
            else if (e[i].second == 22) ele_type = REAL;
            i += 2;
        }

        str = tokentable.getStr(e[i]);
        vector<pair<int, int>> inter;
        if (i + 1 == e.size()) {
            pair<int, int> index = tokentable.insertID(str, scope, ele_type, NONE);
            if (index.first != -1) {
                inter.push_back(index);
                table6.push_back(INS(no, inter, str, 1));
                no++;
            }
            else {
                cout << "ID already exist!" << endl;
                table6.push_back(INS(NONE, inter, str, 0));
            }
        }
        else {
            int table7_index = table7.size();
            pair<int, int> index = tokentable.insertID(str, scope, ARRAY, table7_index);
            if (index.first != -1) {
                table7.push_back(ele_type);
                table7.push_back(0);
                inter.push_back(index);
                table6.push_back(INS(no, inter, str, 1));
                no++;
            }
            else {
                cout << "ID already exist!" << endl;
                table6.push_back(INS(NONE, inter, str, 0));
            }
        }
    }
}

void FCompiler::generate_VARIABLE(vector<pair<int, int>> tokens) {
    int type = NONE;
    if (tokens[0].second == 2) type = BOOLEAN;
    else if (tokens[0].second == 14) type = INTEGER;
    else if (tokens[0].second == 22) type = REAL;

    for (int i = 2; i < tokens.size(); i += 2) {
        string str = tokentable.getStr(tokens[i]);
        vector<pair<int, int>> inter;
        pair<int, int> index = tokentable.insertID(str, scope, type, NONE);
        if (index.first != -1) {
            inter.push_back(index);
            table6.push_back(INS(no, inter, str, 1));
            no++;
        }
        else {
            cout << "ID already exist!" << endl;
            table6.push_back(INS(NONE, inter, str, 0));
        }
    }
}

void FCompiler::generate_DIMENSION(vector<pair<int, int>> tokens) {
    int ele_type = NONE;
    if (tokens[0].second == 2) ele_type = BOOLEAN;
    else if (tokens[0].second == 14) ele_type = INTEGER;
    else if (tokens[0].second == 22) ele_type = REAL;

    int i = 2;
    while (i < tokens.size()) {
        string str = tokentable.getStr(tokens[i]);
        vector<pair<int, int>> inter;

        int j = i;
        for (; j < tokens.size(); j++) {
            if (tokentable.getStr(tokens[j]) == ")") break;
        }
        vector<pair<int, int>> token(tokens.begin() + i + 2, tokens.begin() + j);
        vector<vector<pair<int, int>>> element = split(token, ",");

        int table7_index = table7.size();

        pair<int, int> index = tokentable.insertID(str, scope, ARRAY, table7_index);
        if (index.first != -1) {
            table7.push_back(ele_type);
            table7.push_back(element.size());
            for (vector<pair<int, int>> e : element) {
                if (e.size() == 1) {
                    table7.push_back(stoi(tokentable.getStr(e[0])));
                }
            }
            inter.push_back(index);
            table6.push_back(INS(no, inter, str, 1));
            no++;
        }
        else {
            cout << "ID already exist!" << endl;
            table6.push_back(INS(NONE, inter, str, 0));
        }

        i = j + 2;
    }

}

vector<vector<pair<int, int>>> FCompiler::split(vector<pair<int, int>> tokens, string c) {
    vector<vector<pair<int, int>>> element;
    vector<pair<int, int>> temp;
    for (pair<int, int> index : tokens) {
        if (tokentable.getStr(index) != c)temp.push_back(index);
        else {
            element.push_back(temp);
            temp.clear();
        }
    }
    element.push_back(temp);
    return element;
}

void FCompiler::generate_LABEL(vector<pair<int, int>> tokens) {
    int type = LABEL;

    for (int i = 0; i < tokens.size(); i += 2) {
        string str = tokentable.getStr(tokens[i]);
        vector<pair<int, int>> inter;
        pair<int, int> index = tokentable.insertID(str, scope, type, NONE);
        if (index.first != -1) {
            inter.push_back(index);
            table6.push_back(INS(no, inter, str, 1));
            no++;
        }
        else {
            cout << "LABEL already exist!" << endl;
            table6.push_back(INS(NONE, inter, str, 0));
        }
    }
}

int FCompiler::generate_GTO(vector<pair<int, int>> tokens, string str) {
    string label = tokentable.getStr(tokens[0]);
    pair<int, int> pointer = tokentable.findID(label, scope);
    vector<pair<int, int>> inter;
    if (pointer.first != -1) {
        inter.push_back(make_pair(2, 11));
        inter.push_back(pointer);
        table6.push_back(INS(no, inter, "GTO " + label, 2));
        no++;
        return 0;
    }
    else {
        cout << "The Label is undefine!" << endl;
        table6.push_back(INS(NONE, inter, str, 0));
        return 1;
    }
}

int FCompiler::generate_CALL(vector<pair<int, int>> tokens, string str) {
    vector<pair<int, int>> inter;
    vector<int> temp;
    bool success = true;
    int table7_index = table7.size();

    vector<pair<int, int>> token(tokens.begin() + 2, tokens.end() - 1);
    vector<vector<pair<int, int>>> element = split(token, ",");
    temp.push_back(element.size());
    for (vector<pair<int, int>> e : element) {
        if (e.size() == 1 && e[0].first == 5) {
            pair<int, int> arg_index = tokentable.findID(tokentable.getStr(e[0]), scope);
            if (arg_index.first != -1) {
                temp.push_back(arg_index.first);
                temp.push_back(arg_index.second);
            }
            else {
                cout << "The ARG undefine!" << endl;
                success = false;
            }
        }
        else {
            if (e.size() == 1) {
                temp.push_back(e[0].first);
                temp.push_back(e[0].second);
            }
            else {
                if (e[0].second == 6) {
                    pair<int, int> arg_index = tokentable.getTokenIndex('-' + tokentable.getStr(e[1]), e[1].first);
                    temp.push_back(arg_index.first);
                    temp.push_back(arg_index.second);
                }
                else {
                    temp.push_back(e[1].first);
                    temp.push_back(e[1].second);
                }
            }
        }
    }

    if (success) {
        for (int e : temp) table7.push_back(e);
        inter.push_back(make_pair(2, 3));
        inter.push_back(make_pair(7, table7_index));
        inter.push_back(tokens[0]);
        table6.push_back(INS(no, inter, str, 3));
        no++;
        return 0;
    }
    else {
        table6.push_back(INS(NONE, inter, str, 0));
        return 1;
    }
}

int FCompiler::generate_INPUT(vector<pair<int, int>> tokens, string line) {
    string var = tokentable.getStr(tokens[0]);
    pair<int, int> index = tokentable.findID(var, scope);
    vector<pair<int, int>> inter;
    if (index.first != -1) {
        inter.push_back(make_pair(2, 13));
        string str = "";
        int temp = 1;
        pair<int, int> element = generate_ELEMENT(tokens, str, temp);
        if (str != "") {
            inter.push_back(element);
            table6.push_back(INS(no, inter, "INPUT " + str, 1));
            no++;
            return 0;
        }
        else {
            table6.push_back(INS(NONE, inter, line, 0));
            return 1;
        }
    }
    else {
        cout << "The VAR undefine!" << endl;
        table6.push_back(INS(NONE, inter, line, 0));
        return 1;
    }
}

int FCompiler::generate_OUTPUT(vector<pair<int, int>> tokens, string line) {
    string var = tokentable.getStr(tokens[0]);
    pair<int, int> index = tokentable.findID(var, scope);
    vector<pair<int, int>> inter;
    if (index.first != -1) {
        inter.push_back(make_pair(2, 13));
        string str = "";
        int temp = 1;
        pair<int, int> element = generate_ELEMENT(tokens, str, temp);
        if (str != "") {
            inter.push_back(element);
            table6.push_back(INS(no, inter, "OUTPUT " + str, 1));
            no++;
            return 0;
        }
        else {
            table6.push_back(INS(NONE, inter, line, 0));
            return 1;
        }
    }
    else {
        cout << "The VAR undefine!" << endl;
        table6.push_back(INS(NONE, inter, line, 0));
        return 1;
    }
}

int FCompiler::generate_ASSIGN(vector<pair<int, int>> tokens, string line) {
    vector<vector<pair<int, int>>> element = split(tokens, "=");
    vector<pair<int, int>> inter;
    int temp = 1;
    string assign_left = "";
    string assign_right = "";
    pair<int, int> sentence_left = generate_ELEMENT(element[0], assign_left, temp);
    pair<int, int> sentence_right = generate_SENTENCE(element[1], assign_right, temp);

    if (assign_left != "" && assign_right != "") {
        inter.push_back(make_pair(1, 4));
        inter.push_back(sentence_right);
        inter.push_back(sentence_left);
        table6.push_back(INS(no, inter, assign_left + " = " + assign_right, 1));
        no++;
        return 0;
    }
    else {
        table6.push_back(INS(NONE, inter, line, 0));
        return 1;
    }
}

void FCompiler::generate_IF(vector<pair<int, int>> tokens, string line) {
    vector<pair<int, int>> inter;
    int temp = 1;
    vector<vector<pair<int, int>>> element = split(tokens, "THEN");
    string str_cond = "";
    pair<int, int> cond_index = generate_SENTENCE(element[0], str_cond, temp);

    if (str_cond != "") {
        int if_t6i = table6.size();
        inter.push_back(make_pair(2, 12));
        inter.push_back(cond_index);
        
        string str = "IF " + str_cond + ' ';
        int then_i_str = line.find("THEN");
        str.append(line.begin() + then_i_str, line.end());
        table6.push_back(INS(no, inter, str, 1));
        no++;

        int pc_true = no;
        int pc_false;
        int bool_then = 0; // 0 -> right
        int bool_else = 0; // 0 -> right
        vector<vector<pair<int, int>>> element2 = split(element[1], "ELSE");

        if (element2.size() == 1) {
            int then_i_str = line.find("THEN");
            string then_str;
            then_str.append(line.begin() + then_i_str + 4, line.end());

            bool_then = generate_stamentI(element2[0], then_str);
            pc_false = no;
            bool_else = 0;

            //table6[if_t6i].str += " THEN GTO Line" + to_string(pc_true);
        }
        else {
            int then_i_str = line.find("THEN");
            int else_i_str = line.find("ELSE");
            string then_str;
            then_str.append(line.begin() + then_i_str + 4, line.begin() + else_i_str);
            string else_str;
            else_str.append(line.begin() + else_i_str + 4, line.end());

            bool_then = generate_stamentI(element2[0], then_str);

            int go_t6i = table6.size();
            vector<pair<int, int>> inter;
            inter.push_back(make_pair(2, 11));
            table6.push_back(INS(no, inter, "GTO", 1));
            no++;

            pc_false = no;
            bool_else = generate_stamentI(element2[1], else_str);
            
            table6[go_t6i].inter.push_back(make_pair(6, no));
            //table6[go_t6i].str += " Line" + to_string(no);

            //table6[if_t6i].str += " THEN GTO Line" + to_string(pc_true) + " ELSE GTO Line" + to_string(pc_false);
        }

        if (str_cond != "" && bool_then == 0 && bool_else == 0) {
            table6[if_t6i].inter.push_back(make_pair(6, pc_true));
            table6[if_t6i].inter.push_back(make_pair(6, pc_false));
        }
        else {
            table6[if_t6i].no = NONE;
            table6[if_t6i].inter.clear();
            table6[if_t6i].done = 0;
        }
    }
    else {
        cout << "The IF Cond error!" << endl;
        table6.push_back(INS(NONE, inter, line, 0));
    }
}

int FCompiler::generate_stamentI(vector<pair<int, int>> tokens, string line) {
    if (tokens.size() == 0) {
        return 0;
    }

    int i = 0;
    vector<pair<int, int>> token(tokens.begin() + i + 1, tokens.end());

    if (tokentable.getStr(tokens[0]) == "GTO") {
        return generate_GTO(token, line);
    }
    else if (tokentable.getStr(tokens[i]) == "CALL") {
        return generate_CALL(token, line);
    }
    else if (tokentable.getStr(tokens[i]) == "INPUT") {
        return generate_INPUT(token, line);
    }
    else if (tokentable.getStr(tokens[i]) == "OUTPUT") {
        return generate_OUTPUT(token, line);
    }
    else {
        vector<pair<int, int>> token_a(tokens.begin() + i, tokens.end());
        return generate_ASSIGN(token_a, line);
    }
}

pair<int, int> FCompiler::generate_ELEMENT(vector<pair<int, int>> tokens, string& line, int& temp) {
    string var = tokentable.getStr(tokens[0]);
    pair<int, int> index = tokentable.findID(var, scope);
    if (tokens.size() == 1) {
        if (tokens[0].first == 0) {
            index = make_pair(-1, -1);
            var = "T";
        }
        line = var;
        if (index.first != -1) return index;
        else return tokens[0];
    }
    else {
        if (index.first == -1) {
            cout << "The ARRAY undefine!" << endl;
            line = "";
            return make_pair(-1, -1);
        }
        int table7_index = tokentable.getPtr(index.second);
        vector<pair<int, int>> token(tokens.begin() + 2, tokens.end() - 1);
        vector<vector<pair<int, int>>> element = split(token, ",");
        if (element.size() == table7[table7_index + 1]) {
            vector<pair<int, int>> inter_assign;

            inter_assign.push_back(make_pair(1, 4));
            inter_assign.push_back(index);
            pair<int, int> offset_index = generate_ARRAYELEMENT(element, table7_index, temp);
            inter_assign.push_back(offset_index);
            pair<int, int> arrayelement_index = make_pair(0, temp++);
            inter_assign.push_back(arrayelement_index);

            string str = intertos(inter_assign);
            table6.push_back(INS(no, inter_assign, str, 1));
            no++;

            line = 'T' + to_string(arrayelement_index.second);
            return arrayelement_index;
        }
        else {
            cout << "The ARRAY index error!" << endl;
            line = "";
            return make_pair(-1, -1);
        }
    }
}

pair<int, int> FCompiler::generate_ARRAYELEMENT(vector<vector<pair<int, int>>> element, int table7_index, int& temp) {
    int arraysize = table7[table7_index + 1];
    vector<pair<int, int>> axis_size;
    for (int i = 0; i < arraysize; i++) axis_size.push_back(tokentable.getTokenIndex(to_string(table7[table7_index + 2 +i]), 3));
    pair<int, int> one_index = tokentable.getTokenIndex(to_string(1), 3);

    if (axis_size.size() == 1) {
        return element[0][0];
    }

    for (int e = element.size() - 1; e > 0; e--) {
        vector<pair<int, int>> inter_add;
        if (e < element.size() - 1) {
            inter_add.push_back(make_pair(1, 5));
            inter_add.push_back(make_pair(0, temp - 1));
        }

        vector<pair<int, int>> inter;
        inter.push_back(make_pair(1, 6));
        inter.push_back(element[e][0]);
        inter.push_back(one_index);
        inter.push_back(make_pair(0, temp++));
        string str = intertos(inter);
        table6.push_back(INS(no, inter, str, 1));
        no++;

        for (int i = 1; 0 <= e - i; i++) {
            pair<int, int> size = axis_size[e - i];

            vector<pair<int, int>> inter;
            inter.push_back(make_pair(1, 7));
            inter.push_back(make_pair(0, temp - 1));
            inter.push_back(size);
            inter.push_back(make_pair(0, temp++));
            string str = intertos(inter);
            table6.push_back(INS(no, inter, str, 1));
            no++;
        }

        if (e < element.size() - 1) {
            inter_add.push_back(make_pair(0, temp - 1));
            inter_add.push_back(make_pair(0, temp++));
            string str = intertos(inter_add);
            table6.push_back(INS(no, inter_add, str, 1));
            no++;
        }
    }

    vector<pair<int, int>> inter;
    inter.push_back(make_pair(1, 5));
    inter.push_back(make_pair(0, temp - 1));
    inter.push_back(element[0][0]);
    inter.push_back(make_pair(0, temp++));
    string str = intertos(inter);
    table6.push_back(INS(no, inter, str, 1));
    no++;

    return make_pair(0, temp - 1);
}

pair<int, int> FCompiler::generate_SENTENCE(vector<pair<int, int>> tokens, string& line, int& temp) {
    int left = 0;
    int right = 0;
    int count = 0;
    bool a = false;
    for (int i = 0; i < tokens.size(); i++) {
        if (tokentable.getStr(tokens[i]) == "(") {
            if (count == 0 && (i == 0 || i > 0 && tokens[i - 1].first != 5)) left = i;
            if (i > 0 && tokens[i - 1].first == 5) a = true;
            count++;
        }
        if (tokentable.getStr(tokens[i]) == ")") {
            count--;
            if (count == 0 && !a) {
                right = i + 1;
                vector<pair<int, int>> token(tokens.begin() + left + 1, tokens.begin() + right - 1);
                string str = "";
                pair<int, int> element = generate_SENTENCE(token, str, temp);
                if (str != "") {
                    tokens.erase(tokens.begin() + left, tokens.begin() + right);
                    tokens.insert(tokens.begin() + left, element);
                    i = left;
                }
                else {
                    line = "";
                    return make_pair(-1, -1);
                }
            }
            if (a) a = false;
        }
    }

    bool sign = false;
    stack<pair<int, int>> Operand;
    stack<pair<int, int>> Operator;
    vector<pair<int, int>> element_temp;
    for (pair<int, int> index : tokens) {
        if (index.first == 2 &&
            (index.second == 1 || (index.second >= 8 && index.second <= 10) || (index.second >= 16 && index.second <= 19)) ||
            index.first == 1 &&
            (index.second >= 5 && index.second <= 9)) {
            if (element_temp.size() == 0) {
                sign = true;
                continue;
            }
            else {
                string str = "";
                pair<int, int> element = generate_ELEMENT(element_temp, str, temp);
                element_temp.clear();
                if (str != "") {
                    Operand.push(element);
                }
                else {
                    line = "";
                    return make_pair(-1, -1);
                }
            }
            // sign: * -1
            if (sign) {
                pair<int, int> n_one_index = tokentable.getTokenIndex(to_string(-1), 3);
                vector<pair<int, int>> inter;
                inter.push_back(make_pair(1, 7));
                inter.push_back(Operand.top());
                Operand.pop();
                inter.push_back(n_one_index);
                inter.push_back(make_pair(0, temp++));

                string str = intertos(inter);
                table6.push_back(INS(no, inter, str, 1));
                no++;

                Operand.push(make_pair(0, temp - 1));
            }

            while (!(Operator.empty() || cmp(index, Operator.top()))) {
                vector<pair<int, int>> inter;

                inter.push_back(Operator.top());
                Operator.pop();
                pair<int, int> op2 = Operand.top();
                Operand.pop();
                inter.push_back(Operand.top());
                Operand.pop();
                inter.push_back(op2);
                inter.push_back(make_pair(0, temp++));

                string str = intertos(inter);
                table6.push_back(INS(no, inter, str, 1));
                no++;

                Operand.push(make_pair(0, temp - 1));
            }
            Operator.push(index);
            
        }
        else {
            element_temp.push_back(index);
        }
    }

    string str = "";
    pair<int, int> element = generate_ELEMENT(element_temp, str, temp);
    if (str != "") {
        Operand.push(element);
    }
    else {
        line = "";
        return make_pair(-1, -1);
    }
    // sign: * -1
    if (sign) {
        pair<int, int> n_one_index = tokentable.getTokenIndex(to_string(-1), 3);
        vector<pair<int, int>> inter;
        inter.push_back(make_pair(1, 7));
        inter.push_back(Operand.top());
        Operand.pop();
        inter.push_back(n_one_index);
        inter.push_back(make_pair(0, temp++));

        string str = intertos(inter);
        table6.push_back(INS(no, inter, str, 1));
        no++;

        Operand.push(make_pair(0, temp - 1));
    }

    while (!Operator.empty()) {
        vector<pair<int, int>> inter;

        inter.push_back(Operator.top());
        Operator.pop();
        pair<int, int> op2 = Operand.top();
        Operand.pop();
        inter.push_back(Operand.top());
        Operand.pop();
        inter.push_back(op2);
        inter.push_back(make_pair(0, temp++));

        string str = intertos(inter);
        table6.push_back(INS(no, inter, str, 1));
        no++;

        Operand.push(make_pair(0, temp - 1));
    }

    if (Operand.top().first == 0) line = 'T' + to_string(Operand.top().second);
    else line = tokentable.getStr(Operand.top());
    return Operand.top();
}

string FCompiler::intertos(vector<pair<int, int>> tokens) {
    string str = "";
    if (tokens.size() == 4) {
        if (tokens[0].first == 1 && tokens[0].second == 4) {
            if (tokens[3].first == 0) str += 'T' + to_string(tokens[3].second);
            else if (tokens[3].first == 5) str += tokentable.getID(tokens[3].second);
            else str += tokentable.getStr(tokens[3]);
            str += " = ";

            if (tokens[1].first == 0) str += 'T' + to_string(tokens[1].second);
            else if (tokens[1].first == 5) str += tokentable.getID(tokens[1].second);
            else str += tokentable.getStr(tokens[1]);
            str += '(';

            if (tokens[2].first == 0) str += 'T' + to_string(tokens[2].second);
            else if (tokens[2].first == 5) str += tokentable.getID(tokens[2].second);
            else str += tokentable.getStr(tokens[2]);

            str += ')';
        }
        else {
            if (tokens[3].first == 0) str += 'T' + to_string(tokens[3].second);
            else if (tokens[3].first == 5) str += tokentable.getID(tokens[3].second);
            else str += tokentable.getStr(tokens[3]);
            str += " = " ;

            if (tokens[1].first == 0) str += 'T' + to_string(tokens[1].second);
            else if (tokens[1].first == 5) str += tokentable.getID(tokens[1].second);
            else str += tokentable.getStr(tokens[1]);

            if (tokens[0].first == 1 && tokens[0].second == 9) 
                str += " ^ ";
            else str += ' ' + tokentable.getStr(tokens[0]) + ' ';

            if (tokens[2].first == 0) str += 'T' + to_string(tokens[2].second);
            else if (tokens[2].first == 5) str += tokentable.getID(tokens[2].second);
            else str += tokentable.getStr(tokens[2]);
        }
    }
    return str;
}

bool FCompiler::cmp(pair<int, int> index1, pair<int, int> index2) {
    string index1_str = tokentable.getStr(index1);
    string index2_str = tokentable.getStr(index2);
    int index1_num = cmp_num(index1_str);
    if (index1.first == 1 && index1.second == 9) index1_num = 4;
    int index2_num = cmp_num(index2_str);
    if (index2.first == 1 && index2.second == 9) index2_num = 4;
    if (index1_num == 4 && index2_num == 4) return true;
    else if (index1_num > index2_num) return true;
    else return false;
}

int FCompiler::cmp_num(string str) {
    if (str == "*" || str == "/" || str == "AND") return 3;
    if (str == "+" || str == "-" || str == "OR") return 2;
    if (str == "EQ" || str == "NE" || str == "GT" || str == "GE" || str == "LT" || str == "LE") return 1;
    return 0;
}