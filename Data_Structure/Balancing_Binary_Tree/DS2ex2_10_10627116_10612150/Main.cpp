// ��10�� 106127116 �\�h�� 10612150 �L���� ��T�G��
// must to use -std=c++11 or higher version

#include "HandleFile.h"
#include "Header.h"
#include "Test.h"
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

// clang-format off
#define MENU_QUIT               0
#define MENU_TREE23             1
#define MENU_AVL                2
#define MENU_IQ                 3
// clang-format on

int main(int argc, char *argv[])
{
#ifdef DEBUG1111
    debug();
#else
    int mode;   // ���ﶵ
    int result; // ���O�^���ˬd
    while (true) {

        // ��X���
        cout << "              MENU              " << endl;
        cout << "* 0.          QUIT             *" << endl;
        cout << "* 1.        2-3 TREE           *" << endl;
        cout << "* 2.        AVL TREE           *" << endl;
        cout << "* 3.   Intersection Query      *" << endl;
        cout << "choice: ";

        // ��J���
        cin >> mode;

        HandleFile f;

        // �P�_��ܪ����e
        switch (mode) {
        case MENU_QUIT:
            return 0; // �h�X

        case MENU_TREE23:
            result = f.task1();
            break;

        case MENU_AVL:
            result = f.task2();
            break;
        case MENU_IQ:
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
#endif
    return 0;
}
