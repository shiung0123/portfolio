// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#include "Test.h"
#include "AVLTree.h"
#include "Tree23.h"
#include <iostream>

using namespace std;

void doTest(int result, string meg)
{
    cout << "[" << (result == 0 ? "done" : "fail" + to_string(result))
         << "]:" << meg << endl;
}

bool testAVLInsert_inorder(vector<string> &case1,
                           vector<string> &case1RightInorder)
{
    vector<string> result;
    AVLTree tree;
    bool success = true;

    for (auto item : case1)
        tree.insert(0, item);

    tree.inorder(result);
    if (result != case1RightInorder) {
        cout << endl;
        for (auto item : result)
            cout << item << ", ";

        cout << endl;
        success = false;
    }

    result.clear();
    tree.clear();
    return success;
}

bool testAVLInsert_preorder(vector<string> &case1,
                            vector<string> &case1RightPreorder)
{
    vector<string> result;
    AVLTree tree;
    bool success = true;

    for (auto item : case1)
        tree.insert(0, item);

    tree.preorder(result);
    if (result != case1RightPreorder) {
        cout << endl;
        for (auto item : result)
            cout << item << ", ";

        cout << endl;

        success = false;
    }

    result.clear();
    tree.clear();
    return success;
}

bool testAVLHeight_case(vector<string> &case1, int height)
{
    AVLTree tree;
    bool success = true;
    for (auto item : case1)
        tree.insert(0, item);

    if (tree.height() != height) {
        cout << tree.height() << endl;
        success = false;
    }

    tree.clear();
    return success;
}

int testAVLHeight()
{
    vector<string> result, cases;

    // double rotation case 1 RL
    cases = {"4", "2", "6", "1", "3", "5", "7", "G", "F"};
    if (!testAVLHeight_case(cases, 4))
        return 1;

    // null tree
    cases = {};
    if (!testAVLHeight_case(cases, 0))
        return 2;

    // one height tree
    cases = {"4"};
    if (!testAVLHeight_case(cases, 1))
        return 3;

    // two height tree
    cases = {"4", "5"};
    if (!testAVLHeight_case(cases, 2))
        return 4;
    return 0;
}

int testAVLInsert()
{
    vector<string> cases, rightResult;

    // double rotation case 1 RL
    cases = {"4", "2", "6", "1", "3", "5", "7", "G", "F"};
    rightResult = {"1", "2", "3", "4", "5", "6", "7", "F", "G"};
    if (!testAVLInsert_inorder(cases, rightResult))
        return 1;
    rightResult = {"4", "2", "1", "3", "6", "5", "F", "7", "G"};
    if (!testAVLInsert_preorder(cases, rightResult))
        return 2;

    // double rotation case 2 RL
    cases = {"4", "2", "6", "1", "3", "5", "7", "G", "F", "E"};
    rightResult = {"1", "2", "3", "4", "5", "6", "7", "E", "F", "G"};
    if (!testAVLInsert_inorder(cases, rightResult))
        return 3;
    rightResult = {"4", "2", "1", "3", "7", "6", "5", "F", "E", "G"};
    if (!testAVLInsert_preorder(cases, rightResult))
        return 4;

    return 0;
}

bool test23Insert_test1(vector<string> &case1)
{
    vector<string> result;
    Tree23 tree;
    bool success = true;

    for (auto item : case1)
        tree.insert(0, item);

    result.clear();
    tree.clear();
    return success;
}

int test23Insert()
{
    vector<string> cases, rightResult;

    // add case
    cases = {"1", "2", "3", "4", "5", "9'", "8", "7", "6", "A"};
    test23Insert_test1(cases);

    cases = {"1", "2", "3", "4", "7", "5", "8"};
    test23Insert_test1(cases);

    cases = {"8", "5", "7", "4", "3", "2", "1"};
    test23Insert_test1(cases);

    // leftmost case
    cases = {"4", "5", "6", "2", "3"};
    test23Insert_test1(cases);
    return 1;
}

void debug()
{
    cout << "==== debug ==== " << endl;
    doTest(testAVLInsert(), "AVL Insert");
    doTest(testAVLHeight(), "AVL Height");
    doTest(test23Insert(), "23 Insert");
}
