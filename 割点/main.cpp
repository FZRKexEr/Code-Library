//
//  main.cpp
//  2022-07-25 20:11
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// Tarjan 求割点
// 1. 重边不影响
// 2. 核心思想：
//   a. 非根节点 u 是 G 的割顶当且仅当 u 存在一个子结点 v, 
//      使得 v 及其所有后代都没有反相边连回 u 的祖先(连回 u 不算)。
//   b. 根节点只在儿子个数大于等于 2 的时候是割点。
// 3. 一个例子: 两点一线图中两个点都不是割点。
// 4. g 中的点可以从1开始也可以从0开始编号，因为单独一个点不是割点。
// 4. 复杂度 O(n + m)

struct Tarjan {
 
  vector<int> dfn, low, iscut;  
  vector<vector<int>> g;
  int dfs_clock;

  Tarjan(int n, vector<vector<int>> &_g) : g(_g) {
    dfn.resize(n + 1, 0);
    low.resize(n + 1, 0);
    iscut.resize(n + 1, 0);
    dfs_clock = 0;
    for (int i = 0; i <= n; i++) {
      if (!dfn[i]) dfs(i, -1); 
    }
  }
  
  // 根节点的父亲必须是负数
  int dfs(int u, int fa) {
    int lowu = dfn[u] = ++dfs_clock;
    int child = 0; 
    for (auto &v : g[u]) {
      if (!dfn[v]) {
        child++;
        int lowv = dfs(v, u);
        lowu = min(lowu, lowv);
        if (lowv >= dfn[u]) {
          iscut[u] = true;
        }
      } else if (dfn[v] < dfn[u] && v != fa) {
        lowu = min(lowu, dfn[v]);
      }
    } 
    if (fa < 0 && child == 1) iscut[u] = 0;
    low[u] = lowu;
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
    g[u].push_back(v);
    g[v].push_back(u); 
  }

  Tarjan T(n, g);
  vector<int> ans;
  for (int i = 1; i <= n; i++) {
    if (T.iscut[i]) ans.push_back(i);
  }

  cout << ans.size() << endl;
  for (auto &it : ans) cout << it << " ";

  return 0;
}

