
// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#include "BTree.h"
#include <algorithm>

// initialize
BPTree::BPTree() { root = NULL; }
BPTree::Node::Node(BPTree::Node *getPre)
{
    // set NULL
    pre = getPre;
    for (int i = 0; i < TREE_SIZE + 1; i++)
        subtree[i] = NULL;
    left = right = NULL;
    size = 0;
}

// traversal
BPTree::Node *BPTree::nextPtr(BPTree::Node *node, const float &key,
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
bool BPTree::Node::isLeaf()
{
    if (subtree[0] == NULL)
        return true;
    else
        return false;
}
int BPTree::Node::hasKey(const float &key)
{
    for (int i = 0; i < size; i++) {
        if (data[i].key == key)
            return i;
    }

    return -1;
}

// find
void BPTree::find(vector<int> &result, const float &key)
{
    result.clear();

    Node *cur = root, *next = cur;
    int subtreeIndex;
    // find correct node and key
    while (cur != (next = nextPtr(cur, key, subtreeIndex)))
        cur = next;

    int index = cur->hasKey(key);

    if (index != -1) {
        for (auto id : cur->data[index].id)
            result.push_back(id);
    }

    // sort(result.begin(), result.end());
}


// insert
void Data::assign(const Data &data)
{
    key = float(data.key);    // add a key
    id = vector<int>(data.id); // add a ids
}
void BPTree::insert(const Data &data)
{
    // is null tree
    if (root == NULL) {
        root = new Node(NULL);
        root->addKey(data);
    }
    else {
        Node *cur = root, *next = cur;
        int subtreeIndex;
        bool isFull;

        // find correct node and key
        while (cur != (next = nextPtr(cur, data.key, subtreeIndex)))
            cur = next;

        // isSome key
        int index = cur->hasKey(data.key);
        if (index != -1) {
            for (int i : data.id)
                cur->data[index].id.push_back(i);
        }

        else // insert to node
        {
            cur->addKey(data);

            // sort cur
            cur->sortLeaf();

            // split
            splitleaf(cur);
        }
    }
}
void BPTree::Node::addKey(const Data getData)
{
    data[size].assign(getData);
    size++;
}
void BPTree::insert(int id, const float &key)
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
            splitleaf(cur);
        }
    }
}
void BPTree::insertToNode(BPTree::Node *node, int id, const float &key)
{
    int keySize = node->size;
    node->data[keySize].key = key;
    node->data[keySize].id.push_back(id);
    node->size++;
}
void BPTree::Node::addKey(int id, const float &key)
{
    data[size].key = key;
    data[size].id.push_back(id);
    size++;
}
bool cmp(const Data &A, const Data &B) { return A.key < B.key; }
void BPTree::Node::sortLeaf() { sort(data, data + size, cmp); }

// split
BPTree::Node* BPTree::splitleaf(BPTree::Node *node)
{
    // node is exist and full
    if (node->size > TREE_KEY_SIZE) {
        if (node == root) { // is root
            // create new root
            Node *newRoot = new Node(NULL);
            newRoot->connect(root, 0); // set pre
            root = newRoot;            // replace root
        }

        Node *pre = node->pre;
        Node *leftNode = new Node(pre), *rightNode = new Node(pre);
        
        // copy data
        for (int i = 0; i < node->size; i++) {
            if(i < KEY_RMID_INDEX) leftNode->addKey(node->data[i]);
            else rightNode->addKey(node->data[i]);
        }

        // relink list
        if (node->left) node->left->right = leftNode;
        if (node->right) node->right->left = rightNode;
        leftNode->left = node->left;
        leftNode->right = rightNode;
        rightNode->left = leftNode;
        rightNode->right = node->right;

        // merge node and pre_node
        node->data[0].assign(node->data[KEY_RMID_INDEX]);
        node->subtree[0] = leftNode;
        node->subtree[1] = rightNode;
        node->size = 1;

        pre->merge(node);

        // remove node
        delete node;

        // recursive
        split(pre);
    }

    return node;
}
BPTree::Node* BPTree::split(BPTree::Node *node)
{
    // node is exist and full
    if (node->size > TREE_KEY_SIZE) {
        if (node == root) { // is root
            // create new root
            Node *newRoot = new Node(NULL);
            newRoot->connect(root, 0); // set pre
            root = newRoot;            // replace root
        }

        Node *pre = node->pre;
        Node *leftNode = new Node(pre), *rightNode = new Node(pre);

        // copy data & subtree
        for (int i = 0; i < node->size; i++) {
            if (i < KEY_RMID_INDEX) leftNode->addKey(node->data[i]);
            else if (i > KEY_RMID_INDEX) rightNode->addKey(node->data[i]);
        }
        for (int i = 0; i <= node->size; i++) {
            if (i < DATA_RMID_INDEX) leftNode->connect(node->subtree[i], i);
            else rightNode->connect(node->subtree[i], i - DATA_RMID_INDEX);
        }

        // merge node and pre_node
        node->data[0].assign(node->data[KEY_RMID_INDEX]);
        node->subtree[0] = leftNode;
        node->subtree[1] = rightNode;
        node->size = 1;

        pre->merge(node);

        // remove node
        delete node;

        // recursive
        split(pre);
    }

    return node;
}
void BPTree::Node::connect(Node *sub, int subIndex)
{
    subtree[subIndex] = sub;
    if (sub != NULL)
        sub->pre = this;
}
void BPTree::Node::merge(Node *node)
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

// remove
bool BPTree::empty() { return (root == NULL); }
void BPTree::clear() { removeNode(root); }
void BPTree::removeNode(Node *cur)
{
    if (cur == NULL)
        return;

    for (int i = 0; i < cur->size; i++)
        removeNode(cur->subtree[i]);

    delete cur;
}
void BPTree::Node::remove(int index)
{
    for (int i = index; i < size - 1; i++)
        data[i] = data[i + 1];
}

// status func
int BPTree::getHeight()
{
    int count = 0;
    Node *cur = root;
    while (cur != NULL) {
        cur = cur->subtree[0];
        count++;
    }

    return count;
}
void BPTree::getRoot(vector<int> &result)
{
    result.clear();
    for (int i = 0; i < root->size; i++) {
        for (auto id : root->data[i].id)
            result.push_back(id);
    }

    sort(result.begin(), result.end());
}
void BPTree::printList()
{
    Node *cur = root, *next = cur;
    int subtreeIndex;
    // find correct node and key
    while (cur != (next = nextPtr(cur, MAX_KEY, subtreeIndex)))
        cur = next;

    int count = 1;
    while (cur != NULL) {
        for (int i = cur->size -1; i >= 0; i--) {
            cout << '[' << count++ << "] " << cur->data[i].key << " : ";
            cout << cur->data[i].id[0];
            for (int j = 1; j < cur->data[i].id.size(); j++)
                cout << ", " << cur->data[i].id[j];
            cout << endl;
        }
        cur = cur->left;
    }
}

int BPTree::getOffset(float key)
{
    Node *cur = root, *next = cur;
    int subtreeIndex;
    // find correct node and key
    while (cur != (next = nextPtr(cur, key, subtreeIndex)))
        cur = next;

    for (int i = 0; i < cur->size; i++)
        if (cur->data[i].key == key) {
            if(i-1 >= 0) return cur->data[i-1].id[0];
            else return cur->left->data[0].id[0];
        }
}






