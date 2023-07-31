//
//  main.cpp
//  2022-05-24 16:27
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 回滚并查集
// 1. 操作的两个数 >= 0
// 2. rollback 回滚到第 t 次操作，注意一次合并有两次操作
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


int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  

  return 0;
}

