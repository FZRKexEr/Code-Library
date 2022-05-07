//
//  main.cpp
//  2022-05-06 22:18
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 无旋 Treap
// 特性说明:
// 1. kth 可以求出 Treap 中第 k 小的数
// 2. merge_treap 可以合并两个 treap，并且不需要保证它们之间的大小关系, 例题: CF911G。
//    但是效率成迷, 感性理解大概介于 O(logn) 和 O(n),  实测:
//
//    a. 在洛谷快速排序中 merge 可以 AC, merge_treap T 3组。
//    b. Treap 板题merge 和 merge_treap 都可以AC。(因为数据水)
// 3. erase 可以删除一个 key, 如果要删光 key, 把 res2[1] 赋值成 -1 即可
// 4. 这个 Treap 是模拟的 multiset
// 5. 没有 lower_bound 和 upper_bound, nxt() pre() count() 够用了，不要尝试实现 lower_bound,
//    返回 pos(下标) 对 Treap 没用，因为不能保证是根结点。
// 
// 注意:
// 1. 不是根结点的点是完全没用的，不能提供任何有用性质。所以一定要用 split 来得到根结点。
// 2. 不要随意在外面修改 size。修改后要想办法 update
//

struct Treap {
  struct Node {
    int l, r; 
    int key, pri, size;
    Node (int a, int b) : key(a), pri(b) { l = r = -1, size = 1; }
  };

  vector<Node> tree; 
  vector<int> root; 
  
  Treap(int n) : root(n + 1, -1) {} 
  
  array<int, 2> split(int pos, int key) {
    if (pos == -1) return {-1, -1};
    if (tree[pos].key <= key) {
      array<int, 2> res = split(tree[pos].r, key);
      tree[pos].r = res[0];
      update(pos);
      return {pos, res[1]};
    } else {
      array<int, 2> res = split(tree[pos].l, key);
      tree[pos].l = res[1]; 
      update(pos);
      return {res[0], pos};
    }
  }

  array<int, 2> split_sz(int pos, int sz) {
    if (pos == -1) return {-1, -1};
    if (tree[tree[pos].l].size + 1 <= sz) {
      array<int, 2> res = split_sz(tree[pos].r, sz - tree[tree[pos].l].size - 1);
      tree[pos].r = res[0];
      update(pos);
      return {pos, res[1]};
    } else {
      array<int, 2> res = split_sz(tree[pos].l, sz); 
      tree[pos].l = res[1];
      update(pos);
      return {res[0], pos};
    }
  }

  int merge(int x, int y) {
    if (x == -1) return y;
    if (y == -1) return x;
    if (tree[x].pri < tree[y].pri) {
      tree[x].r = merge(tree[x].r, y);  
      update(x);
      return x;
    } else {
      tree[y].l = merge(x, tree[y].l); 
      update(y);
      return y;
    }
  }

  int merge_treap(int x, int y) {
    if (x == -1) return y;
    if (y == -1) return x;
    if (tree[x].pri > tree[y].pri) swap(x, y);
    array<int, 2> res = split(y, tree[x].key);
    tree[x].l = merge_treap(tree[x].l, res[0]);
    tree[x].r = merge_treap(tree[x].r, res[1]);
    update(x);
    return x;
  }

  void update(int pos) {
    if (pos == -1) return;
    tree[pos].size = 1;
    if (tree[pos].l != -1) tree[pos].size += tree[tree[pos].l].size;
    if (tree[pos].r != -1) tree[pos].size += tree[tree[pos].r].size;
  }

  int create(int key) {
    mt19937 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
    int pri = (int) (rng() & ((1ll << 31) - 1));
    tree.emplace_back(key, pri);
    return (int) tree.size() - 1;
  }
  
  void insert(int &pos, int key) {
    int o = create(key);
    array<int, 2> res = split(pos, key);
    pos = merge(merge(res[0], o), res[1]); 
  }

  int rank(int &pos, int key) {
    array<int, 2> res = split(pos, key - 1);
    int rk = (res[0] == -1) ? 1 : tree[res[0]].size + 1;
    pos = merge(res[0], res[1]);
    return rk;
  }

  int kth(int &pos, int k) {
    assert(pos != -1);
    assert(k <= tree[pos].size);
    array<int, 2> res1 = split_sz(pos, k); 
    array<int, 2> res2 = split_sz(res1[0], k - 1);
    int key = tree[res2[1]].key;
    pos = merge(merge(res2[0], res2[1]), res1[1]);
    return key;
  }

  void erase(int &pos, int key) {
    array<int, 2> res1 = split(pos, key); 
    array<int, 2> res2 = split(res1[0], key - 1); 
    if (res2[1] != -1) {
      res2[1] = merge(tree[res2[1]].l, tree[res2[1]].r);
    }
    pos = merge(merge(res2[0], res2[1]), res1[1]);
  }

  int pre(int &pos, int key) {
    array<int, 2> res = split(pos, key - 1);  
    int ans = kth(res[0], tree[res[0]].size);
    pos = merge(res[0], res[1]);
    return ans;
  }

  int nxt(int &pos, int key) {
    array<int, 2> res = split(pos, key);
    int ans = kth(res[1], 1);
    pos = merge(res[0], res[1]);
    return ans;
  }

  int count(int &pos, int key) {
    array<int, 2> res1 = split(pos, key);
    array<int, 2> res2 = split(res1[0], key - 1);
    int ans = res2[1] == -1 ? 0 : tree[res2[1]].size;
    pos = merge(merge(res2[0], res2[1]), res1[1]);
    return ans;
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
 
  return 0;
}

