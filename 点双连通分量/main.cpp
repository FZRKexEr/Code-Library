//
//  main.cpp
//  2022-07-27 21:30
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// Tarjan 求点双连通分量和割点
// 1. bccno : 非割点属于的点双连通分量编号(从0开始), 割点编号无意义(可以属于多个点双连通分量)。
// 2. bcc : 每个点双连通分量包含的点。(包含割点, 编号从0开始)
// 3. 注意: 点数小于等于2的连通图是bcc
// 4. 重边不影响答案，但是务必(在读入时，而不是模板内!)除掉自环

struct Tarjan {
  
  vector<int> dfn, low, iscut, bccno;
  vector<vector<int>> g, bcc;
  stack<array<int, 2>> stk;
  int dfs_clock;

  Tarjan(int o, int n, vector<vector<int>> &_g) : g(_g) {
    dfn.resize(n + 1, 0);
    low.resize(n + 1, 0);
    iscut.resize(n + 1, 0);
    bccno.resize(n + 1, -1);  
    dfs_clock = 0; 

    for (int i = o; i <= n; i++) {
      if (!dfn[i]) dfs(i, -1);
    }
  }

  int dfs(int u, int fa) {
    int lowu = dfn[u] = ++dfs_clock;
    int child = 0;  
    for (auto &v : g[u]) {
      if (!dfn[v]) {
        stk.push({u, v});    
        child++;
        int lowv = dfs(v, u);
        lowu = min(lowu, lowv);
        if (lowv >= dfn[u]) {
          iscut[u] = true;
          bcc.push_back({});
          while (true) {
            auto [xu, xv] = stk.top(); stk.pop();
            if (bccno[xu] != (int) bcc.size() - 1) {
              bcc.back().push_back(xu); 
              bccno[xu] = (int) bcc.size() - 1;
            }
            if (bccno[xv] != (int) bcc.size() - 1) {
              bcc.back().push_back(xv);
              bccno[xv] = (int) bcc.size() - 1;
            }
            if (xu == u && xv == v) break;
          }
        } 
      } else if (dfn[v] < dfn[u] && v != fa) {
        stk.push({u, v});
        lowu = min(lowu, dfn[v]);
      }
    }
    if (fa < 0 && child == 1) iscut[u] = 0; 
    if (fa < 0 && (int) g[u].size() == 0) bcc.push_back({u});
    return lowu;
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n, m; cin >> n >> m;
  vector<vector<int>> g(n + 1);
  for (int i = 1; i <= m; i++) {
    int u, v; cin >> u >> v;
    if (u == v) continue;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  
  Tarjan T(1, n, g);

  cout << T.bcc.size() << endl;
  
  for (int i = 0; i < (int) T.bcc.size(); i++) {
    cout << T.bcc[i].size() << " ";
    for (auto &it : T.bcc[i]) cout << it << " "; 
    cout << endl;
  }

  return 0;
}

