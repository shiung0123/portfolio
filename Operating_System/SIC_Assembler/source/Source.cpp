#include <iostream>
#include "TokenTable.h"
#include "TokenFile.h"
#include "CA.h"
using namespace std;

#define MENU_Quit 0
#define MENU_Read_Table 1
#define MENU_Get_Token 2
#define MENU_RUN 3

int main() {
    int mode;   // ���ﶵ
    int result; // ���O�^���ˬd
    TokenTable tokentable;
    // InsTable instable;
    CrossAssembler ca;
    TokenFile file;

    while (true) {

        // ��X���
        cout << "                 MENU                 " << endl;
        cout << "* 0.             Quit                *" << endl;
        cout << "* 1.        Read Token Table         *" << endl;
        cout << "* 2.        Read INS Table           *" << endl;
        cout << "* 3.      Read Table & Get Token     *" << endl;
        cout << "* 4.      Generate Machine Code      *" << endl;

        cout << "choice: ";

        // ��J���
        cin >> mode;

        // �P�_��ܪ����e
        switch (mode) {
        case MENU_Quit:
            return 0; // �h�X

        case MENU_Read_Table:
            result = tokentable.Menu();
            break;

        case MENU_Get_Token:
            result = file.Menu(tokentable);
            break;

        case MENU_RUN:
            result = tokentable.Menu();
            result = file.Menu(tokentable);
            break;

        case 4:
            result = ca.Menu();
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