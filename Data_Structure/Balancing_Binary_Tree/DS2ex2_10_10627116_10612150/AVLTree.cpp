// 第10組 106127116 許逸翔 10612150 林詠翔 資訊二甲
// must to use -std=c++11 or higher version
#include "AVLTree.h"

AVLTree::Node *AVLTree::LR_roate(AVLTree::Node *cur)
{
    cur->left = RR_roate(cur->left);
    return LL_roate(cur);
}

AVLTree::Node *AVLTree::RL_roate(AVLTree::Node *cur)
{
    cur->right = LL_roate(cur->right);
    return RR_roate(cur);
}

AVLTree::Node *AVLTree::LL_roate(AVLTree::Node *cur)
{
    Node *left = cur->left;
    cur->left = left->right;
    left->right = cur;
    return left;
}

AVLTree::Node *AVLTree::RR_roate(AVLTree::Node *cur)
{
    Node *right = cur->right;
    cur->right = right->left;
    right->left = cur;
    return right;
}

void AVLTree::newHeight(Node *node, int deepMax)
{
    if (node == NULL)
        return;

    Node *left = node->left, *right = node->right;
    if (deepMax != 0) {
        newHeight(left, deepMax - 1);
        newHeight(right, deepMax - 1);
    }

    // count
    if (left != NULL && right != NULL)
        node->height =
            (left->height > right->height ? left->height : right->height) + 1;
    else if (right != NULL)
        node->height = right->height + 1;
    else if (left != NULL)
        node->height = left->height + 1;
    else
        node->height = 1;
}

int AVLTree::getBF(Node *node)
{
    if (node == NULL)
        return 0;

    Node *left = node->left, *right = node->right;
    if (left != NULL && right != NULL)
        return left->height - right->height;
    else if (right != NULL)
        return 0 - right->height;
    else if (left != NULL)
        return left->height - 0;
    else
        return 0;
}

AVLTree::AVLTree() { root = NULL; }

AVLTree::Node *AVLTree::insert(Node *node, int id, const string &key)
{
    if (node == NULL) {
        // create new node
        node = new Node();
        node->left = node->right = NULL;
        node->key = key;
        node->ids.push_back(id);
    }
    else if (node->key == key)
        node->ids.push_back(id);
    else {
        if (key < node->key)
            node->left = insert(node->left, id, key);
        else if (key > node->key)
            node->right = insert(node->right, id, key);

        // count BF
        Node *left = node->left, *right = node->right;
        int nodeBF = getBF(node);
        int rightBF = getBF(node->right);
        int leftBF = getBF(node->left);

        if (left != NULL && nodeBF == +2 && leftBF != -1)
            node = LL_roate(node);
        else if (left != NULL && nodeBF == +2 && leftBF == -1)
            node = LR_roate(node);
        else if (right != NULL && nodeBF == -2 && rightBF == +1)
            node = RL_roate(node);
        else if (right != NULL && nodeBF == -2 && rightBF != +1)
            node = RR_roate(node);
    }

    // count tree height
    newHeight(node, 2);

    return node;
}

void AVLTree::insert(int id, const string &key)
{
    root = insert(root, id, key);
}

void AVLTree::inorder(vector<string> &vec, Node *cur)
{
    if (cur == NULL)
        return;

    inorder(vec, cur->left);
    vec.push_back(cur->key);
    inorder(vec, cur->right);
}

void AVLTree::preorder(vector<string> &vec, Node *cur)
{
    if (cur == NULL)
        return;

    vec.push_back(cur->key);
    preorder(vec, cur->left);
    preorder(vec, cur->right);
}

void AVLTree::inorder(vector<string> &vec) { inorder(vec, root); }
void AVLTree::preorder(vector<string> &vec) { preorder(vec, root); }

void AVLTree::removeNode(Node *cur)
{
    if (cur == NULL)
        return;
    removeNode(cur->left);
    removeNode(cur->right);
    delete cur;
}

void AVLTree::clear() { removeNode(root); }

int AVLTree::height()
{
    if (root != NULL)
        return root->height;
    else
        return 0;
}

void AVLTree::getRoot(vector<int> &result) { result = root->ids; }

void AVLTree::find(vector<int> &result, const string &key, Node *cur)
{
    if (cur == NULL)
        return;

    if (cur->key > key)
        find(result, key, cur->left);
    else if (cur->key < key)
        find(result, key, cur->right);
    else // equal
        result = cur->ids;
}

void AVLTree::find(vector<int> &result, const string &key)
{
    result.clear();
    find(result, key, root);
}
