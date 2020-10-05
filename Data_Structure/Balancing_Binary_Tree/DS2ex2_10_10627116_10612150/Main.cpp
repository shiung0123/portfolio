// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
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
    int mode;   // 選單選項
    int result; // 指令回傳檢查
    while (true) {

        // 輸出選單
        cout << "              MENU              " << endl;
        cout << "* 0.          QUIT             *" << endl;
        cout << "* 1.        2-3 TREE           *" << endl;
        cout << "* 2.        AVL TREE           *" << endl;
        cout << "* 3.   Intersection Query      *" << endl;
        cout << "choice: ";

        // 輸入選擇
        cin >> mode;

        HandleFile f;

        // 判斷選擇的內容
        switch (mode) {
        case MENU_QUIT:
            return 0; // 退出

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
