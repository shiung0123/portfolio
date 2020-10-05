// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#include "Tree23.h"
#include <algorithm>

bool Tree23::Node::isLeaf()
{
    if (subtree[0] == NULL)
        return true;
    else
        return false;
}

// connect two node
void Tree23::Node::connect(Node *sub, int subIndex)
{
    subtree[subIndex] = sub;
    if (sub != NULL)
        sub->pre = this;
}

Tree23::Node::Node(Tree23::Node *getPre)
{
    // set NULL
    pre = getPre;
    for (int i = 0; i < TREE23_SIZE + 1; i++)
        subtree[i] = NULL;
    size = 0;
}

void Tree23::Node::addKey(const Data &getData)
{
    data[size].assign(getData);
    size++;
}

void Tree23::Node::addKey(int id, const string &key)
{
    data[size].key = key;
    data[size].id.push_back(id);
    size++;
}

// count number of Node
int Tree23::Node::isNodeN() { return size + 1; }

void Tree23::Node::remove(int index)
{
    for (int i = index; i < size - 1; i++)
        data[i] = data[i + 1];
}

void Tree23::Data::assign(const Tree23::Data &data)
{
    key = string(data.key);    // add a key
    id = vector<int>(data.id); // add a ids
}

// merge by order
void Tree23::Node::merge(Node *node)
{
    if (size == 0) {                   // empty node
        data[0].assign(node->data[0]); // add a key
        connect(node->subtree[0], 0);
        connect(node->subtree[1], 1);
    }
    else { // between
        // find insert index
        int index = 0;
        while (index < size && data[index].key < node->data[0].key)
            index++;

        for (int i = size; i > index; i--) { // shift
            data[i] = data[i - 1];
            subtree[i + 1] = subtree[i];
        }

        // merge
        data[index].assign(node->data[0]); // add a key
        connect(node->subtree[0], index);
        connect(node->subtree[1], index + 1);
    }

    size++;
}

Tree23::Node *Tree23::split(Tree23::Node *node)
{
    // node is exist and full
    if (node->size > TREE23_KEY_SIZE) {
        if (node == root) { // is root
            // create new root
            Node *newRoot = new Node(NULL);
            newRoot->connect(root, 0); // set pre
            root = newRoot;            // replace root
        }

        Node *pre = node->pre;
        Node *left = new Node(pre), *right = new Node(pre);

        // key1
        left->addKey(node->data[0]);
        left->connect(node->subtree[0], 0);
        left->connect(node->subtree[1], 1);

        // key3
        right->addKey(node->data[2]);
        right->connect(node->subtree[2], 0);
        right->connect(node->subtree[3], 1);

        // merge node and pre_node
        node->data[0].assign(node->data[1]);
        node->subtree[0] = left;
        node->subtree[1] = right;
        node->size = 1;

        pre->merge(node);

        // remove node
        delete node;

        // recursive
        split(pre);
    }

    return node;
}

// get next node
Tree23::Node *Tree23::nextPtr(Tree23::Node *node, const string &key,
                              int &subtreeIndex)
{
    // return if node is leaf or has some key
    if (node->isLeaf() || node->hasKey(key) != -1)
        return node;

    // find next index
    subtreeIndex = 0;
    while (subtreeIndex < node->size && node->data[subtreeIndex].key < key)
        subtreeIndex++;

    return node->subtree[subtreeIndex];
}

// some key return index
// not include return -1
int Tree23::Node::hasKey(const string &key)
{
    for (int i = 0; i < size; i++) {
        if (data[i].key == key)
            return i;
    }

    return -1;
}

// insert id to the node
void Tree23::insertToNode(Tree23::Node *node, int id, const string &key)
{
    int keySize = node->size;
    node->data[keySize].key = key;
    node->data[keySize].id.push_back(id);
    node->size++;
}

bool cmp(const Tree23::Data &A, const Tree23::Data &B) { return A.key < B.key; }

// sort and modify leaf
void Tree23::Node::sortLeaf() { sort(data, data + size, cmp); }

void Tree23::insert(int id, const string &key)
{
    // is null tree
    if (root == NULL) {
        root = new Node(NULL);

        // set first key
        root->addKey(id, key);
    }
    else {
        Node *cur = root, *next = cur;
        int subtreeIndex;
        bool isFull;

        // find correct node and key
        while (cur != (next = nextPtr(cur, key, subtreeIndex)))
            cur = next;

        // isSome key
        int index = cur->hasKey(key);
        if (index != -1)
            cur->data[index].id.push_back(id);
        else // insert to node
        {
            insertToNode(cur, id, key);

            // sort cur
            cur->sortLeaf();

            // split
            split(cur);
        }
    }
}

// get height
int Tree23::height()
{
    int count = 0;
    Node *cur = root;
    while (cur != NULL) {
        cur = cur->subtree[0];
        count++;
    }

    return count;
}

// result root data
void Tree23::getRoot(vector<int> &result)
{
    result.clear();
    for (int i = 0; i < root->size; i++) {
        for (auto id : root->data[i].id)
            result.push_back(id);
    }

    sort(result.begin(), result.end());
}

Tree23::Tree23() { root = NULL; }

void Tree23::removeNode(Node *cur)
{
    if (cur == NULL)
        return;

    for (int i = 0; i < cur->size; i++)
        removeNode(cur->subtree[i]);

    delete cur;
}

void Tree23::clear() { removeNode(root); }

void Tree23::find(vector<int> &result, const string &key)
{
    result.clear();

    Node *cur = root, *next = cur;
    int subtreeIndex;
    // find correct node and key
    while (cur != (next = nextPtr(cur, key, subtreeIndex)))
        cur = next;

    int index = cur->hasKey(key);

    if (index != -1) {
        for (int i = 0; i < cur->size; i++) {
            for (auto id : cur->data[i].id)
                result.push_back(id);
        }
    }

    sort(result.begin(), result.end());
}

