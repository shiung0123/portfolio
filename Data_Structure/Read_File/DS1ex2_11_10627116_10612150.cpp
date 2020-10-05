// 第11組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

#define MENU_COPY_FILE        1 
#define MENU_FILTER_FILE      2
#define MENU_MERGE_FILE       3
#define MENU_QUIT             4

#define DATA_SIZE             11
#define DATA_ID               0
#define DATA_NAME             1
#define DATA_DEPARTMENT_ID    2
#define DATA_DEPARTMENT_NAME  3
#define DATA_DAY_EXT_EDU      4
#define DATA_STAGE            5
#define DATA_STUDENTS         6
#define DATA_TEACHERS         7
#define DATA_GRADUATES        8
#define DATA_CITY             9
#define DATA_TYPE             10


static bool inputSuccess;
void errorHandling(string message);

class Data
{
public:
    string column[DATA_SIZE];

    friend istream &operator>>(istream &in, Data &data)
    {
        string input, temp;
        getline(in, input);

        if (!in)
            return in;
        // drop \r if the program running on unix
        // or unix like system, the string may be 
        // contained '\r'
        if (input.back() == '\r')
            input.pop_back();

        // put \t for split easily
        input += '\t';

        // splitting
        int count = 0;
        inputSuccess = true;
        for (char c : input) {
            if (c != '\t')
                temp += c;
            else {
                data.column[count++] = temp;
                temp = "";
            }
        }

        if (count != DATA_SIZE)
            inputSuccess = false;

        return in;
    }

    friend ostream &operator<<(ostream &out, Data &data)
    {
        for (int i = 0; i < DATA_SIZE; i++)
            out << data.column[i] << (i < DATA_SIZE - 1 ? '\t' : '\n');

        return out;
    }
};

class HandleFile
{
    fstream fin, fmerge;
    fstream fout;

    // common function
    string getOnlyDigits(string str)
    {
        string tmp = "";
        for (char c : str)
            if (isdigit(c)) tmp += c;

        return tmp;
    }

    int numberInput(string message, string errorMsg)
    {
        int result;
        while (true) {
            cout << message;
            cin >> result;
            if (cin && result > -1)
                return result;
            else
                errorHandling("Error : " + errorMsg);
        }
    }

    void save(string saveName, vector<Data> &database)
    {
        // closs all file
        if (fin.is_open())
            fin.close();

        if (fmerge.is_open())
            fmerge.close();

        fout.open(saveName, ios::out | ios::trunc);
        for (Data i : database)
            fout << i;         // << overload

        fout.close();

        cout << "Total number of records: " << database.size() << endl;
    }

    void dropHeader(fstream &file)
    {
        for (int i = 0; i < 3; ++i)
            file.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string fileInput(fstream &file, string message, string prefix)
    {
        string fileName;
        while (true) {

            // input file name
            cout << message;
            cin  >> fileName;

            // quit program if input "0"
            if (fileName == "0")
                return "";

            file.open(prefix + fileName + ".txt", ios::in);

            if (!file) {
                errorHandling("Error : there is no such file!");
                continue; // input again
            }

            if (prefix == "input")
                dropHeader(file);

            return fileName;
        }
    }

    // use in task1
    bool task1_input(string &fileName)
    {
        fileName = fileInput(fin, "Input (201, 202, ...[0]Quit): ", "input");

        // if fileName == "" then quit to menu
        return fileName != "";  // {quit: 0, continue: 1}
    }

    int stringToInt(string str)
    {
        try {
            // "1,223,234,234,234"
            if (str[0] == '\"')
                str = getOnlyDigits(str);

            return stoi(str);
        }
        catch (exception e) {
            cout << "ERROR : stoi error!" << endl;
            cout << "Value : " << str << endl;
            return -1; // return error value
        }
    }

    bool task2_input(int &students, int &graduates, string &fileName)
    {
        string errorMsg = "number out of range!";
        fileName = fileInput(fin, "Input (201, 202, ...[0]Quit): ", "copy");
        if (fileName == "")
            return 0; // quit

        students = numberInput("Threshold on number of students: ", errorMsg);
        graduates = numberInput("Threshold on number of graduates: ", errorMsg);

        return 1;
    }

    // use in task3
    void merge(vector<Data> &database, vector<int> &selected) {

        // comp function return data priority
        Data temp;
        while (fmerge >> temp) {
            
            // check database from end to begin
            for (int i = database.size() -1 ; i >= 0; i--) {

                // check current condition DATA_NAME
                if (database[i].column[DATA_NAME] == temp.column[DATA_NAME]) {
                    for (int j = i; j >= 0; j--) {
                        // check previous condition {DATA_NAME}
                        if (database[j].column[DATA_NAME] != temp.column[DATA_NAME]) 
                            break;

                        // check current condition DATA_DEPARTMENT_NAME
                        if (database[j].column[DATA_DEPARTMENT_NAME] == temp.column[DATA_DEPARTMENT_NAME]) {
                            database.insert(database.begin()+j+1, temp);
                            goto success;
                        }
                    }
                    
                    database.insert(database.begin()+i+1, temp);
                    goto success;
                }
            }
            // if not found any thing than put in back
            database.push_back(temp);
        success:;
        }
    }

    bool task3_input(string &fileName1, string &fileName2)
    {
        fileName1 = fileInput(fin, "Input 1st(201, 202, ...[0]Quit): ", "copy");
        if (fileName1 == "")
            return 0; // quit

        fileName2 = fileInput(fmerge, "Input 2nd(201, 202, ...[0]Quit): ", "copy");
        if (fileName2 == "")
            return 0; // quit

        return 1;
    }

public:

    bool task1()
    {
        vector<Data> database;
        string fileName;

        if (!task1_input(fileName)) {
            cout << "switch to menu" << endl;
            return 0;
        }

        Data temp;
        while (fin >> temp)     // >> overload
            if (inputSuccess) database.push_back(temp);

        save("copy" + fileName + ".txt", database);

        return 0;
    }

    bool task2()
    {
        vector<Data> database;
        int students, graduates;
        string fileName;

        if (!task2_input(students, graduates, fileName)) {
            cout << "switch to menu" << endl;
            return 0;
        }

        Data temp;
        while (fin >> temp) {
            if (inputSuccess &&
                stringToInt(temp.column[DATA_STUDENTS]) >= students &&
                stringToInt(temp.column[DATA_GRADUATES]) >= graduates)
            {
                database.push_back(temp);
            }
        }

        save("copy" + fileName + ".txt", database);

        return 0;
    }

    bool task3()
    {
        vector<Data> database;
        vector<int>  selected;
        string fileName1, fileName2;

        if (!task3_input(fileName1, fileName2)) {
            cout << "switch to menu" << endl;
            return 0;
        }

        Data temp;
        while (fin >> temp)     // >> overload
            if (inputSuccess) database.push_back(temp);

        // College priority than department
        selected.push_back(DATA_ID);
        selected.push_back(DATA_DEPARTMENT_ID);

        merge(database, selected);
        save("output" + fileName1 + "_" + fileName2 + ".txt", database);

        return 0;
    }
};

int main(int argc, char *argv[])
{
    int mode;                           // 選單選項
    int result;                         // 指令回傳檢查
    while (true) {

        // 輸出選單
        cout << "              MENU              " << endl;
        cout << "* 1. COPY (Read & Save a file) *" << endl;
        cout << "* 2. FILTER (Reduce a file)    *" << endl;
        cout << "* 3. MERGE (Join two files)    *" << endl;
        cout << "* 4. Quit                      *" << endl;
        cout << "choice: ";

        // 輸入選擇
        cin >> mode;

        HandleFile f;

        // 判斷選擇的內容
        switch (mode) {
        case MENU_QUIT:
            return 0;               // 退出

        case MENU_COPY_FILE:
            result = f.task1();       // 任務1
            break;

        case MENU_FILTER_FILE:
            result = f.task2();       // 任務2
            break;

        case MENU_MERGE_FILE:
            result = f.task3();       // 任務3
            break;

        default:
            errorHandling("Error: Command not found!");
            continue;
        }

        // 檢查回傳值是否為successful
        if (result)
            return 1;
        else cout << endl;
    };
    return 0;
}

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
