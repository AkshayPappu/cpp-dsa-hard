#include <iostream>
#include <vector>
using namespace std;

/*
B-Tree
Problem it solves: Minimizes # of nodes accessed while searching for a value by storing many keys in a single node and minimizing tree height (all leaves at same level)
- mainly used in databases and disk


Things to Note:
- Core idea is to insert at leaf nodes until they fill up and we need to build new parents
- Given a number T, each internal non-root node has [t-1, 2t - 1] keys and [t, 2t] children

Insert:
- if not leaf: split child if full and go to child
- if leaf: insert directly into node
*/

class BTreeNode {
  private:
    int t;
    bool leaf;
    int n;
    vector<int> keys;
    vector<BTreeNode*> children;
  public:
    BTreeNode(int _t, bool _leaf) : t(_t), leaf(_leaf), n(0) {
      keys = vector<int>(2 * t - 1, 0);
      children = vector<BTreeNode*>(2 * t, nullptr);
    };

    void insertNonFull(int key) {
      int i = n - 1; 

      if (leaf) {
        while (i >= 0 && key < keys[i]) {
          keys[i + 1] = keys[i];
          --i;
        }
        keys[i + 1] = key;
        ++n;
      } else {
        while (i >= 0 && key < keys[i]) {
          --i;
        }
        ++i;
        
        if (children[i]->n == 2 * t - 1) {
          splitChild(i, children[i]);
          if (key > keys[i]) {
            ++i;
          }
        }
        children[i]->insertNonFull(key);
      }
    }

    void splitChild(int i, BTreeNode* node) {
      BTreeNode* sibling_child = new BTreeNode(t, node->leaf);
      sibling_child->n = t - 1;

      // update current node with new child
      for (int j = n; j >= i + 1; --j) {
        children[j + 1] = children[j];
      }
      children[i + 1] = sibling_child;

      for (int j = n - 1; j >= i; --j) {
        keys[j + 1] = keys[j];
      }
      int median = node->keys[t - 1];
      keys[i] = median;
      node->keys[t - 1] = 0;
      ++n;


      // update child and sibling_child node's keys and children
      for (int j = 0; j < t - 1; ++j) {
        sibling_child->keys[j] = node->keys[t + j];
        node->keys[t + j] = 0;
      }

      if (node->leaf == false) {
        for (int j = 0; j < t; ++j) {
          sibling_child->children[j] = node->children[t + j];
          node->children[t + j] = nullptr;
        }
      }

      node->n = t - 1;
    }

    friend class BTree;
};

class BTree {
  private:
    BTreeNode* root;
    int t;
  public:
    BTree(int _t) : root(nullptr), t(_t) {};

    void insert(int key) {
      if (root == nullptr) {
        root = new BTreeNode(t, true);
        root->insertNonFull(key);
      } else {
        if (root->n == 2 * t - 1) {
          BTreeNode* new_root = new BTreeNode(t, false);
          new_root->children[0] = root;
          new_root->splitChild(0, root);
          
          root = new_root;
          root->insertNonFull(key);
        } else {
          root->insertNonFull(key);
        }
      }
    }

    bool searchHelper(BTreeNode* node, int key) {
      if (node == nullptr) return false;

      int i = 0; 

      while (i < node->n && key > node->keys[i]) {
        ++i;
      }

      if (i < node->n && key == node->keys[i]) {
        return true;
      }

      if (node->leaf) {
        return false;
      }

      return searchHelper(node->children[i], key);
    }

    bool search(int key) {
      return searchHelper(root, key);
    }

};

int main() {

  BTree tree{2};

  tree.insert(1);
  tree.insert(4);
  tree.insert(6);
  tree.insert(8);
  tree.insert(2);

  cout << tree.search(5) << '\n';
  cout << tree.search(6) << '\n';
  return 0;
}
