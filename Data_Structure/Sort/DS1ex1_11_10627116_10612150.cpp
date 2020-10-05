// 第11組 106127116 許逸翔 10612150 林詠翔 資訊二甲
#include <iostream>
#include <list>
#include <iterator>
#include <ctime>
#include <cstring>
using namespace std;

#define MENU_ERROR            0
#define MENU_INCREMENTAL_LIST 1
#define MENU_INPUT_LIST       2
#define MENU_LIMIT_LIST       3
#define MENU_QUIT             4

int counter;

// 排列遞迴
void permutation(list<int> &numList, list<int> &pick, int limitLength) 
{   
    list<int>::iterator it;

    // 判斷numList是否含有項目，沒有時則條件成立
    // limiteLength為-1時則不啟用該功能，否則他可以限制排列的最大長度
    if (!numList.size() || (limitLength != -1 && pick.size() == limitLength)) {
        // basecase
        // 顯示次數計數器遞增後顯示
        cout << '[' << ++counter << ']';

        // 將堆疊內容顯示出來
        for (it = pick.begin(); it != pick.end(); ++it)
            cout << " " << *it << ",";

        // 去除','字元
        cout << "\b " << endl;

        return;
    }
     
    // 下列迴圈用於選取所有選項各一次，確保所有組合都被選取
    for (int n = numList.size(); n > 0; --n) {

        // 紀錄目前選取的那一項到堆疊(pick)中
        int num = numList.front();
        pick.push_back(num);

        //移除numList的第一項
        numList.pop_front();

        // 遞迴自己，並且保留長度設定值
        permutation(numList, pick, limitLength);

        // 還原原本移除的內容
        numList.push_back(num);

        // 堆疊pop
        pick.pop_back();
    }

    return;
}

// 錯誤處理 
void errorHandling(string message)
{
    // 如果eof則強制結束程式
    if (cin.eof())
        exit(0);

    // 恢復cin的狀態 
    cin.clear();

    // 消滅最多2048個字元遇到\n 
    cin.ignore(2048, '\n');

    // 顯示錯誤訊息
    cout << message << endl;
}

int task1Input() 
{
    int n;
    while (1) {
        // 顯示輸入提示訊息
        cout << "Input: ";

        // 請求輸入數字N
        cin >> n;
        
        // 檢查cin是否正常並且正確
        // 不必特別檢查n是否溢位，若溢位cin就會回報錯誤   
        // 為了避免堆疊溢出而限制於20000以下
        if (cin && 0 < n && n < 20000)
            return n;
        errorHandling("Error: not a postive number or too large!"); 
    }
}

// 任務1
int task1() 
{
    int n = task1Input();
    
    // 產生由1到N的陣列與
    list<int> numList, pick;
    for (int i = 1; i <= n; ++i)
        numList.push_back(i);

    // 顯示輸出提示
    cout << "Output" << endl;

    // 呼叫並且maxLength為-1
    permutation(numList, pick, -1);

    //顯示最大層數
    cout << "L= " << n + 1 << endl;
    
    // 回傳結果為successful
    return 0;
}

void getNumber(char &c, int &numTmp) 
{
    numTmp = 0;
    // it must be a number character
    if (!isdigit(c))
        throw "invalid Character [0-9]";

    while (1) {
        if (isdigit(c)) {
            numTmp = numTmp * 10 + c - '0';        
            if (numTmp < 0)
                throw "Overflow";
        } else if (strchr("\n ,", c) != NULL)
            break;
        else
            throw "invalid Character [0-9]";
        cin.get(c);
    } 
}

void ignoreSpace(char &c)
{
    while (c == ' ')
        cin.get(c);
}

int delimiter(char &c, list<int> &retList, int &numTmp)
{
    if (strchr("\n,", c) == NULL) 
        throw "invalid Character (',', '\\n')";

    retList.push_back(numTmp);
    numTmp = 0;

    if (c == ',')  
        cin.get(c);
    else if (c == '\n')
        return 0;
    return 1;
}

void task2Input(list<int> &retList)
{
    // 丟棄\n
    cin.ignore(1);
    
    while (1) {
        // 顯示輸入提示訊息
        cout << "(ex: 1,2,3,4)" << endl;
        cout << "Input: ";

        // 輸入一組數字M
        int numTmp = 0;
        char c;
        try {
            cin.get(c);
            do {
                // space trim
                ignoreSpace(c);

                // number
                getNumber(c, numTmp);

                // space trim
                ignoreSpace(c);

                // '\n' or ','
            } while (delimiter(c, retList, numTmp));
            // 檢查cin是否正常並且正確，以及M的範圍
            if (!(cin && 2 <= retList.size() && retList.size() <= 9))
                throw "!(2 <= m <= 9) or cin error";
            else
                break;
        } catch (const char *err){
             
            // cout << err << c << endl;
            if (c != '\n')
                cin.ignore(2048, '\n'); 
            // 如果eof則強制結束程式
            if (cin.eof())
                exit(0);

            cout << "Error: " << err << endl;
            retList.clear();
        }
    }
}

// 任務2
int task2() 
{
    clock_t timer1, timer2;
         
    // 連續輸入M個數字
    // 依序填入輸入的數字
    list<int> numList;
    task2Input(numList);

    // 產生空的pickList
    list<int> pick;
    
    // 顯示輸出提示
    cout << "Output" << endl;

    // 計時器1計時
    timer1 = clock();
    
    // 呼叫並且maxLength為-1
    permutation(numList, pick, -1);

    // 計時器2計時
    timer2 = clock();

    // 顯示時間差
    cout << "T= " << (float) (timer2 - timer1) * 1000 / CLOCKS_PER_SEC << "ms" << endl;      

    // 回傳結果為successful
    return 0;
}

int task3Input() 
{
    int m;
    while (1) {
        // 顯示輸入提示訊息
        cout << "Input: ";

        // 請求輸入數字M
        cin >> m;
        
        // 檢查cin是否正常並且正確，以及M的範圍
        if (cin && (0 < m && m <= 9))
            return m;
        errorHandling("Error: out of range!"); 
    }
}

// 任務3
int task3() 
{
    clock_t timer1, timer2;
    int m = task3Input();
    
    // 產生大小為9的numlist與產生空的pickList
    list<int> numList, pick;
    for (int i = 1; i <= 9; ++i)
        numList.push_back(i);

    // 顯示輸出提示
    cout << "Output" << endl;

    // 計時器1計時
    timer1 = clock();
    
    // 呼叫並且maxLength為M
    permutation(numList, pick, m);

    // 計時器2計時
    timer2 = clock();

    // 顯示時間差
    cout << "T= " << (float) (timer2 - timer1) * 1000 / CLOCKS_PER_SEC << "ms" << endl;      

    // 回傳結果為successful
    return 0;
}

int main(int argc, char *argv[])
{
    int mode;                           // 選單選項
    int result;                         // 指令回傳檢查
    while (1) {
        //初始化次數計數器
        counter = 0;

        // 輸出選單
        cout << "           MENU           " << endl;
        cout << "* 1. N numbers from 1..N *" << endl;
        cout << "* 2. M numbers from input*" << endl;
        cout << "* 3. M numbers from 1..9 *" << endl;
        cout << "* 4. Quit                *" << endl;
        cout << "choice: ";

        // 輸入選擇
        cin >> mode;
         
        // 判斷選擇的內容
        switch (mode) {
            case MENU_QUIT:
                return 0;               // 退出
            case MENU_INCREMENTAL_LIST:
                result = task1();       // 任務1
                break;
            case MENU_INPUT_LIST:
                result = task2();       // 任務2
                break;
            case MENU_LIMIT_LIST:
                result = task3();       // 任務3
                break;
            default:
                errorHandling("Error: Command not found!"); 
                continue;
        }

        // 檢查回傳值是否為successful
        if (result)
            return 1;                   
    };
    return 0;
}


