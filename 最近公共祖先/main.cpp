//
//  main.cpp
//  2022-04-19 00:01
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 特性：
// 1. add 会自动add双向边
// 2. add 必须 add 一颗标准的树，边数比点数少一的那种。
// 3. 如果是森林需要开多个 LCA 来存每一个树。 
// 4. 查询 lca 前要初始化
// 5. 编号请从1开始

struct LCA {
  vector<vector<int>> G, p;
  vector<int> d;
  int n, s; 

  LCA(int _n, int _s) : G(_n + 1), d(_n + 1, 1), n(_n), s(_s) {
    p.resize(_n + 1);
    int level = 0;
    while ((1 << level) <= _n) level++;
    for (int i = 0; i <= _n; i++) {
      p[i].resize(level + 1);
    }
  }

  int flag = false;

  void add(int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }
  
  void dfs(int pos, int fa) {
    p[pos][0] = fa;
    for (auto &v : G[pos]) {
      if (v == fa) continue;
      d[v] = d[pos] + 1;
      dfs(v, pos);
    }
  }

  void init() {
    flag = true; 
    dfs(s, s); 
    for (int level = 1; (1 << level) <= n; level++) {
      for (int i = 1; i <= n; i++) {
        p[i][level] = p[p[i][level - 1]][level - 1];
      }
    }
  }

  int lca(int a, int b) {
    assert(flag);
    if (d[a] < d[b]) swap(a, b); 
    int i, j;
    for (i = 0; (1 << i) <= d[a]; i++);  
    i--; 
    for (j = i; j >= 0; j--) {
      if (d[a] - (1 << j) >= d[b]) {
        a = p[a][j];
      }
    }
    if (a == b) return b; 
    for (j = i; j >= 0; j--) {
      if (p[a][j] != p[b][j]) {
        a = p[a][j];
        b = p[b][j];
      }
    }
    return p[a][0];
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  

  return 0;
}

