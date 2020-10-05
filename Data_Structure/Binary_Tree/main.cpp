// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#include <algorithm>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

// clang-format off
#define MENU_QUIT               0
#define MENU_MAXHEAP            1
#define MENU_DEAP               2

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
		cout << "Value : " << str << endl;
		return -1; // return error value
	}
}

// select column datatype must be integer
static vector<int> selectOrder = { DATA_STUDENTS };

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

		if (count != DATA_SIZE - 1)
			inputSuccess = false;
		else data.column[count] = to_string(order);

		return in;
	}

	friend ostream &operator<<(ostream &out, Data &data)
	{
		for (int i = 0; i < DATA_SIZE; i++)
			out << data.column[i] << (i < DATA_SIZE - 1 ? '\t' : '\n');

		return out;
	}

	int convertToInt(int num) { return stringToInt(column[num]); }

	bool operator>(Data &b)
	{
		for (auto i : selectOrder) {
			if (stringToInt(column[i]) > stringToInt(b.column[i]))
				return true;
			else if (stringToInt(column[i]) < stringToInt(b.column[i]))
				return false;
			// else stringToInt(column[i]) == stringToInt(b.column[i] then i++
		}
		return false;
	}

	bool operator>=(Data &b)
	{
		for (auto i : selectOrder) {
			if (stringToInt(column[i]) > stringToInt(b.column[i]))
				return true;
			else if (stringToInt(column[i]) < stringToInt(b.column[i]))
				return false;
			// else stringToInt(column[i]) == stringToInt(b.column[i] then i++
		}
		return true;
	}

	bool operator<=(Data &b)
	{
		for (auto i : selectOrder) {
			if (stringToInt(column[i]) < stringToInt(b.column[i]))
				return true;
			else if (stringToInt(column[i]) > stringToInt(b.column[i]))
				return false;
			// else stringToInt(column[i]) == stringToInt(b.column[i] then i++
		}
		return true;
	}

	bool operator<(Data &b)
	{
		for (auto i : selectOrder) {
			if (stringToInt(column[i]) < stringToInt(b.column[i]))
				return true;
			else if (stringToInt(column[i]) > stringToInt(b.column[i]))
				return false;
			// else stringToInt(column[i]) == stringToInt(b.column[i] then i++
		}
		return false;
	}

	void print()
	{
		for (auto i : selectOrder)
			cout << ' ' << column[i];
	}
	int getorder()
	{
		return convertToInt(DATA_ORDER);
	}
};

class Heap {

public:
	vector<Data> heap;
	virtual bool cmp(int &cur, int &pre) = 0;

	void bubbleup(int &cur, int &pre)
	{
		if (cmp(cur, pre)) {
			swap(heap[cur], heap[pre]);
			cur = pre;
			pre = (pre - 1) / 2;
			bubbleup(cur, pre);
		}
	}

	void insert(Data temp)
	{
		heap.push_back(temp);
		int cur = heap.size() - 1;
		int pre = (cur - 1) / 2;
		bubbleup(cur, pre);
	}

	void print_ans()
	{
		int ans[3] = { 0,  heap.size() - 1, pow(2, floor(log2(heap.size()))) - 1 };
		for (int i = 0; i < 3; i++) {
			cout << '[' << heap[ans[i]].getorder() << ']';
			heap[ans[i]].print();
			cout << endl;
		}
	}

};

class MaxHeap : public Heap {
	bool cmp(int &cur, int &pre) {
		return (heap[cur] > heap[pre]) ? true : false;
	}
};

class MinHeap : public Heap {
	bool cmp(int &cur, int &pre) {
		return (heap[cur] < heap[pre]) ? true : false;
	}
};

class Deap {
};

class HandleFile {
	fstream fin;
	fstream fout;

	MaxHeap maxheap;
	MinHeap minheap; // test

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
	bool task1() {
		string fileName = fileInput(fin, "Input (601, 602, ...[0]Quit): ", "input");
		order = 1;
		// if fileName == "" then quit to menu
		if (fileName != "") {
			Data temp;
			while (fin >> temp) // >> overload
				if (inputSuccess) {
					maxheap.insert(temp);
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

	bool task2() { return 0; }
};

int main(int argc, char *argv[])
{
	int mode;   // 選單選項
	int result; // 指令回傳檢查
	while (true) {

		// 輸出選單
		cout << "              MENU              " << endl;
		cout << "* 0.          QUIT             *" << endl;
		cout << "* 1.        MAX-HEAP           *" << endl;
		cout << "* 2.          DEAP             *" << endl;
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
