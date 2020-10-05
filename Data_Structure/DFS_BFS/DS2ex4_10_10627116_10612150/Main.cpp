// ��10�� 106127116 �\�h�� 10612150 �L���� ��T�G��
// must to use -std=c++11 or higher version

#include "AdjacencyList.h"
#include "Header.h"
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

// clang-format off
#define MENU_QUIT               0
#define MENU_LOAD               1
#define MENU_INFLUENCE          2
// clang-format on

int main(int argc, char *argv[])
{
    int mode;   // ���ﶵ
    int result; // ���O�^���ˬd
    AdjacencyList f;

    while (true) {

        // ��X���
        cout << "              MENU              " << endl;
        cout << "* 0.          Quit             *" << endl;
        cout << "* 1.   Load Adjacency Lists    *" << endl;
        cout << "* 2.        Influence          *" << endl;
        cout << "choice: ";

        // ��J���
        cin >> mode;

        // �P�_��ܪ����e
        switch (mode) {
        case MENU_QUIT:
            return 0; // �h�X

        case MENU_LOAD:
            result = f.task1();
            break;

        case MENU_INFLUENCE:
            result = f.task2();
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
