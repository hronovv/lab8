#ifndef BST_H
#define BST_H
#include "iostream"

/*
 *
 * 1. BST = Binary search tree
 * 2. Diff from binary tree:
 * 2.1 Left node is smaller than the root
 * 2.2 Right node is larger than the root
 *
 */

enum Color { RED, BLACK };

template <typename Key, typename Value>
struct TreeNode {
    Key key;
    Value value;
    Color color;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;

    TreeNode(const Key& key, const Value& value)
        : key(key), value(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};



template<typename Key, typename Value>
class GoodRBTree {
    TreeNode<Key, Value>* root;

public:
    GoodRBTree() : root(nullptr) {}

    ~GoodRBTree() {
        clear();
    }

    TreeNode<Key, Value>* getRoot() const {
        return root;
    }

    void insert(const Key& key, const Value& value) {
        TreeNode<Key, Value>* newNode = new TreeNode<Key, Value>(key, value);
        root = bstInsert(root, newNode);
        fixInsert(newNode);
    }

    TreeNode<Key, Value>* search(const Key& key) {
        return search(root, key);
    }

    void deleteNode(const Key& key) {
        TreeNode<Key, Value>* node = search(key);
        if (node) deleteNodeHelper(node);
    }

    void clear() {
        clearHelper(root);
        root = nullptr;
    }

    Value getValueByKey(const Key& key) {
        TreeNode<Key, Value>* node = search(key);
        if (!node) throw std::invalid_argument("Key not found!");
        return node->value;
    }

    std::string preorder() const {
        std::string result;
        preorder(root, result);
        return result;
    }

    std::string inorder() const {
        std::string result;
        inorder(root, result);
        return result;
    }

    std::string postorder() const {
        std::string result;
        postorder(root, result);
        return result;
    }

    TreeNode<Key, Value>* getMinNode() {
        TreeNode<Key, Value>* curr = root;
        while (curr && curr->left)
            curr = curr->left;
        return curr;
    }

    TreeNode<Key, Value>* getMaxNode() {
        TreeNode<Key, Value>* curr = root;
        while (curr && curr->right)
            curr = curr->right;
        return curr;
    }

    void swapValues(TreeNode<Key, Value>* a, TreeNode<Key, Value>* b) {
        if (a && b)
            std::swap(a->value, b->value);
    }

    TreeNode<Key, Value>* lowestCommonAncestor(const Key& key1, const Key& key2) const {
        return lowestCommonAncestor(root, key1, key2);
    }


private:
    TreeNode<Key, Value>* bstInsert(TreeNode<Key, Value>* root, TreeNode<Key, Value>* node) {
        if (!root) return node;

        if (node->key < root->key) {
            root->left = bstInsert(root->left, node);
            root->left->parent = root;
        } else if (node->key > root->key) {
            root->right = bstInsert(root->right, node);
            root->right->parent = root;
        }
        return root;
    }

    void rotateLeft(TreeNode<Key, Value>*& root, TreeNode<Key, Value>*& x) {
        TreeNode<Key, Value>* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rotateRight(TreeNode<Key, Value>*& root, TreeNode<Key, Value>*& y) {
        TreeNode<Key, Value>* x = y->left;
        y->left = x->right;
        if (x->right) x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent) root = x;
        else if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    void fixInsert(TreeNode<Key, Value>*& node) {
        while (node != root && node->parent && node->parent->color == RED) {
            TreeNode<Key, Value>* parent = node->parent;
            TreeNode<Key, Value>* grandparent = parent->parent;

            if (!grandparent) break;  // нужно, чтобы избежать segfault

            if (parent == grandparent->left) {
                TreeNode<Key, Value>* uncle = grandparent->right;
                if (uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->right) {
                        rotateLeft(root, parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(root, grandparent);
                    if (parent) std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            } else {
                TreeNode<Key, Value>* uncle = grandparent->left;
                if (uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->left) {
                        rotateRight(root, parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(root, grandparent);
                    if (parent) std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        if (root) root->color = BLACK;
    }

    void transplant(TreeNode<Key, Value>* u, TreeNode<Key, Value>* v) {
        if (!u->parent)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v)
            v->parent = u->parent;
    }

    void deleteNodeHelper(TreeNode<Key, Value>* node) {
        TreeNode<Key, Value>* y = node;
        TreeNode<Key, Value>* x;
        Color y_original_color = y->color;

        if (!node->left) {
            x = node->right;
            transplant(node, node->right);
        } else if (!node->right) {
            x = node->left;
            transplant(node, node->left);
        } else {
            y = minimum(node->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == node) {
                if (x) x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }

            transplant(node, y);
            y->left = node->left;
            y->left->parent = y;
            y->color = node->color;
        }

        delete node;

        if (y_original_color == BLACK && x)
            fixDelete(x);
    }

    void fixDelete(TreeNode<Key, Value>* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                TreeNode<Key, Value>* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(root, x->parent);
                    w = x->parent->right;
                }

                if ((!w->left || w->left->color == BLACK) &&
                    (!w->right || w->right->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (!w->right || w->right->color == BLACK) {
                        if (w->left) w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(root, w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right) w->right->color = BLACK;
                    rotateLeft(root, x->parent);
                    x = root;
                }
            } else {
                TreeNode<Key, Value>* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(root, x->parent);
                    w = x->parent->left;
                }

                if ((!w->right || w->right->color == BLACK) &&
                    (!w->left || w->left->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (!w->left || w->left->color == BLACK) {
                        if (w->right) w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(root, w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left) w->left->color = BLACK;
                    rotateRight(root, x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    TreeNode<Key, Value>* minimum(TreeNode<Key, Value>* node) {
        while (node->left)
            node = node->left;
        return node;
    }

    void clearHelper(TreeNode<Key, Value>* node) {
        if (!node) return;
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }

    TreeNode<Key, Value>* search(TreeNode<Key, Value>* node, const Key& key) {
        if (!node || node->key == key)
            return node;
        if (key < node->key)
            return search(node->left, key);
        return search(node->right, key);
    }

    void preorder(TreeNode<Key, Value>* node, std::string& result) const {
        if (!node) return;
        result += std::to_string(node->key) + ": " + node->value + "\n";
        preorder(node->left, result);
        preorder(node->right, result);
    }

    void inorder(TreeNode<Key, Value>* node, std::string& result) const {
        if (!node) return;
        inorder(node->left, result);
        result += std::to_string(node->key) + ": " + node->value + "\n";
        inorder(node->right, result);
    }

    void postorder(TreeNode<Key, Value>* node, std::string& result) const {
        if (!node) return;
        postorder(node->left, result);
        postorder(node->right, result);
        result += std::to_string(node->key) + ": " + node->value + "\n";
    }

    TreeNode<Key, Value>* lowestCommonAncestor(TreeNode<Key, Value>* node, const Key& key1, const Key& key2) const {
        if (!node) return nullptr;

        if (key1 < node->key && key2 < node->key)
            return lowestCommonAncestor(node->left, key1, key2);

        if (key1 > node->key && key2 > node->key)
            return lowestCommonAncestor(node->right, key1, key2);

        return node;
    }

};



#endif //BST_H
