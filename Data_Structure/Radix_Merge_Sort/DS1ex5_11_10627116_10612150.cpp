// 第11組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#include <algorithm>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

// clang-format off
#define MENU_QUIT               0
#define MENU_SELECT_BUBBLE      1
#define MENU_MERGE_QUICK        2
#define MENU_RADIX              3

#define DATA_SIZE               11
#define DATA_ID                 0
#define DATA_NAME               1
#define DATA_DEPARTMENT_ID      2
#define DATA_DEPARTMENT_NAME    3
#define DATA_DAY_EXT_EDU        4
#define DATA_STAGE              5
#define DATA_STUDENTS           6
#define DATA_TEACHERS           7
#define DATA_GRADUATES          8
#define DATA_CITY               9
#define DATA_TYPE               10
// clang-format on

static bool inputSuccess;
void errorHandling(string message);

static string getOnlyDigits(string str)
{
    string tmp = "";
    for (char c : str)
        if (isdigit(c))
            tmp += c;

    return tmp;
}

static int stringToInt(string str)
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

// select column datatype must be integer
static vector<int> selectOrder;

class Data {
    string column[DATA_SIZE];

public:
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

    int convertToInt(int num) { 
        return stringToInt(column[num]);
    }

    bool operator>(Data &b)
    {
        for (auto i : selectOrder) {
            if (stringToInt(column[i]) < stringToInt(b.column[i])) return true;
            else if (stringToInt(column[i]) > stringToInt(b.column[i])) return false;
            // else stringToInt(column[i]) == stringToInt(b.column[i] then i++
        }
        return false;
    }

    bool operator>=(Data &b)
    {
        for (auto i : selectOrder) {
            if (stringToInt(column[i]) < stringToInt(b.column[i])) return true;
            else if (stringToInt(column[i]) > stringToInt(b.column[i])) return false;
            // else stringToInt(column[i]) == stringToInt(b.column[i] then i++
        }
        return true;
    }

    bool operator<=(Data &b)
    {
        for (auto i : selectOrder) {
            if (stringToInt(column[i]) > stringToInt(b.column[i])) return true;
            else if (stringToInt(column[i]) < stringToInt(b.column[i])) return false;
            // else stringToInt(column[i]) == stringToInt(b.column[i] then i++
        }
        return true;
    }

    bool operator<(Data &b)
    {
        for (auto i : selectOrder) {
            if (stringToInt(column[i]) > stringToInt(b.column[i])) return true;
            else if (stringToInt(column[i]) < stringToInt(b.column[i])) return false;
            // else stringToInt(column[i]) == stringToInt(b.column[i] then i++
        }
        return false;
    }
};

class HandleFile {
    fstream fin;
    fstream fout;

    // common function
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

        fout.open(saveName, ios::out | ios::trunc);
        for (Data i : database)
            fout << i; // << overload

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
            cin >> fileName;

            // quit program if input "0"
            if (fileName == "0")
                return "";

            file.open(prefix + fileName + ".txt", ios::in);

            if (!file) {
                errorHandling("Error : there is no such file!");
                continue; // input again
            }

            dropHeader(file);

            return fileName;
        }
    }

    // use in task123 & set select order
    bool task123_input(string &fileName, vector<Data> &database)
    {
        fileName = fileInput(fin, "Input (501, 502, ...[0]Quit): ", "input");

        // if fileName == "" then quit to menu
        if (fileName != "") {
            Data temp;
            while (fin >> temp) // >> overload
                if (inputSuccess)
                    database.push_back(temp);
        }
        else {
            cout << "switch to menu" << endl;
        }

        selectOrder.push_back(DATA_GRADUATES);
        selectOrder.push_back(DATA_TEACHERS);

        return fileName != ""; // {quit: 0, continue: 1}
    }

    typedef const function<void()> timerHandler;
    void timing(const string &display, timerHandler doing)
    {
        clock_t t = clock();
        doing();
        cout << display << (clock() - t) << " ms" << endl;
    }

public:
    // sorts (decrement)
    template <class T>
    void selection(vector<T> &array)
    {
        int size = array.size();
        for (int i = 0; i < size; i++) {
            int maxIndex = i;
            for (int j = i; j < size; j++) {
                if (array[maxIndex] > array[j])
                    maxIndex = j;
            }
            swap(array[maxIndex], array[i]);
        }
    }

    template <class T>
    void bubble(vector<T> &array)
    {
        int size = array.size();
        for (int i = 0; i < size; i++) {
            for (int j = size - 2; j >= i; j--) {
                if (array[j] > array[j + 1])
                    swap(array[j], array[j + 1]);
            }
        }
    }

    template <class T>
    void merge(vector<T> &array)
    {
        int size = array.size();
        if (size <= 1)
            return;

        // split
        int lSize = size / 2;
        vector<T> left(lSize);
        copy(array.begin(), array.begin() + lSize, left.begin());

        int rSize = size - lSize;
        vector<T> right(rSize);
        copy(array.begin() + lSize, array.begin() + size, right.begin());

        // divide and conquer sort
        merge(left);
        merge(right);

        // merge
        auto lPtr = left.begin(), rPtr = right.begin();
        auto ptr = array.begin();
        while (lPtr < left.begin() + lSize && rPtr < right.begin() + rSize) {
            if (*lPtr < *rPtr)
                *ptr = *lPtr, lPtr++;
            else
                *ptr = *rPtr, rPtr++;
            ptr++;
        }

        while (lPtr < left.begin() + lSize)
            *ptr = *lPtr, lPtr++, ptr++;

        while (rPtr < right.begin() + rSize)
            *ptr = *rPtr, rPtr++, ptr++;
    }

    template <class It>
    void quick(It begin, int size)
    {
        auto front = begin, last = begin + size - 1;

        if (size <= 1)
            return;

        // take first for pivot
        auto pivot = front;
        front++;

        // sort
        while (front < last) {
            // more than the pivot then put it after that pivot
            if (*front > *pivot) {
                // find a one less than the pivot then put it before that pivot
                while (front<last && * last> * pivot)
                    last--;

                swap(*front, *last);
            }
            front++;
        }

        // put pivot at center
        if (*last >= *pivot)
            last--;

        swap(*last, *pivot);
        pivot = last;

        // divide and conquer sort
        int leftSize = (pivot - begin);
        quick(begin, leftSize);
        quick(pivot + 1, size - leftSize - 1);
    }

    int getDigit(int num, int digit, int base)
    {
        int hi = 1, lo = 1;
        while (digit > 0)
            hi *= base, digit--;
        lo = hi / base;
        return num % hi / lo;
    }

    int getMaxDigit(int num, int base)
    {
        int ret = 0;
        while (num > 0)
            num /= base, ret++;
        return ret;
    }

    template <class T>
    void redix(vector<T> &array, int base)
    {
        int size = array.size();
        // base
        for (int order = selectOrder.size() - 1; order >= 0; order--) {
            vector<T>* bucket;
            bucket = new vector<T>[base];
            int digit = 1;
            int max = array[0].convertToInt(selectOrder[order]);
            do {
                for (int i = size - 1; i >= 0; i--) {
                    bucket[getDigit(array[i].convertToInt(selectOrder[order]), digit, base)].push_back(array[i]);
                    if (digit == 1) {
                        if (max < array[i].convertToInt(selectOrder[order]))
                            max = array[i].convertToInt(selectOrder[order]);
                    }
                }

                if (digit == 1)
                    max = getMaxDigit(max, base);

                auto ptr = array.begin();
                for (int i = base - 1; i >= 0; i--) {
                    while (bucket[i].size() > 0) {
                        *ptr = bucket[i].back();
                        ptr++;
                        bucket[i].pop_back();
                    }
                }
                digit++;
            } while (digit <= max);
        } 
    }

    bool task1()
    {
        vector<Data> database;
        string fileName;

        if (!task123_input(fileName, database))
            return 0;

        // sort and timer
        vector<Data> selectData(database), bubbleData(database);
        timing("selection: ", [&]() { selection(selectData); });
        timing("bubble: ", [&]() { bubble(bubbleData); });

        // save
        save("bubble_sort" + fileName + ".txt", bubbleData);
        save("select_sort" + fileName + ".txt", selectData);

        return 0;
    }

    bool task2()
    {
        vector<Data> database;
        string fileName;

        if (!task123_input(fileName, database))
            return 0;

        // sort and timer
        vector<Data> mergeData(database), quickData(database);
        timing("Merge: ", [&]() { merge(mergeData); });
        timing("Quick: ",
            [&]() { quick(quickData.begin(), quickData.size()); });

        // save
        save("merge_sort" + fileName + ".txt", mergeData);
        save("quick_sort" + fileName + ".txt", quickData);

        return 0;
    }

    bool task3()
    {
        vector<Data> database;
        string fileName;

        if (!task123_input(fileName, database))
            return 0;

        // sort and timer
        vector<Data> redixData(database);
        timing("redix: ", [&]() { redix(redixData, 10); });

        // save
        save("redix_sort" + fileName + ".txt", redixData);

        return 0;
    }
};

int main(int argc, char *argv[])
{
    int mode;   // 選單選項
    int result; // 指令回傳檢查
    while (true) {

        // 輸出選單
        cout << "              MENU              " << endl;
        cout << "* 0. Quit                      *" << endl;
        cout << "* 1. Selection  & Bubble Sort  *" << endl;
        cout << "* 2. Merge      & Quick Sort   *" << endl;
        cout << "* 3. Radix Sort                *" << endl;
        cout << "choice: ";

        // 輸入選擇
        cin >> mode;

        HandleFile f;

        // 判斷選擇的內容
        switch (mode) {
        case MENU_QUIT:
            return 0; // 退出

        case MENU_SELECT_BUBBLE:
            result = f.task1();
            break;

        case MENU_MERGE_QUICK:
            result = f.task2();
            break;

        case MENU_RADIX:
            result = f.task3(); // 任務3
            break;

        default:
            errorHandling("Error: Command not found!");
            continue;
        }

        // 檢查回傳值是否為successful
        if (result)
            return 1;
        else
            cout << endl;
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

