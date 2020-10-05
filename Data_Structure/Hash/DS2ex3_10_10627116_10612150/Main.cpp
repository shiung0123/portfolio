// ��10�� 106127116 �\�h�� 10612150 �L���� ��T�G��
// must to use -std=c++11 or higher version

#include "HandleFile.h"
#include "Header.h"
#include "Prime.h"
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

// clang-format off
#define MENU_QUIT               0
#define MENU_LINEAR             1
#define MENU_DOUBLE_HASH        2
#define MENU_QUADRATIC_HASH     3
// clang-format on

int main(int argc, char *argv[])
{
    int mode;   // ���ﶵ
    int result; // ���O�^���ˬd

    while (true) {

        // ��X���
        cout << "              MENU              " << endl;
        cout << "* 0.          Quit             *" << endl;
        cout << "* 1.      Linear Probing       *" << endl;
        cout << "* 2.       Double Hash         *" << endl;
        cout << "* 3.     Quadratic Hash        *" << endl;
        cout << "choice: ";

        // ��J���
        cin >> mode;

        HandleFile f;

        // �P�_��ܪ����e
        switch (mode) {
        case MENU_QUIT:
            return 0; // �h�X

        case MENU_LINEAR:
            result = f.task1();
            break;

        case MENU_DOUBLE_HASH:
            result = f.task2();
            break;
        case MENU_QUADRATIC_HASH:
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
