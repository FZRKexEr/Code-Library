//
//  main.cpp
//  2022-04-21 21:00
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

// 暴搜 + 随机化
// 最坏复杂度 O(nm), 随机化后很难被卡（怀疑甚至不能被卡）。
// 在随机数据下效率非常优秀。
// 下标从1开始。

struct TwoSAT{
  vector<vector<int>> G;
  vector<bool> vis;
  stack<int> stk;
  int n;

  TwoSAT(int _n) : G(_n * 2 + 2), vis(_n * 2 + 2, 0) { n = _n; }
 
  void addor(int a, int at, int b, int bt) {
    a += a + at;
    b += b + bt;
    G[a ^ 1].push_back(b); // !a -> b
    G[b ^ 1].push_back(a); // !b -> a
  }

  bool dfs(int pos) {
    if (vis[pos ^ 1]) return false;
    if (vis[pos]) return true;
    vis[pos] = 1;
    stk.push(pos);
    for (auto &v : G[pos]) {
      if (!dfs(v)) return false;
    }
    return true;
  }

  bool sol() {
    random_device rd;
    mt19937 seed(rd());
    vector<int> ord(n + 1);
    iota(ord.begin(), ord.end(), 0);
    shuffle(ord.begin() + 1, ord.end(), seed); // 随机选点 dfs
    
    for (int i = 1; i <= n; i++) {
      int it = ord[i];
      if (!vis[it * 2 + 1] && !vis[it * 2]) {
        while (!stk.empty()) stk.pop();
        if (!dfs(it * 2)) {
          while (!stk.empty()) {
            vis[stk.top()] = 0;
            stk.pop();
          }
          if (!dfs(it * 2 + 1)) return false;
        }
      }
    }
    return true;
  }
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n, m; cin >> n >> m;

  TwoSAT T(n);
  for (int i = 1; i <= m; i++) {
    int u, x, v, y; cin >> u >> x >> v >> y;
    T.addor(u, x, v, y);
  }
  if (T.sol()) {
    cout << "POSSIBLE" << endl;
    for (int i = 1; i <= n; i++) {
      if (T.vis[i * 2]) cout << 0 << " ";
      else cout << 1 << " ";
    }
  } else {
    cout << "IMPOSSIBLE" << endl;
  }
   
  return 0;
}
