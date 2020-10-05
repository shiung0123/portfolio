// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#include <algorithm>
#include <cmath>
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
#define MENU_MAXHEAP            1
#define MENU_DEAP               2
#define MENU_MINMAX             3

#define DATA_SIZE               12
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
#define DATA_ORDER              11
// clang-format on

static bool inputSuccess;
static int order;
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
        cout << "Value : \"" << str << "\"" << endl;
        return -1; // return error value
    }
}

// select column datatype must be integer
static vector<int> selectOrder = { DATA_STUDENTS };

class Data {
    string column[DATA_SIZE];
    int cache[DATA_SIZE];
    void updateCache()
    {
        for (auto i : selectOrder)
            cache[i] = stringToInt(column[i]);
    }

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

        if (count != DATA_SIZE - 1)
            inputSuccess = false;
        else
            data.column[count] = to_string(order);

        data.updateCache();
        return in;
    }

    friend ostream &operator<<(ostream &out, Data &data)
    {
        for (int i = 0; i < DATA_SIZE; i++)
            out << data.column[i] << (i < DATA_SIZE - 1 ? '\t' : '\n');

        return out;
    }

    string getData(int index) const { return column[index]; }

    void setData(int index, string data)
    {
        column[index] = data;
        updateCache();
    }

    int convertToInt(int num) { return stringToInt(column[num]); }

    bool operator>(Data &b)
    {
        for (auto i : selectOrder) {
            if (cache[i] > b.cache[i])
                return true;
            else if (cache[i] < b.cache[i])
                return false;
            // else cache[i] == b.cache[i] then i++
        }
        return false;
    }

    bool operator>=(Data &b)
    {
        for (auto i : selectOrder) {
            if (cache[i] > b.cache[i])
                return true;
            else if (cache[i] < b.cache[i])
                return false;
            // else cache[i] == b.cache[i] then i++
        }
        return true;
    }

    bool operator<=(Data &b)
    {
        for (auto i : selectOrder) {
            if (cache[i] < b.cache[i])
                return true;
            else if (cache[i] > b.cache[i])
                return false;
            // else cache[i] == b.cache[i] then i++
        }
        return true;
    }

    bool operator<(Data &b)
    {
        for (auto i : selectOrder) {
            if (cache[i] < b.cache[i])
                return true;
            else if (cache[i] > b.cache[i])
                return false;
            // else cache[i] == b.cache[i] then i++
        }
        return false;
    }

    void print()
    {
        for (auto i : selectOrder)
            cout << ' ' << column[i];
    }

    void println()
    {
        for (auto i : selectOrder)
            cout << ' ' << column[i] << endl;
    }

    int getorder() { return convertToInt(DATA_ORDER); }
};

#define preNode(i) (i - 1) / 2
#define grandNode(i) (i - 3) / 4
#define rightNode(i) 2 * i + 2
#define leftNode(i) 2 * i + 1
#define NULL_NODE -1

class Heap {
public:
    virtual bool cmp(Data &cur, Data &pre) = 0;

private:
    bool cmp(int &cur, int &pre) { return cmp(heap[cur], heap[pre]); }

protected:
    vector<Data> heap;

public:
    bool exist(int index)
    {
        // heap.size() is unsigend int
        // 0 - 1 underflow
        return 0 <= index && index <= ((int)heap.size()) - 1;
    }

    int size() { return heap.size(); }
    int root() { return 0; }
    int bottom() { return heap.size() - 1; }
    int leftbottom() { return pow(2, floor(log2(heap.size()))) - 1; }
    Data &operator[](int index) { return heap[index]; }

    int mostChild(int cur)
    {
        int left = leftNode(cur);
        int right = rightNode(cur);

        // return size if cur no child
        if (!exist(left) && !exist(right))
            return NULL_NODE;

        // if right not exist, left is largest
        if (!exist(right))
            return left;

        // return most child
        if (cmp(right, left))
            return right;
        else
            return left;
    }

    int leastChild(int cur)
    {
        int left = leftNode(cur);
        int right = rightNode(cur);

        // return size if cur no child
        if (!exist(left) && !exist(right))
            return NULL_NODE;

        // if right not exist, left is least
        if (!exist(right))
            return left;

        // return least child
        if (cmp(right, left))
            return left;
        else
            return right;
    }

    // Reheap up from cur to root
    void reheapUp(int cur, int root = 0)
    {
        int pre = preNode(cur);

        // Compare and check is arrival root
        while (cur > root && cmp(cur, pre)) {
            swap(heap[cur], heap[pre]);
            cur = pre;

            // iterate until arrival root
            pre = preNode(cur);
        }
    }

    // Reheap down from root to leaf
    int reheapDown(int cur, int root = 0)
    {
        int child = mostChild(cur);

        // Compare and check is arrival leaf
        while (exist(child) && cmp(child, cur)) {
            swap(heap[cur], heap[child]);
            cur = child;

            // iterate until arrival left
            child = mostChild(cur);
        }
        return cur;
    }

    Heap() {}
    Heap(vector<Data> array) : heap(array) {}

    // rebuld from array
    void rebuild()
    {
        for (int i = heap.size() / 2; i >= 0; i--)
            reheapDown(i);
    }

    void push(Data temp)
    {
        heap.push_back(temp);
        // Reheap last item
        reheapUp(bottom());
    }

    void push_back(Data temp) { heap.push_back(temp); }

    // pop root
    void pop()
    {
        if (heap.size()) {
            swap(heap[root()], heap[bottom()]);
            heap.pop_back();
            reheapDown(root());
        }
    }

    void pop_back() { heap.pop_back(); }

    void print_ans()
    {
        if (heap.size()) {
            string name[3] = { "root", "bottom", "left bottom" };
            int ans[3] = { 0, bottom(), leftbottom() };
            for (int i = 0; i < 3; i++) {
                cout << name[i] << ":[" << heap[ans[i]].getorder() << ']';
                heap[ans[i]].println();
            }
        }
        else
            cout << "It's empty" << endl;
    }

    // debug print
    void print()
    {
        cout << "[";
        for (auto i : heap)
            cout << i.getData(selectOrder[0]) << ", ";
        if (heap.size() > 0)
            cout << "\b\b";
        cout << "]" << endl;
    }
};

class MaxHeap : public Heap {
    bool cmp(Data &cur, Data &pre) { return cur > pre; }

public:
    MaxHeap() : Heap() {}
    MaxHeap(vector<Data> array) : Heap(array) {}
};

class MinHeap : public Heap {
    bool cmp(Data &cur, Data &pre) { return cur < pre; }

public:
    MinHeap() : Heap() {}
    MinHeap(vector<Data> array) : Heap(array) {}
};

class MinMaxHeap : public Heap {

private:
    // min
    bool isMin(int cur)
    {
        return ((int)floor(log2(cur + 1))) % 2 == 0;
    } // right~

    bool cmpMinMax(int cur, int pre)
    {
        // cur node , pre root
        // min level pre (max) 小於新的值
        // max level pre (min) 大於新的值
        // true swap
        return isMin(cur) ? heap[cur] > heap[pre] : heap[cur] < heap[pre];
    } // right

    bool cmp(Data &cur, Data &pre) { return cur > pre; }

    bool cmp1(int &cur, int &pre)
    {
        // min level cur > pre
        // max level cur < pre
        // true swap
        return isMin(cur) ? heap[cur] > heap[pre] : heap[cur] < heap[pre];
    } // right

public:
    MinMaxHeap() : Heap() {}
    MinMaxHeap(vector<Data> array) : Heap(array) {}

    void reheapUp(int cur, int root = 0)
    {
        int pre = grandNode(cur);

        // Compare and check is arrival root
        if (isMin(cur)) {
            while (pre >= 0 && heap[cur] < heap[pre]) {
                swap(heap[cur], heap[pre]);
                cur = pre;

                // iterate until arrival root
                pre = grandNode(cur);
            }
        }
        else {
            // max
            while (pre >= 1 && heap[cur] > heap[pre]) {
                swap(heap[cur], heap[pre]);
                cur = pre;

                // iterate until arrival root
                pre = grandNode(cur);
            }
        }
    }

    void push(Data temp)
    {
        heap.push_back(temp);
        // swap if not currect
        int cur = bottom();

        if (cmpMinMax(cur, preNode(cur))) {
            swap(heap[cur], heap[preNode(cur)]);

            // Reheap swap item
            cur = preNode(cur);
        }

        reheapUp(cur);
    }

    void print_ans()
    {
        if (heap.size()) {
            cout << "< min-max heap >" << endl;
            string name[3] = { "root", "bottom", "left bottom" };
            int ans[3] = { 0, bottom(), leftbottom() };
            for (int i = 0; i < 3; i++) {
                cout << name[i] << ":[" << heap[ans[i]].getorder() << ']';
                heap[ans[i]].println();
            }
        }
        else
            cout << "It's empty" << endl;
    }
};

class Deap {
    MaxHeap maxheap;
    MinHeap minheap;
    bool isminheap = true;

    bool isfull(int size)
    {
        return size == 1 ? false
            : pow(2, floor(log2(size))) == pow(2, log2(size));
    }

    int bottom() { return isminheap ? minheap.size() - 1 : maxheap.size() - 1; }
    int leftbottom() { return pow(2, floor(log2(minheap.size()))) - 1; }
    Heap &bottomHeap() { return isminheap ? (Heap &)minheap : (Heap &)maxheap; }
    Heap &bottomHeapN()
    {
        return !isminheap ? (Heap &)minheap : (Heap &)maxheap;
    }

    // Calculate the corresponding node
    int corresNode(int cur, Heap &side)
    {
        if (side.size() == 0)
            return -1;
        if (!side.exist(cur)) {
            // two side diff must be one // while (!side.exist(cur))
            cur = preNode(cur);
        }

        return cur;
    }

    // select filling heap
    void updateSideStage()
    {
        if (isminheap && isfull(minheap.size() + 1))
            isminheap = false;
        else if (!isminheap && isfull(maxheap.size() + 1))
            isminheap = true;
    }

public:
    int size() { return minheap.size() + maxheap.size(); }

    void push(Data temp)
    {
        // call updateSideStage() to select filling heap
        updateSideStage();

        // curHeap is filling heap, corresHeap is correspond
        Heap &curHeap = bottomHeap();
        Heap &corresHeap = bottomHeapN();
        int corres = corresNode(curHeap.size(), corresHeap);

        // if temp should pushing to the corresponding heap, swap data and
        // reheap
        if (corres >= 0 && corresHeap.cmp(temp, corresHeap[corres])) {
            swap(temp, corresHeap[corres]);
            corresHeap.reheapUp(corres);
        }

        curHeap.push(temp);
    }

    // use bottom node to replace root node and remove bottom node
    void replaceRoot(Heap &heap)
    {
        // using bottom of bottomheap to replace root of heap
        Heap &bottomheap = bottomHeap();
        swap(heap[heap.root()], bottomheap[bottomheap.bottom()]);
        bottomheap.pop_back();

        // max or min heap is changed, so update side stage
        updateSideStage();
        // when maxheap is empty, it may switch to minheap
        // function updateSideStage() can't work because it also use in insert
        if (!maxheap.size())
            isminheap = true;
    }

    // pop and return min data
    Data pop_min()
    {
        Data result;
        if (minheap.size()) {
            result = minheap[minheap.root()];

            // using bottom of bottomheap to replace root of minheap
            replaceRoot(minheap);

            // minheap is modified so we reheap
            int cur = minheap.reheapDown(minheap.root());

            // if cur is leaf and this heap is not empty and correspond is
            // exist, we would check them and swap to make deap heap has a sense
            if (!minheap.exist(leftNode(cur)) && minheap.size()) {
                int corres = corresNode(cur, maxheap);
                if (corres >= 0 && minheap[cur] > maxheap[corres]) {
                    swap(minheap[cur], maxheap[corres]);
                    maxheap.reheapUp(corres);
                }
            }
        }

        return result;
    }

    // pop and return max data
    Data pop_max()
    {
        Data result;
        // pop from max heap
        if (maxheap.size()) {
            result = maxheap[maxheap.root()];

            // using bottom of bottomheap to replace root of maxheap
            replaceRoot(maxheap);

            // maxheap is modified so we reheap
            int cur = maxheap.reheapDown(maxheap.root());

            // if cur is leaf and this heap is not empty and correspond is
            // exist, we would check them and swap to make deap heap has a sense
            if (!maxheap.exist(leftNode(cur)) && maxheap.size()) {
                int corres = corresNode(cur, minheap);
                if (maxheap[cur] < minheap[corres]) {
                    swap(maxheap[cur], minheap[corres]);
                    minheap.reheapUp(corres);
                }
                int child = minheap.leastChild(corres);
                // be sure that correspond children are smaller than cur
                if (child >= 0 && maxheap[cur] < minheap[child]) {
                    swap(maxheap[cur], minheap[child]);
                }
            }
        }
        // pop from minheap
        else {
            result = minheap[minheap.root()];

            // only minheap has one item
            minheap.pop_back();
            updateSideStage();
        }

        return result;
    }

    void print_ans()
    {
        if (minheap.size()) {
            int ans[2] = { bottom(), leftbottom() };

            // println bottom data
            Heap &bottomheap = bottomHeap();
            cout << "bottom"
                << ":[" << bottomheap[ans[0]].getorder() << ']';
            bottomheap[ans[0]].println();

            // left bottom always at minheap
            cout << "left bottom"
                << ":[" << minheap[ans[1]].getorder() << ']';
            minheap[ans[1]].println();
        }
        else
            cout << "It's empty" << endl;
    }

    void print()
    {
        cout << "{" << endl;
        cout << "    minheap:";
        minheap.print();
        cout << "    maxheap:";
        maxheap.print();
        cout << "}";
    }
};

class HandleFile {
    fstream fin;
    fstream fout;

    MaxHeap maxheap;
    MinHeap minheap; // test
    Deap deap;
    MinMaxHeap minmaxheap;

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

public:
    bool task1()
    {
        string fileName =
            fileInput(fin, "Input (101, 102, ...[0]Quit): ", "input");
        order = 1;
        // if fileName == "" then quit to menu
        if (fileName != "") {
            Data temp;
            while (fin >> temp) // >> overload
                if (inputSuccess) {
                    maxheap.push(temp);
                    order++;
                }

            maxheap.print_ans();
            // print out something
        }
        else {
            cout << "switch to menu" << endl;
        }

        fin.close();

        return fileName == ""; // {quit: 0, continue: 1}
    }

    bool task2()
    {
        string fileName =
            fileInput(fin, "Input (101, 102, ...[0]Quit): ", "input");
        order = 1;
        // if fileName == "" then quit to menu
        if (fileName != "") {
            Data temp;
            while (fin >> temp) // >> overload
                if (inputSuccess) {
                    deap.push(temp);
                    order++;
                }

            deap.print_ans();
            // print out something
        }
        else {
            cout << "switch to menu" << endl;
        }

        fin.close();

        return fileName == ""; // {quit: 0, continue: 1} }
    }

    bool task3()
    {
        string fileName =
            fileInput(fin, "Input (101, 102, ...[0]Quit): ", "input");
        order = 1;
        // if fileName == "" then quit to menu
        if (fileName != "") {
            Data temp;
            while (fin >> temp) // >> overload
                if (inputSuccess) {
                    minmaxheap.push(temp);
                    order++;
                }

            minmaxheap.print_ans();
            // print out something
        }
        else {
            cout << "switch to menu" << endl;
        }

        fin.close();

        return fileName == ""; // {quit: 0, continue: 1} }
    }
};

int testDeap()
{
    cout << endl << "== testDeap ==" << endl << endl;
    int key[] = { 6, 3, 5, 9, 2, 10, 13, 1, 23, 4 };
    int size = sizeof(key) / sizeof(int);
    vector<Data> array(size);
    for (int i = 0; i < size; i++)
        array[i].setData(selectOrder[0], to_string(key[i]));

    Deap heap1;
    for (int i = 0; i < size; i++)
        heap1.push(array[i]);
    heap1.print();

    cout << endl << "== pop max ==" << endl << endl;
    while (heap1.size() > 0)
        cout << heap1.pop_max().getData(selectOrder[0])
        << " size:" << heap1.size() << endl,
        heap1.print();

    for (int i = 0; i < size; i++)
        heap1.push(array[i]);
    heap1.print();

    cout << endl << "== pop min ==" << endl << endl;
    while (heap1.size() > 0)
        cout << heap1.pop_min().getData(selectOrder[0])
        << " size:" << heap1.size() << endl,
        heap1.print();

    return false;
}

#define size 6
void testMaxHeap()
{
    cout << endl << "== testMaxHeap ==" << endl << endl;
    int key[size] = { 6, 3, 5, 9, 2, 10 };
    vector<Data> array(size);
    for (int i = 0; i < size; i++)
        array[i].setData(selectOrder[0], to_string(key[i]));

    MaxHeap heap1(array);
    heap1.rebuild();
    heap1.print();
}

void testMinHeap()
{
    cout << endl << "== testMinHeap ==" << endl << endl;
    int key[size] = { 6, 3, 5, 9, 2, 10 };
    vector<Data> array(size);
    for (int i = 0; i < size; i++)
        array[i].setData(selectOrder[0], to_string(key[i]));

    MinHeap heap1(array);
    heap1.rebuild();
    heap1.print();
}

void debug()
{
    // Before finding a batter testing tool,
    // please not remove three testing
    // define "DEBUG1111" to enter testing
    testMaxHeap();
    testMinHeap();
    testDeap();
}

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
        cout << "* 1.        MAX-HEAP           *" << endl;
        cout << "* 2.          DEAP             *" << endl;
        cout << "* 3.         MINMAX            *" << endl;
        cout << "choice: ";

        // 輸入選擇
        cin >> mode;

        HandleFile f;

        // 判斷選擇的內容
        switch (mode) {
        case MENU_QUIT:
            return 0; // 退出

        case MENU_MAXHEAP:
            result = f.task1();
            break;

        case MENU_DEAP:
            result = f.task2();
            break;

        case MENU_MINMAX:
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