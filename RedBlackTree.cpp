#include <iostream>
using namespace std;

/*
Red Black Tree

Problem it solves: Self balancing tree that ensures that the distance from the root to the furthest leaf is at most 2x that of the closest leaf
- best used as a general purpose data structure with many insertions and deletions
- used in std::map, std::set, Linux completely fair scheduler

Insert Cases:
Case 0: node is root: make it black
Case 1: uncle is red: recolor
Case 2: uncle is black (triangle): rotate parent away from node to create Case 3
Case 3: uncle is black (line): recolor parent and grandparent and rotate grandparent away from node
*/

class RedBlackTree {
  private:
    struct Node {
      int key;
      std::string color;
      Node *left, *right, *parent;
      Node(int _key) : key(_key), color("Red"), left(nullptr), right(nullptr), parent(nullptr) {};
    };

    void leftRotate(Node* x) {
      Node* y = x->right;
      
      x->right = y->left;
      if (y->left != NIL) y->left->parent = x;

      y->parent = x->parent;
      if (x->parent == nullptr) root = y;
      else if (x == x->parent->left) x->parent->left = y;
      else x->parent->right = y;

      y->left = x;
      x->parent = y;
    }

    void rightRotate(Node* x) {
      Node* y = x->left;

      x->left = y->right;
      if (y->right != NIL) y->right->parent = x;

      y->parent = x->parent;
      if (x->parent == nullptr) root = y;
      else if (x == x->parent->left) x->parent->left = y;
      else x->parent->right = y;

      y->right = x;
      x->parent = y;
    }

    void fixInsert(Node* k) {
      while (k != root && k->parent->color == "Red") {
          if (k->parent == k->parent->parent->left) {
            Node* uncle = k->parent->parent->right;

            // Insert Case 1 (Uncle is red)
            if (uncle->color == "Red") {
              uncle->color = "Black";
              k->parent->color = "Black";
              k->parent->parent->color = "Red";
              k = k->parent->parent;
            } else {
              // Insert Case 2 (Uncle is black & triangle)
              if (k == k->parent->right) {
                k = k->parent;
                leftRotate(k);
              }

              // Insert Case 3 (Uncle is black & line)
              k->parent->color = "Black";
              k->parent->parent->color = "Red";
              rightRotate(k->parent->parent);
            }
          } else {
            // Mirror of above
            Node* uncle = k->parent->parent->left;
            if (uncle->color == "Red") {
              uncle->color = "Black";
              k->parent->color = "Black";
              k->parent->parent->color = "Red";
              k = k->parent->parent;
            } else {
              if (k == k->parent->left) {
                k = k->parent;
                rightRotate(k);
              }
              k->parent->color = "Black";
              k->parent->parent->color = "Red";
              leftRotate(k->parent->parent);
            }
          }
      }
      root->color = "Black";
    }

    void inorder(Node* node) {
      if (node == NIL) return;
      inorder(node->left);
      cout << node->key << '\n';
      inorder(node->right);
    }

    Node* root;
    Node* NIL;

  public:
    RedBlackTree() {
      NIL = new Node(0);
      NIL->color = "Black";
      NIL->left = NIL->right = NIL;
      root = NIL;
    }

    void insert(int key) {
      Node* node = new Node(key);
      node->color = "Red";
      node->left = node->right = NIL;

      Node* cur = root;
      Node* parent = nullptr;

      while (cur != NIL) {
        parent = cur;
        if (cur->key > key) {
          cur = cur->left;
        } else {
          cur = cur->right;
        }
      }
      
      node->parent = parent;
      if (parent == nullptr) {
        root = node;
      }
      else if (parent->key > node->key) {
        parent->left = node;
      } else {
        parent->right = node;
      }
      
      // Insert Case 0
      if (parent == nullptr) {
        root->color = "Black";
        return;
      }

      if (node->parent->parent == nullptr) return;

      fixInsert(node);
    }

    bool search(int key) {
      Node* cur = root;
      while (cur != NIL) {
        if (cur->key > key) cur = cur->left;
        else if (cur->key < key) cur = cur->right;
        else return true;
      }
      return false;
    }

    void inorder() {
      return inorder(root);
    }

};

int main() {
  RedBlackTree tree;

  tree.insert(10);
  tree.insert(20);
  tree.insert(30);
  tree.insert(50);
  tree.insert(40);
  tree.insert(25);
  tree.inorder();
}
