#include <iostream>
#include <vector>
using namespace std;

/*
Segment Tree

Problem it solves: Range sums in log(n); efficient when there are frequent updates

Things to Note:
- Recursion is intuitive
- Lazy Propogation: Makes range updates log(n); need a apply_lazy helper function that is used while updating single and multiple values and in queries. 
*/

using namespace std;

class SegmentTree {
  private:
    int n;
    vector<int> tree, lazy;
  public:
    SegmentTree(const vector<int>& arr) {
      n = arr.size();
      tree = vector<int>(4 * n);
      lazy = vector<int>(4 * n);
      buildHelper(arr, 1, 0, n - 1);
    };

    void buildHelper(const vector<int>& arr, int i, int s, int e) {
      if (s == e) {
        tree[i] = arr[s];
        return;
      }

      int m = s + (e - s) / 2;
      buildHelper(arr, 2 * i, s, m);
      buildHelper(arr, 2 * i + 1, m + 1, e);
      tree[i] = tree[2 * i] + tree[2 * i + 1];
    }

    void updateValue(int i, int val) {
      int old = query(i, i);
      int diff = val - old;
      updateRange(i, i, diff);  
    }

    void apply_lazy(int i , int s, int e) {
      if (lazy[i] != 0) {
        tree[i] += lazy[i] * (e - s + 1);
        if (s != e) {
          lazy[2 * i] += lazy[i];
          lazy[2 * i + 1] += lazy[i];
        }
        lazy[i] = 0;
      }
    }

    void updateRangeHelper(int l, int r, int val, int i, int s, int e) {
      apply_lazy(i, s, e);

      if (l > e || r < s) return;
      if (l <= s && r >= e) {
        lazy[i] += val;
        apply_lazy(i, s, e);
        return;
      }

      int m = s + (e - s) / 2;
      updateRangeHelper(l, r, val, 2 * i, s, m);
      updateRangeHelper(l, r, val, 2 * i + 1, m + 1, e);
      tree[i] = tree[2 * i] + tree[2 * i + 1];
    }

    void updateRange(int l, int r, int val) {
      updateRangeHelper(l, r, val, 1, 0, n - 1);
    }

    int queryHelper(int l, int r, int i, int s, int e) {
      apply_lazy(i, s, e);
      if (l > e || r < s) return 0;
      if (l <= s && r >= e) return tree[i];
      
      int m = s + (e - s) / 2;
      int lsum = queryHelper(l, r, 2 * i, s, m);
      int rsum = queryHelper(l, r, 2 * i + 1, m + 1, e);
      return lsum + rsum;
    }

    int query(int l, int r) {
      return queryHelper(l, r, 1, 0, n - 1);
    } 
};

int main() {
  vector<int> nums{1, 3, 5, 7, 9, 11};
  SegmentTree tree(nums);
  tree.updateRange(3, 5, 1);
  tree.updateValue(0, 1);

  cout << tree.query(3, 5) << '\n';;
  return 0;
}
