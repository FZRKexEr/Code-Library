//
//  main.cpp
//  2022-08-09 13:38
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 边双连通分量
// 1. 重边对边双连通分量有影响，模板可以处理。
// 2. 需要把双向边都存入 edges, 编号从 0 开始，保证双向边可以异或得到。
struct Tarjan {
  vector<int> dfn, low, belong, isbridge;
  vector<vector<int>> g, bcc;
  vector<array<int, 2>> edges;

  int dfs_clock;

  Tarjan(int o, int n, vector<vector<int>> &_g, vector<array<int, 2>> &_edges) : g(_g), edges(_edges) {
    dfn.resize(n + 1, 0);
    low.resize(n + 1, 0);
    isbridge.resize(edges.size(), 0);
    belong.resize(n + 1, -1);
    dfs_clock = 0;

    for (int i = o; i <= n; i++) {
      if (!dfn[i]) dfs(i, -1);
    }

    for (int i = o; i <= n; i++) {
      if (belong[i] == -1) {
        bcc.push_back({});
        ebcc(i);
      }
    }
  }

  int dfs(int u, int from) {
    int lowu = dfn[u] = ++dfs_clock;
    for (auto &it : g[u]) {
      auto [x, v] = edges[it];
      if (!dfn[v]) {
        int lowv = dfs(v, it);
        lowu = min(lowu, lowv);
        if (dfn[u] < low[v]) {
          isbridge[it] = 1;
          isbridge[it ^ 1] = 1;
        }
      } else if (dfn[v] < dfn[u] && (it ^ 1) != from) {
        lowu = min(lowu, dfn[v]);
      }
    }
    low[u] = lowu;
    return lowu;
  }

  void ebcc(int u) {
    belong[u] = (int) bcc.size() - 1;
    bcc.back().push_back(u);
    for (auto &it : g[u]) {
      if (isbridge[it]) continue;
      auto [x, v] = edges[it];
      if (belong[v] != -1) continue;
      ebcc(v);
    }
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n, m; cin >> n >> m;

  vector<array<int, 2>> edges;
  vector<vector<int>> g(n + 1);

  auto add = [&] (int u, int v) {
    edges.push_back({u, v}); 
    g[u].push_back((int) edges.size() - 1);
  };

  for (int i = 1; i <= m; i++) {
    int u, v; cin >> u >> v;
    add(u, v); add(v, u);
  }
  
  Tarjan T(1, n, g, edges);

  cout << T.bcc.size() << endl;
  for (int i = 0; i < (int) T.bcc.size(); i++) {
    cout << T.bcc[i].size() << " ";
    for (auto &it : T.bcc[i]) cout << it << " ";
    cout << endl;
  }

  return 0;
}

