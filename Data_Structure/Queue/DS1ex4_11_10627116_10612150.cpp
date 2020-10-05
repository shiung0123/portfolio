// 第11組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <iomanip>
#include <ctime>
#include <functional>

using namespace std;

#define MENU_QUIT                    0
#define MENU_SHELL_SORT              1
#define MENU_SIMULATE_ONE_QUEUE      2
#define MENU_SIMULATE_TWO_QUEUES     3
#define MENU_SIMULATE_SOME_QUEUES    4

// sort column
#define DATA_SIZE             4
#define DATA_OID              0
#define DATA_ARRIVAL          1
#define DATA_DEURATION        2
#define DATA_TIMEOUT          3

// cancel & timeout cloumn
#define DATA_OID              0
#define DATA_CID              1
#define DATA_DELAY            2
#define DATA_ABORT_DEPARTURE  3

#define endchar(i, end) (i < end ? '\t' : '\n')

static bool inputSuccess;
void errorHandling(const string &message);

string getOnlyDigits(string str)
{
    string tmp = "";
    for (char c : str)
        if (isdigit(c)) tmp += c;

    return tmp;
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
        string temp = "ERROR : stoi( " + str + " ) error!";
        errorHandling(temp);
        return -1; // return error value
    }
}

// sort & cancel & timeout DATA use
class Data {
public:
    int column[DATA_SIZE];

    Data() {}

    // used to creat cancel & timeout cloumn
    Data(int num1, int num2, int num3, int num4)
    {
        column[DATA_OID] = num1;
        column[DATA_CID] = num2;
        column[DATA_DELAY] = num3;
        column[DATA_ABORT_DEPARTURE] = num4;
    }

    int operator[](int i) const
    {
        return column[i];
    }

    int &operator[](int i)
    {
        return column[i];
    }

    friend bool operator<(const Data &i, const Data &j)
    {
        if (j[DATA_ARRIVAL] > i[DATA_ARRIVAL])
            return true;
        else if (j[DATA_ARRIVAL] < i[DATA_ARRIVAL])
            return false;

        // when j[DATA_ARRIVAL] == i[DATA_ARRIVAL]
        else if (j[DATA_OID] > i[DATA_OID])
            return true;
        else
            return false;
    }

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
                // change to int
                int num = stringToInt(temp);
                if (num == -1) inputSuccess = false;
                else data[count++] = num;
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
            out << data[i] << endchar(i, DATA_SIZE - 1);

        return out;
    }

    void print(fstream &fout, int skipcloumn) {
        for (int i = 0; i < DATA_SIZE; i++)
            if (i != skipcloumn)
                fout << column[i] << endchar(i, DATA_SIZE - 1);
    }
};

class LinkedList {
private:

    typedef struct node {
        Data data;
        node *next, *prev;
    } node;

    node *firstNode, *endNode;

public:

    LinkedList()
    {
        firstNode = endNode = nullptr;
    }

    void pop()
    {
        // mark remove node
        node *rmNode = firstNode;

        if (rmNode == nullptr) {  // nothing
            throw "LinkedList empty!!";

        }
        else if (firstNode == endNode) {           // only one node
            endNode = firstNode = nullptr;

        }
        else {                             // not only one
         // replace end node with prev
            firstNode = firstNode->next;
            // unlink
            firstNode->prev = nullptr;
        }

        // free memory
        delete rmNode;
        rmNode = nullptr;
    }

    void pop_back()
    {
        // mark remove node
        node *rmNode = endNode;

        if (rmNode == nullptr) {  // nothing
            throw "LinkedList empty!!";

        }
        else if (firstNode == endNode) {           // only one node
            endNode = firstNode = nullptr;

        }
        else {                             // not only one
         // replace end node with prev
            endNode = rmNode->prev;
            // unlink
            endNode->next = nullptr;
        }

        // free memory
        delete rmNode;
        rmNode = nullptr;
    }

    void push_back(const Data d)
    {
        node *newNode = new node;
        newNode->data = d;
        newNode->next = nullptr;

        if (firstNode == nullptr) {  // no node
            firstNode = endNode = newNode;
            newNode->prev = nullptr;

        }
        else {              // have node
            // set new node prev
            newNode->prev = endNode;
            // set prev node link to newNode
            endNode->next = newNode;
            // ptr to new
            endNode = newNode;
        }
    }

    int size()
    {
        int count = 0;
        node *cur = firstNode;   // current

        while (cur != nullptr)
            cur = cur->next, count++;

        return count;
    }

    bool empty()
    {
        return firstNode == nullptr;
    }

    Data front()
    {
        return firstNode->data;
    }

    Data back()
    {
        return endNode->data;
    }

    void print()
    {
        // current
        node *cur = firstNode;

        while (cur != nullptr) {
            cout << cur->data;
            cout << (cur != endNode ? ',' : '\n');
            cur = cur->next;
        }

        cout.flush();
    }

    void clear()
    {
        node *cur = firstNode;   // current
        firstNode = nullptr;
        while (cur != nullptr) {
            if (cur->prev != nullptr)
                delete cur->prev;

            cur = cur->next;
        }
    }

    // range based for
    class iterator {
        node *current;

    public:
        iterator(node *node)
        {
            current = node;
        }

        Data operator*()
        {
            return current->data;
        }

        bool operator!=(const iterator &it2)
        {
            return current != it2.current;
        }

        iterator &operator++()
        {
            current = current->next;
            return *this;
        }
    };

    iterator begin()
    {
        return iterator(firstNode);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

};

class Queue {
    LinkedList queueList;
public:

    int size()
    {
        return queueList.size();
    }

    bool empty()
    {
        return queueList.empty();
    }

    Data front()
    {
        return queueList.front();
    }

    void pop()
    {
        queueList.pop();
    }

    void push(Data c)
    {
        queueList.push_back(c);
    }

    void print()
    {
        queueList.print();
    }

    void clear()
    {
        queueList.clear();
    }

};

class Chef : public Queue {
    int order;
    int idleTime;
public:
    Chef() : idleTime(0), order(0) {}

    int getOrder()
    {
        return order;
    }

    void setOrder(int o)
    {
        order = o;
    }

    int getIdleTime()
    {
        return idleTime;
    }

    void setIdleTime(int time)
    {
        idleTime = time;
    }
};

class Manager {
    vector<Chef> chefs;
    vector<Data> abort;
    vector<Data> timeout;
    vector<Data> done;

    int total;
    int delay_count;

    void handleOrder(Chef &chef, const Data &data)
    {
        if (chef.getIdleTime() < data[DATA_ARRIVAL])
            chef.setIdleTime(data[DATA_ARRIVAL]); // time jump

        int final_time = chef.getIdleTime() + data[DATA_DEURATION];
        int delay_time = chef.getIdleTime() - data[DATA_ARRIVAL];
        int cid = chef.getOrder();

        if (data[DATA_TIMEOUT] <= chef.getIdleTime())
            logOrder(abort, cid, data, delay_time, chef.getIdleTime());
        else {
            if (data[DATA_TIMEOUT] < final_time)
                logOrder(timeout, cid, data, delay_time, final_time);
            else
                logOrder(done, cid, data, delay_time, final_time);

            chef.setIdleTime(final_time);
        }
    }

    void logOrder(vector<Data> &database, int id, const Data &data, int delay_time, int time)
    {
        database.push_back(
            Data(data[DATA_OID], id, delay_time, time));

        delay_count += delay_time;
    }

public:
    Manager(int num) : chefs(num), delay_count(0), total(0)
    {
        for (int i = 0; i < num; i++)
            chefs[i].setOrder(i + 1);
    }

    void handleQueue(const Data *data = nullptr)
    {
        while (true) {
            int min = -1;
            for (int i = 0; i < chefs.size(); i++) {
                if (!chefs[i].empty()) {
                    // check data only data is not NULL when
                    if (data == nullptr) {
                        // no select
                        if (min == -1)
                            min = i;
                        else if (chefs[i].getIdleTime() < chefs[min].getIdleTime())
                            min = i;
                        else if (chefs[i].getIdleTime() == chefs[min].getIdleTime() &&
                            chefs[i].front()[DATA_ARRIVAL] < chefs[min].getIdleTime())
                            min = i;
                    }
                    else {
                        while (!chefs[i].empty() &&
                            chefs[i].getIdleTime() <= data->column[DATA_ARRIVAL])
                        {
                            handleOrder(chefs[i], chefs[i].front());
                            chefs[i].pop();
                        }
                    }
                }
            }

            if (min == -1)
                break;

            handleOrder(chefs[min], chefs[min].front());
            chefs[min].pop();
        }
    }

    void action(const Data &data)
    {
        total++;

        handleQueue(&data);

        // handle order immediately
        for (int i = 0; i < chefs.size(); i++) {
            if (chefs[i].empty() && chefs[i].getIdleTime() <= data[DATA_ARRIVAL]) {
                handleOrder(chefs[i], data);
                return;
            }
        }

        // push to minimum size queue
        int min = -1;
        for (int i = 0; i < chefs.size(); i++) {
            if (chefs[i].size() < 3 &&
                (min == -1 || chefs[i].size() < chefs[min].size()))
            {
                min = i;
            }
        }

        if (min != -1) {
            // push order
            chefs[min].push(data);
        }
        else {
            // abort order
            logOrder(abort, 0, data, 0, data[DATA_ARRIVAL]);
        }
    }

    vector<Data> &getAbort()
    {
        return abort;
    }

    vector<Data> &getTimeout()
    {
        return timeout;
    }

    vector<Data> &getDone()
    {
        return done;
    }

    int getTotalDelay()
    {
        return delay_count;
    }

    int getFailOrder()
    {
        return getAbort().size() + getTimeout().size();
    }

    int getTotal()
    {
        return total;
    }
};

class HandleFile {
    fstream fin;
    fstream fout;

    void dropHeader(fstream &file, int num)
    {
        for (int i = 0; i < num; ++i)
            file.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void save(string saveName, vector<Data> &database, string column[])
    {
        // closs all file
        if (fin.is_open())
            fin.close();

        fout.open(saveName, ios::out | ios::trunc);

        for (int i = 0; i < DATA_SIZE; i++)
            fout << column[i] << endchar(i, DATA_SIZE - 1);

        for (int i = 0; i < database.size(); i++)
            fout << database[i];         // << overload

        fout.close();
    }

    void save(string saveName, vector<Data> &database, string title,
        string column[], int skipcolumn)
    {
        // closs all file
        if (fin.is_open())
            fin.close();

        fout.open(saveName, ios::out | ios::app);

        fout << "\t[" << title << "]\n" << '\t';
        for (int i = 0; i <= column->size(); i++)
            if (i != skipcolumn)
                fout << column[i] << endchar(i, column->size());

        for (int i = 0; i < database.size(); i++) {
            fout << '[' << i + 1 << "]\t";

            // print to database[i] to fout
            database[i].print(fout, skipcolumn);
        }

        fout.close();
    }

    string fileInput(fstream &file, const string &message, const string &prefix)
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

            // if (prefix == "input" && prefix == "sort")
            dropHeader(file, 1);

            return fileName;
        }
    }

    bool input(string &fileName, const string &prefix)
    {
        fileName = fileInput(fin, "Input (401, 402, ...[0]Quit): ", prefix);

        // if fileName == "" then quit to menu
        return fileName != "";  // {quit: 0, continue: 1}
    }

    // use in task1
    void shell_sort(vector<Data> &arr)
    {
        int len = arr.size();
        for (int d = len / 2; d >= 1; d /= 2) {
            for (int i = d; i < len; i++) {
                Data temp = arr[i];
                int j = i - d;
                for (; j >= 0 && temp < arr[j]; j -= d)
                    arr[j + d] = arr[j];

                arr[j + d] = temp;
            }
        }
    }

    typedef const function<void()> timerHandler;
    void timing(const string &display, timerHandler doing)
    {
        clock_t t = clock();
        doing();
        cout << display << (clock() - t) << " ms" << endl;
    }

    // use in task2_3
    void summary(string saveName, Manager &manager)
    {
        if (fin.is_open())
            fin.close();

        fout.open(saveName, ios::out | ios::app);

        // display delay time
        fout << "[Total Delay]" << endl;
        fout << manager.getTotalDelay() << " min." << endl;

        // count fail order
        fout << "[Failure Percentage]" << endl;
        fout << fixed << setprecision(2) <<
            manager.getFailOrder() / float(manager.getTotal()) * 100 <<
            " %" << endl;
    }

    // use in task4
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

public:

    bool task1()
    {
        vector<Data> database;
        string fileName;

        // is quit task1?
        if (!input(fileName, "input")) {
            cout << "switch to menu" << endl;
            return 0;
        }

        // read
        timing("Read File: ", [&]() {
            Data temp;
            while (fin >> temp)     // >> overload
                if (inputSuccess) database.push_back(temp);
        });

        // sort
        timing("Sort File: ", [&]() {shell_sort(database); });

        // save
        timing("Save File: ", [&]() {
            string column[4] = {
                "OID", "Arrival", "Duration", "TimeOut"
            };
            save("sort" + fileName + ".txt", database, column);
        });

        return 0;
    }

    bool task2_3(int num, const string &prefix)
    {
        Manager manager(num);
        string fileName;

        // is quit task2?
        if (!input(fileName, "sort")) {
            cout << "switch to menu" << endl;
            return 0;
        }

        // read
        Data temp;
        while (fin >> temp) {     // >> overload
            if (inputSuccess)
                manager.action(temp);
        }

        manager.handleQueue();

        // save file
        string saveName = prefix + fileName + ".txt";

        // clean old file
        fout.open(saveName, ios::out | ios::trunc);
        fout.close();

        // one queue also print "CID"
        string column[4] = {
            "OID", "CID", "Delay", "Abort"
        };

        // abort
        if (num == 1)
            save(saveName, manager.getAbort(), "Abort List", column, DATA_CID);
        else
            save(saveName, manager.getAbort(), "Abort List", column, -1);

        // timeout
        column[3] = "Departure";
        if (num == 1)
            save(saveName, manager.getTimeout(), "Timeout List", column, DATA_CID);
        else
            save(saveName, manager.getTimeout(), "Timeout List", column, -1);

        // done
        column[3] = "Departure";
        if (num == 1)
            save(saveName, manager.getDone(), "Done List", column, DATA_CID);
        else
            save(saveName, manager.getDone(), "Done List", column, -1);

        summary(saveName, manager);

        return 0;
    }

    bool task4(const string &prefix)
    {
        int num = numberInput("Input the positive number of queues(chefs): ", "Number out of range!");
        Manager manager(num);
        string fileName;

        // is quit task2?
        if (!input(fileName, "sort")) {
            cout << "switch to menu" << endl;
            return 0;
        }

        // read
        Data temp;
        while (fin >> temp) {     // >> overload
            if (inputSuccess)
                manager.action(temp);
        }

        manager.handleQueue();

        // save file
        string saveName = prefix + fileName + ".txt";

        // clean old file
        fout.open(saveName, ios::out | ios::trunc);
        fout.close();

        // one queue also print "CID"
        string column[4] = {
            "OID", "CID", "Delay", "Abort"
        };

        // abort
        if (num == 1)
            save(saveName, manager.getAbort(), "Abort List", column, DATA_CID);
        else
            save(saveName, manager.getAbort(), "Abort List", column, -1);

        // timeout
        column[3] = "Departure";
        if (num == 1)
            save(saveName, manager.getTimeout(), "Timeout List", column, DATA_CID);
        else
            save(saveName, manager.getTimeout(), "Timeout List", column, -1);

        summary(saveName, manager);

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
        cout << "* 0. Quit                      *" << endl;
        cout << "* 1. SHELL_SORT                *" << endl;
        cout << "* 2. Simulate one queue        *" << endl;
        cout << "* 3. Simulate two queues       *" << endl;
        cout << "* 4. Simulate some queues      *" << endl;
        cout << "choice: ";

        // 輸入選擇
        cin >> mode;

        HandleFile f;

        // 判斷選擇的內容
        switch (mode) {
        case MENU_QUIT:
            return 0;               // 退出

        case MENU_SHELL_SORT:
            result = f.task1();       // 任務1
            break;

        case MENU_SIMULATE_ONE_QUEUE:
            result = f.task2_3(1, "one");       // 任務2
            break;

        case MENU_SIMULATE_TWO_QUEUES:
            result = f.task2_3(2, "two");       // 任務3
            break;

        case MENU_SIMULATE_SOME_QUEUES:
            result = f.task4("more");       // 任務4
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

void errorHandling(const string &message)
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
