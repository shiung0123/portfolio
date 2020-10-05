#include <iostream>
#include "FC.h"
using namespace std;

#define MENU_Quit 0
#define Generate_Intermediate_Code 4

int main() {
    int mode;   // ���ﶵ
    int result; // ���O�^���ˬd
    FCompiler fc;

    while (true) {

        // ��X���
        cout << "                 MENU                 " << endl;
        cout << "* 0.             Quit                *" << endl;
        cout << "* 4.    Generate Intermediate Code   *" << endl;

        cout << "choice: ";

        // ��J���
        cin >> mode;

        // �P�_��ܪ����e
        switch (mode) {
        case MENU_Quit:
            return 0; // �h�X

        case 4:
            result = fc.Entry();
            break;

        default:
            cout << "No such option";
            continue;
        }

        // check is return successful 0
        if (result == 0) cout << endl;
        else return 1;
    };
    return 0;
}