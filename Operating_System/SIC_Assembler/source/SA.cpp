#include "SA.h"

int pc = 0;
int bace = 0;
int temp_add;
bool pc_fix = false;
bool label;
bool pluss;
bool trans;
int i;
string binary;
bool isright;

int caculate(vector<pair<int, int>>& tokens, TokenTable& table, SymbolTable& symtable) {
    int num1, num2;
    int exp = 0;
    if (tokens[i].first == 5) {
        num1 = symtable.get(table.getStr(tokens[i]));
        i++;
    }
    while (i != tokens.size() && (tokens.size() - i) % 2 == 0) {
        if (tokens[i].first == 4 && (tokens[i].second == 2 || tokens[i].second == 3) &&
            tokens[i + 1].first == 5) {
            if (exp == 0) {
                num2 = symtable.get(table.getStr(tokens[i+1]));
                exp = tokens[i].second;
            }
            else if (exp == 2) {
                num1 += num2;
                num2 = symtable.get(table.getStr(tokens[i+1]));
                exp = tokens[i].second;
            }
            else if (exp == 3) {
                num1 -= num2;
                num2 = symtable.get(table.getStr(tokens[i+1]));
                exp = tokens[i].second;
            }
            i += 2;
        }
        else if (tokens[i].first == 4 && (tokens[i].second == 4 || tokens[i].second == 5) &&
            tokens[i + 1].first == 5) {
            if (exp == 0) {
                if (tokens[i].second == 4) num1 *= symtable.get(table.getStr(tokens[i+1]));
                else num1 /= symtable.get(table.getStr(tokens[i+1]));
            }
            else if (exp == 2 || exp == 3) {
                if (tokens[i].second == 4) num2 *= symtable.get(table.getStr(tokens[i+1]));
                else num2 /= symtable.get(table.getStr(tokens[i+1]));
            }
            i += 2;
        }
    }

    if (isright && i >= tokens.size()) {
        if (exp == 2)num1 += num2;
        else if (exp == 3)num1 -= num2;
    }
    else isright = false;
    return num1;
}

void PSEUDO(vector<pair<int, int>>& tokens, TokenTable& table, SymbolTable& symtable, LiteralTable& littable) {
    trans = false;
    if (i < tokens.size()) {
        switch (tokens[i++].second) {
        case 1: // start
            if (i != tokens.size() && tokens[i].first == 6) {
                pc = strhex_int(table.getStr(tokens[i]));
                temp_add = pc;
                pc_fix = true;
                i++;
            }
            else isright = false;
            break;
        case 2: // end
            if (i == tokens.size())littable.allocate(pc);
            else if (i != tokens.size() && tokens[i].first == 5) littable.allocate(pc), i++;
            else isright = false;
            break;
        case 3: // byte
            if (i != tokens.size() && tokens[i].first == 6) {
                binary += fillzero(strint_strhex(table.getStr(tokens[i])), 2);
                pc++;
                i++;
            }
            else if (i + 2 < tokens.size() &&
                tokens[i].first == 4 && tokens[i].second == 9 &&
                tokens[i + 2].first == 4 && tokens[i + 2].second == 9 &&
                (tokens[i + 1].first == 6 || tokens[i + 1].first == 7)) {
                if (tokens[i + 1].first == 6) {
                    binary += fillzero(table.getStr(tokens[i + 1]), 2);
                    pc++;
                }
                else {
                    string temp = fillzero(strchar_strhex(table.getStr(tokens[i + 1])), 2);
                    binary += temp;
                    pc += temp.length() / 2;
                }
                i += 3;
            }
            else isright = false;
            break;
        case 4: // word
            if (i != tokens.size() && tokens[i].first == 6) {
                binary += fillzero(strint_strhex(table.getStr(tokens[i])), 6);
                pc += 3;
                i++;
            }
            else if (i + 2 < tokens.size() &&
                tokens[i].first == 4 && tokens[i].second == 9 &&
                tokens[i + 2].first == 4 && tokens[i + 2].second == 9 &&
                (tokens[i + 1].first == 6 || tokens[i + 1].first == 7)) {
                if (tokens[i + 1].first == 6) {
                    binary += fillzero(table.getStr(tokens[i + 1]), 6);
                    pc += 3;
                }
                else {
                    string temp = fillzero(strchar_strhex(table.getStr(tokens[i + 1])), 6);
                    binary += temp;
                    pc += temp.length() / 2;
                }
                i += 3;
            }
            else isright = false;
            break;
        case 5: // resb
            if (i != tokens.size() && tokens[i].first == 6) {
                pc += strint_int(table.getStr(tokens[i]));
                i++;
            }
            else isright = false;
            break;
        case 6: // resw
            if (i != tokens.size() && tokens[i].first == 6) {
                pc += strint_int(table.getStr(tokens[i])) * 3;
                i++;
            }
            else isright = false;
            break;
        case 7: // beq pc_fix
            if (label == true && i != tokens.size()) {
                if (tokens[i].first == 5) {
                    temp_add = caculate(tokens, table, symtable);
                    // temp_add = symtable.get(table.getStr(tokens[i]));
                    pc_fix = true;
                    i++;
                }
                else if (tokens[i].first == 6) {
                    temp_add = strint_int(table.getStr(tokens[i]));
                    pc_fix = true;
                    i++;
                }
                else if (tokens[i].first == 4 && tokens[i].second == 4) {
                    i++;
                }
            }
            else isright = false;
            break;
        case 8: // base
            if (i != tokens.size() && tokens[i].first == 6) {
                // bace = strint_int(table.getStr(tokens[i]));
                binary = "00";
                trans = true;
                i++;
            }
            else if (i != tokens.size() && tokens[i].first == 5) {
                // bace = symtable.get(table.getStr(tokens[i]));
                binary = "01";
                trans = true;
                i++;
            }
            else isright = false;
            break;
        case 9: // ltorg
            littable.allocate(pc);
            break;
        default:
            break;
        }
    }
    if (isright && i >= tokens.size());
    else isright = false;
}

bool imm(vector<pair<int, int>>& tokens) {
    if (i + 1 < tokens.size() && tokens[i].first == 4 && tokens[i].second == 12) {
        if (tokens[i + 1].first == 5) {
            binary += "11";
            i += 2;
            return true;
        }
        else if (tokens[i + 1].first == 6) {
            binary += "12"; 
            i += 2;
            return true;
        }
    }
    return false;
}

bool direct(vector<pair<int, int>>& tokens) {
    if (i < tokens.size() && tokens[i].first == 5 && i + 1 == tokens.size()) {
        binary += "21";
        i++;
        return true;
    }
    else if (i < tokens.size() && tokens[i].first == 6) {
        binary += "22";
        i++;
        return true;
    }
    return false;
}

bool indirect(vector<pair<int, int>>& tokens) {
    if (i + 1 < tokens.size() && tokens[i].first == 4 && tokens[i].second == 13 &&
        tokens[i + 1].first == 5) {
        binary += "3";
        i += 2;
        return true;
    }
    return false;
}

bool index(vector<pair<int, int>>& tokens) {
    if (i + 2 < tokens.size() && tokens[i].first == 5 &&
        tokens[i + 1].first == 4 && tokens[i + 1].second == 1 &&
        tokens[i + 2].first == 3 && tokens[i + 2].second == 2 ) {
        binary += "4";
        i += 3;
        return true;
    }
    return false;
}

bool literall(vector<pair<int, int>>& tokens, TokenTable& table, LiteralTable& littable) {
    if (i + 3 < tokens.size() && tokens[i].first == 4 && tokens[i].second == 11 &&
        tokens[i + 1].first == 4 && tokens[i + 1].second == 9 &&
        tokens[i + 3].first == 4 && tokens[i + 3].second == 9) {
        if (tokens[i + 2].first == 6) {
            littable.insert(table.getStr(tokens[i + 2]), 0);
            binary += "51";
            i += 4;
            return true;
        }
        else if (tokens[i + 2].first == 7) {
            littable.insert(table.getStr(tokens[i + 2]), 1);
            binary += "52";
            i += 4;
            return true;
        }
    }
    return false;
}

void INSTRUCTION(vector<pair<int, int>>& tokens, TokenTable& table, InsTable& instable, SymbolTable& symtable, LiteralTable& littable) {
    if (i < tokens.size()) {
        if (tokens[i].second == 36) {
            binary = "4F0000";
            trans = false;
            pc += 3;
            i++;
        }
        else {
            switch (instable.getType(tokens[i++].second)) {
            case 1:
                if (i == tokens.size()) binary = "1", pc++;
                else isright = false;
                break;
            case 2:
                if (i < tokens.size() && tokens[i].first == 6) {
                    binary = "21";
                    pc += 2;
                    i++;
                }
                else if (i < tokens.size() && tokens[i].first == 3) {
                    if (i + 1 == tokens.size()) {
                        binary = "22";
                        pc += 2;
                        i++;
                    }
                    else if (i + 2 < tokens.size() && tokens[i + 1].first == 4 && tokens[i + 1].second == 1) {
                        if (tokens[i + 2].first == 3) {
                            binary = "23";
                            pc += 2;
                            i += 3;
                        }
                        else if (tokens[i + 2].first == 6) {
                            binary = "24";
                            pc += 2;
                            i += 3;
                        }
                        else isright = false;
                    }
                    else isright = false;
                }
                else isright = false;
                break;
            case 3:
                if (pluss == true) {
                    binary = "4";
                    if (imm(tokens) || direct(tokens) || indirect(tokens) || index(tokens) || literall(tokens, table, littable)) pc += 4;
                    else isright = false;
                }
                else {
                    binary = "3";
                    if (imm(tokens) || direct(tokens) || indirect(tokens) || index(tokens) || literall(tokens, table, littable)) pc += 3;
                    else isright = false;
                }
                break;
            }
        }
    }

    if (isright && i >= tokens.size());
    else isright = false;
}

string checkSyntax_SICXE(vector<pair<int, int>>& tokens, TokenTable& table, InsTable& instable, SymbolTable& symtable, LiteralTable& littable, int& add, int& b) {
    binary = ""; // "label"
    isright = true;
    trans = true;
    pc_fix = false;
    label = false;
    pluss = false;
    i = 0;
    add = pc;
    b = bace;

    if (!tokens.empty()) {
        if (tokens[i].first == 5) label = true, i++;
        if (i < tokens.size() && tokens[i].first == 4 && tokens[i].second == 2) {
            pluss = true, i++;
            if (i == tokens.size()) isright = false;
        }
        if (i < tokens.size() && tokens[i].first == 2) PSEUDO(tokens, table, symtable, littable);
        else if (i < tokens.size() && tokens[i].first == 1) INSTRUCTION(tokens, table, instable, symtable, littable);
    }

    if (pc_fix) add = temp_add;
    if (isright == false) {
        binary = 'X';
        b = -1;
    }
    if (trans == false) {
        b = -1;
    }

    cout << isright << " :" << binary << endl;

    return binary;
}

string checkSyntax_SIC(vector<pair<int, int>>& tokens, TokenTable& table, InsTable& instable, SymbolTable& symtable, LiteralTable& littable, int& add, int& b) {
    binary = ""; // "label"
    isright = true;
    trans = true;
    pc_fix = false;
    label = false;
    pluss = false;
    i = 0;
    add = pc;
    b = bace;

    if (!tokens.empty()) {
        if (tokens[i].first == 5) label = true, i++;
        if (i < tokens.size() && tokens[i].first == 2) PSEUDO(tokens, table, symtable, littable);
        else if (i < tokens.size() && tokens[i].first == 1) {
            if (tokens[i].second == 36) {
                binary = "4C0000";
                trans = false;
                pc += 3;
                i++;
            }
            else {
                switch (instable.getType(tokens[i++].second)) {
                case 3:
                    if (i < tokens.size() && tokens[i].first == 5) {
                        if (i + 1 == tokens.size()) {
                            binary = "321";
                            pc += 3;
                            i++;
                        }
                        else if (i + 2 < tokens.size() && tokens[i + 1].first == 4 && tokens[i + 1].second == 1 &&
                            tokens[i + 2].first == 3 && tokens[i + 2].second == 2) {
                            binary = "34";
                            pc += 3;
                            i += 3;
                        }
                        else isright = false;
                        break;
                    }
                }
            }
            if (isright && i >= tokens.size());
            else isright = false;
        }
    }

    if (pc_fix) add = temp_add;
    if (isright == false) {
        binary = 'X';
        b = -1;
    }
    if (trans == false) {
        b = -1;
    }

    cout << isright << " :" << binary << endl;

    return binary;
}



