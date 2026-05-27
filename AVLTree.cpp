#include <iostream>
#include <algorithm>
using namespace std;

/*
AVLTree:

Problem it solves: Self-rebalances to ensure worst case queries are optimized

Things to Note:
- need to keep track of node height
- need to calculate balance factor for node (< -1 or > 1 is skew)
- requires rebalancing algorithm that handles 4 edge cases

Left skew
    10                              20
  20       -> rotate right ->     30  10
30

Right skew
10                                  20
  20      -> rotate left ->       30  10
    30

Left skew and left child leans right
  10                                          10
20      -> rotate left child left ->      20      -> rotate right
 30                                    30

Right skew and right child leans left
10                                        10
  20    -> rotate right child right ->      20      -> rotate left
30                                            30

*/

class AVLTree {
  private:
    struct Node {
      int key;
      int height;
      Node* left;
      Node* right;

      Node(int _key) : key(_key), height(1), left(nullptr), right(nullptr) {}; 
    };

    Node* root = nullptr;

    int height(Node* node) {
      return node ? node->height : 0;
    }

    int balanceFactor(Node* node) {
      return height(node->left) - height(node->right);
    }

    void updateHeight(Node* node) {
      if (!node) return;
      node->height = max(height(node->left), height(node->right)) + 1;
    }

    Node* rotateRight(Node* y) {
      Node* x = y->left;
      Node* tmp = x->right;

      x->right = y;
      y->left = tmp;

      updateHeight(y);
      updateHeight(x);

      return x;
    }

    Node* rotateLeft(Node* x) {
      Node* y = x->right;
      Node* tmp = y->left;

      y->left = x;
      x->right = tmp;

      updateHeight(x);
      updateHeight(y);

      return y;
    }

    Node* rebalance(Node* node) {
      updateHeight(node);
      int bf = balanceFactor(node);

      // left heavy
      if (bf > 1) {
        // LR case
        if (balanceFactor(node->left) < 0) {
          node->left = rotateLeft(node->left);
        }
        node = rotateRight(node);
      }
      
      // right heavy
      if (bf < -1) {
        // RL case
        if (balanceFactor(node->right) > 0) {
          node->right = rotateRight(node->right);
        }
        node = rotateLeft(node);
      }

      return node;
    }

    Node* insert(Node* node, int key) {
      if (!node) return new Node(key);

      if (node->key > key) {
        node->left = insert(node->left, key);
      } else if (node->key < key) {
        node->right = insert(node->right, key);
      } else {
        return node;
      }

      return rebalance(node);
    }

    bool search(Node* node, int key) {
      if (!node) return false;
      
      if (node->key > key) {
        return search(node->left, key);
      } else if (node->key < key) {
        return search(node->right, key);
      } else {
        return true;
      }
    }

    Node* minRightNode(Node* node) {
      while (node->left) {
        node = node->left;
      }
      return node;
    }

    Node* remove(Node* node, int key) {
      if (!node) return node;

      if (node->key > key) {
        node->left = remove(node->left, key);
      } else if (node->key < key) {
        node->right = remove(node->right, key);
      } else {
        if (!node->left || !node->right) {
          Node* child = node->left ? node->left : node->right;
          delete node;
          return child;
        } else {
          Node* successor = minRightNode(node->right);
          node->key = successor->key;
          node->right = remove(node->right, successor->key);
        }
      }

      return rebalance(node);
    }

    void inorder(Node* node) {
      if (!node) return;

      inorder(node->left);
      cout << node->key << '\n';
      inorder(node->right);
    }

  public:
    void insert(int key) {
      root = insert(root, key);
    }

    bool search(int key) {
      return search(root, key);
    }

    void remove(int key) {
      root = remove(root, key);
    };

    void inorder() {
      inorder(root);
    }
};


// To execute C++, please define "int main()"
int main() {
  AVLTree tree;

  tree.insert(10);
  tree.insert(20);
  tree.insert(40);
  tree.insert(50);
  tree.insert(30);
  tree.inorder();

  cout << tree.search(30) << '\n';
  cout << tree.search(25) << '\n';

  tree.remove(10);
  tree.inorder();
  return 0;
}
