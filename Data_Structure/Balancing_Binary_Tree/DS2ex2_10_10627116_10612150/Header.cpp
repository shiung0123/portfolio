// ��10�� 106127116 �\�h�� 10612150 �L���� ��T�G��
// must to use -std=c++11 or higher version encoding=utf-8
#include "Header.h"

bool inputSuccess;

void errorHandling(string message)
{
    // �p�Geof�h�j����{��
    if (cin.eof())
        exit(0);

    // ��_cin�����A
    cin.clear();

    // �����̦h2048�Ӧr���J��\n
    cin.ignore(2048, '\n');

    // ��ܿ��~�T��
    cout << message << endl;
}

static string getOnlyDigits(string str)
{
    string tmp = "";
    for (char c : str)
        if (isdigit(c))
            tmp += c;

    return tmp;
}

static int sToi(string str)
{
    try {
        // "1,223,234,234,234"
        if (str[0] == '\"')
            str = getOnlyDigits(str);

        return stoi(str);
    }
    catch (exception e) {
        cout << "ERROR : stoi error!" << endl;
        cout << "Value : \"" << str << "\"" << endl;
        return -1; // return error value
    }
}

int stringToInt(string str) { return sToi(str); }
