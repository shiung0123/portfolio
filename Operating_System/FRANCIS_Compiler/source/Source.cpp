#include <iostream>
#include "FC.h"
using namespace std;

#define MENU_Quit 0
#define Generate_Intermediate_Code 4

int main() {
    int mode;   // 選單選項
    int result; // 指令回傳檢查
    FCompiler fc;

    while (true) {

        // 輸出選單
        cout << "                 MENU                 " << endl;
        cout << "* 0.             Quit                *" << endl;
        cout << "* 4.    Generate Intermediate Code   *" << endl;

        cout << "choice: ";

        // 輸入選擇
        cin >> mode;

        // 判斷選擇的內容
        switch (mode) {
        case MENU_Quit:
            return 0; // 退出

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