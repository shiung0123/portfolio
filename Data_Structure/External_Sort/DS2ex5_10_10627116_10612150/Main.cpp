// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
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
    int mode;   // 選單選項
    int result; // 指令回傳檢查
    HandleFile f;

    while (true) {

        // 輸出選單
        cout << "              MENU              " << endl;
        cout << "* 0.          Quit             *" << endl;
        cout << "* 1.       External Sort       *" << endl;
        cout << "* 2.       Primary Index       *" << endl;
        cout << "* 3.       Range Search        *" << endl;
        cout << "choice: ";

        // 輸入選擇
        cin >> mode;

        // 判斷選擇的內容
        switch (mode) {
        case MENU_QUIT:
            return 0; // 退出

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
