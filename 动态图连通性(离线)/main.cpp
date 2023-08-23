//
//  main.cpp
//  2022-08-14 17:32
//
//  Created by liznb
//  

#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL
#include "/Users/liznb/algo/liznb.h"
#endif

// https://loj.ac/s/1554369
//
// 动态图连通性(离线)
// 做法：
//    线段树分治，下标是时间，用 lazy 标记覆盖一条边出现的时间段。
//    回滚并查集在线段树里 dfs 维护连通性。
// 需要注意的地方:
//   1. 需要单独处理到最后也没消失的边。

struct DSU {
  vector<int> f, size;   
  vector<array<int, 2>> ops;
  DSU(int n) : f(n + 1), size(n + 1, 1) { iota(f.begin(), f.end(), 0); }

  inline int find(int x) {
    while (x != f[x]) x = f[f[x]];
    return x;
  }

  inline bool merge(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (size[x] > size[y]) swap(x, y);
    ops.push_back({x, f[x]});
    f[x] = y;    
    ops.push_back({~y, size[y]});
    size[y] += size[x];
    return true;
  }

  void rollback(int t) {
    while ((int) ops.size() > t) {
      assert(ops.size());
      auto [i, j] = ops.back();
      ops.pop_back();
      if (i >= 0) {
        f[i] = j;
      } else {
        size[~i] = j;
      }
    }
  }
};

struct Segment_Tree {
  struct Tree {
    int l, r; 
  };

  vector<Tree> tree;
  vector<vector<array<int, 2>>> lazy;

  Segment_Tree(int n) {
    tree.resize(n * 4 + 10);  
    lazy.resize(n * 4 + 10);
  }

  void build(int pos, int l, int r) {
    tree[pos].l = l; tree[pos].r = r;
    int m = l + (r - l) / 2;
    if (l == r) return;
    build(pos << 1, l, m);
    build(pos << 1 | 1, m + 1, r);
  }

  void add(int pos, array<int, 2> e) {
    lazy[pos].push_back(e);
  }

  void modify(int pos, int l, int r, array<int, 2> e) {
    assert(l <= r);
    if (tree[pos].l >= l && tree[pos].r <= r) {
      add(pos, e);
      return;
    }  
    int m = tree[pos].l + (tree[pos].r - tree[pos].l) / 2;
    if (l <= m) modify(pos << 1, l, r, e);
    if (r > m) modify(pos << 1 | 1, l, r, e);
  } 
};

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n, m; cin >> n >> m;

  vector<map<int, int>> has(n + 1);
  vector<array<int, 2>> ask(m + 1, {-1, -1});

  Segment_Tree T(m);
  T.build(1, 1, m);

  DSU dsu(n);

  for (int i = 1; i <= m; i++) {
    int op, x, y; cin >> op >> x >> y;
    if (op == 0) {
      has[x][y] = i;
      has[y][x] = i;
    } else if (op == 1) {
      // 注意 l <= r
      if (has[x][y] + 1 <= i - 1)
        T.modify(1, has[x][y] + 1, i - 1, {x, y});
      has[x].erase(y);
      has[y].erase(x);
    } else {
      ask[i] = {x, y};
    }
  }

  // 单独处理到最后也没消失的边
  for (int i = 1; i <= n; i++) {
    vector<array<int, 2>> erase;
    for (auto &it : has[i]) {
      if (it.second + 1 <= m)
        T.modify(1, it.second + 1, m, {i, it.first});
      erase.push_back({i, it.first});
    }
    for (auto &it : erase) {
      has[it[0]].erase(it[1]);
      has[it[1]].erase(it[0]);
    }
  }

  vector<int> ans;

  // 对线段树 dfs
  function<void(int)> dfs = [&] (int pos) {
    int backup = dsu.ops.size();

    for (auto &it : T.lazy[pos]) {
      dsu.merge(it[0], it[1]);
    }

    if (T.tree[pos].l == T.tree[pos].r) {
      int p = T.tree[pos].l;
      if (ask[p] != (array<int, 2>) {-1, -1}) {
        if (dsu.find(ask[p][0]) == dsu.find(ask[p][1])) {
          ans.emplace_back(1);
        } else {
          ans.emplace_back(0);
        }
      }
      dsu.rollback(backup);
      return;
    }

    dfs(pos << 1);
    dfs(pos << 1 | 1);

    dsu.rollback(backup);
  };

  dfs(1);

  for (auto &it : ans) {
    if (it) cout << "Y" << endl;
    else cout << "N" << endl;
  }

  return 0;
}
