//
//  main.cpp
//  2022-07-20 20:54
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 强连通分量 SCC
// 1. 关于强连通分量的数量:
//    a. 如果图的index是从 1 开始，数量 = cnt - 1
//    b. 如果图的index是从 0 开始，数量 = cnt
// 2. 注意不要混淆新图的点和原图的点的编号。

struct Tarjan {
  vector<int> dfn, low, belong, ins, stk;
  vector<vector<int>> g;
  int tm, cnt;

  Tarjan(int n, vector<vector<int>> &_g) : g(_g) {
    dfn.resize(n + 1, 0); 
    low.resize(n + 1, 0);
    ins.resize(n + 1, 0);
    stk.resize(n + 1, 0);
    belong.resize(n + 1, 0);
    tm = cnt = 0;
    for (int i = 0; i <= n; i++) {
      if (!dfn[i]) dfs(i);    
    }
  }

  void dfs(int n) {
    dfn[n] = low[n] = ++tm;       
    stk.push_back(n);
    ins[n] = 1;
    for (auto &v : g[n]) {
      if (dfn[v] == 0) {
        dfs(v);
        low[n] = min(low[v], low[n]);
      } else if (ins[v]) {
        low[n] = min(dfn[v], low[n]);
      }
    }
    if (dfn[n] == low[n]) {
      int last;
      do {
        last = stk.back(); stk.pop_back();
        belong[last] = cnt;
        ins[last] = 0;
      } while (last != n);
      cnt++;
    }
  }
};


int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
  int n, m, s; cin >> n >> m >> s; 

  vector<vector<int>> g1(n + 1); 

  for (int i = 1; i <= m; i++) {
    int u, v; cin >> u >> v; 
    g1[u].push_back(v);    
  }

  Tarjan T(n, g1);

  vector<vector<int>> g2(n + 1); 
  vector<int> degree(n + 1, 0);
  
  for (int i = 1; i <= n; i++) {
    int u = T.belong[i];
    for (auto &j : g1[i]) {
      int v = T.belong[j];
      if (u != v) {
        g2[u].push_back(v);
        degree[v]++;
      }
    }
  }

  int ans = 0;
  for (int i = 1; i <= T.cnt - 1; i++) {
    if (degree[i] == 0 && i != T.belong[s]) ans++;
  }

  cout << ans << endl; 

  return 0;
}

