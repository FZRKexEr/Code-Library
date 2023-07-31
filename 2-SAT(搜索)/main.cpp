//
//  main.cpp
//  2022-04-21 21:00
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

// 标准 2-SAT
// 暴搜 + 随机化
// 期望复杂度 O(nlogn ?), 实测效率优秀。
// 下标从1开始。
// 注意：2-sat 很简单，如果不赶时间建议理解代码再粘板子

struct TwoSAT{
  vector<vector<int>> G;
  vector<bool> vis;
  stack<int> stk;
  int n;

  TwoSAT(int _n) : G(_n * 2 + 2), vis(_n * 2 + 2, 0) { n = _n; }
 
  // a = at, b = bt 至少一个成立
  void addor(int a, int at, int b, int bt) {
    a += a + at;
    b += b + bt;
    G[a ^ 1].push_back(b); 
    G[b ^ 1].push_back(a); 
  }

  // a = at, b = bt 不能同时成立
  void addnand(int a, int at, int b, int bt) {
    a += a + at;
    b += b + bt;
    G[a].push_back(b ^ 1);
    G[b].push_back(a ^ 1);
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
        int c = (it ^ i) & 1; // 随机选真假
        if (!dfs(it * 2 + c)) {
          while (!stk.empty()) {
            vis[stk.top()] = 0;
            stk.pop();
          }
          if (!dfs(it * 2 + (c ^ 1))) return false;
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
