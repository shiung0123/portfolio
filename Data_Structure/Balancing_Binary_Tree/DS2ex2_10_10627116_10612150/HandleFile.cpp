// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version encoding=utf-8
#include "HandleFile.h"
#include "AVLTree.h"
#include "Data.h"
#include "Tree23.h"
#include <algorithm>

int HandleFile::numberInput(string message, string errorMsg)
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

void HandleFile::save(string saveName, vector<Data> &database)
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

void HandleFile::dropHeader(fstream &file)
{
    for (int i = 0; i < 3; ++i)
        file.ignore(numeric_limits<streamsize>::max(), '\n');
}

string HandleFile::fileInput(fstream &file, string message, string prefix)
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

bool HandleFile::task1()
{
    string fileName = fileInput(fin, "Input (201, 202, ...[0]Quit): ", "input");
    vector<Data> database;
    // if fileName == "" then quit to menu
    if (fileName != "") {
        Tree23 tree;
        Data temp;
        while (fin >> temp) { // >> overload
            if (inputSuccess) {
                tree.insert(database.size(), temp.getData(DATA_NAME));
                database.push_back(temp);
            }
        }

        // show height
        cout << "Tree height = " << tree.height() << endl;

        // show root
        vector<int> result;
        tree.getRoot(result);
        for (int i = 0; i < result.size(); i++) {
            cout << (i + 1) << ": [" << (result[i] + 1) << "] ";
            database[result[i]].print();
            cout << endl;
        }
    }
    else
        cout << "switch to menu" << endl;

    fin.close();

    return fileName == ""; // {quit: 0, continue: 1}
}

bool HandleFile::task2()
{
    string fileName = fileInput(fin, "Input (201, 202, ...[0]Quit): ", "input");
    vector<Data> database;
    // if fileName == "" then quit to menu
    if (fileName != "") {
        AVLTree tree;
        Data temp;
        while (fin >> temp) { // >> overload
            if (inputSuccess) {
                tree.insert(database.size(),
                            temp.getData(DATA_DEPARTMENT_NAME));
                database.push_back(temp);
            }
        }

        // show height
        cout << "Tree height = " << tree.height() << endl;

        // show root
        vector<int> result;
        tree.getRoot(result);
        for (int i = 0; i < result.size(); i++) {
            cout << (i + 1) << ": [" << (result[i] + 1) << "] ";
            database[result[i]].print();
            cout << endl;
        }
    }
    else
        cout << "switch to menu" << endl;

    fin.close();

    return fileName == ""; // {quit: 0, continue: 1}
}

void showResult(vector<Data> &database, vector<int> &result)
{
    for (int i = 0; i < result.size(); i++) {
        cout << (i + 1) << ": [" << (result[i] + 1) << "] ";
        database[result[i]].print();
        cout << endl;
    }
}

bool HandleFile::task3()
{
    string fileName = fileInput(fin, "Input (201, 202, ...[0]Quit): ", "input");
    vector<Data> database;
    // if fileName == "" then quit to menu
    if (fileName != "") {
        AVLTree avl;
        Tree23 tree23;
        Data temp;
        while (fin >> temp) { // >> overload
            if (inputSuccess) {
                avl.insert(database.size(), temp.getData(DATA_DEPARTMENT_NAME));
                tree23.insert(database.size(), temp.getData(DATA_NAME));
                database.push_back(temp);
            }
        }

        string name, departmentName;
        vector<int> nameResult, departmentResult;

        cout << "please make sure console is corrent encode(big5)" << endl;
        cout << "Input university name [*]:";
        cin >> name;

        cout << "Input department name [*]:";
        cin >> departmentName;

        if (name == "*" && departmentName == "*") {
            for (int i = 0; i < database.size(); i++) {
                cout << (i + 1) << ": [" << (i + 1) << "] ";
                database[i].print();
                cout << endl;
            }
        }
        else if (departmentName == "*") {
            tree23.find(nameResult, name);
            showResult(database, nameResult);
        }
        else if (name == "*") {
            avl.find(departmentResult, departmentName);
            showResult(database, departmentResult);
        }
        else {
            tree23.find(nameResult, name);
            avl.find(departmentResult, departmentName);

            vector<int> result;
            set_intersection(departmentResult.begin(), departmentResult.end(),
                             nameResult.begin(), nameResult.end(),
                             back_inserter(result));
            showResult(database, result);
        }
    }
    else
        cout << "switch to menu" << endl;

    fin.close();

    return fileName == ""; // {quit: 0, continue: 1}
}
