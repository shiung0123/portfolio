// ��10�� 106127116 �\�h�� 10612150 �L���� ��T�G��
// must to use -std=c++11 or higher version

#include "HandleFile.h"
#include "Header.h"
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

// clang-format off
#define MENU_QUIT               0
#define MENU_EXTERNAL_SORT      1
#define MENU_PRIMARY_INDEX      2
#define MENU_RANGE_SEARCH       3
// clang-format on

int main(int argc, char *argv[])
{
    int mode;   // ���ﶵ
    int result; // ���O�^���ˬd
    HandleFile f;

    while (true) {

        // ��X���
        cout << "              MENU              " << endl;
        cout << "* 0.          Quit             *" << endl;
        cout << "* 1.       External Sort       *" << endl;
        cout << "* 2.       Primary Index       *" << endl;
        cout << "* 3.       Range Search        *" << endl;
        cout << "choice: ";

        // ��J���
        cin >> mode;

        // �P�_��ܪ����e
        switch (mode) {
        case MENU_QUIT:
            return 0; // �h�X

        case MENU_EXTERNAL_SORT:
            result = f.task1();
            break;

        case MENU_PRIMARY_INDEX:
            result = f.task2();
            break;
        case MENU_RANGE_SEARCH:
            result = f.task3();
            break;

        default:
            errorHandling("Error: Command not found!");
            continue;
        }

        // check is return successful
        if (result)
            return 1;
        else
            cout << endl;
    };
    return 0;
}
