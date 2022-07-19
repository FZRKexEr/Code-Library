//
//  main.cpp
//  2022-07-19 23:28
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 最小瓶颈路
// 1. 定义: 两点之间所有路径中最大边权最小的路径。
//    定理: 最小生成树上两点之间在树上的路径是一个最小瓶颈路。
//
// 2. 原则上只能处理正边权，但是可以修改后处理负边权,
//    更改 max_edge 初始化最小值( 默认是0 )
//
// 3. 使用方法：
//    a. 初始化为点数 n
//    b. add 加边 u, v, w, 自动无向
//    c. build 建树, 不需要传参数
//    d. query(a, b) 查询 a <-> b 最小瓶颈路上的最大边权。
struct Bottleneck {
  
  struct DSU {
    vector<int> f, dep, size;   
    DSU(int n) : f(n + 1), dep(n + 1, 0), size(n + 1, 1) { iota(f.begin(), f.end(), 0); }
  
    inline int find(int x) {
      while (x != f[x]) x = f[x] = f[f[x]];
      return x;
    }
  
    inline bool merge(int x, int y) {
      x = find(x), y = find(y); 
  
      if (x == y) return false;
      if (dep[x] > dep[y]) swap(x, y);
      if (dep[x] == dep[y]) dep[y]++;
  
      f[x] = y;    
      size[y] += size[x];
      return true;
    }
  };

  // LCA·改
  // 求出 lca 的同时求出两点路径上的边权最大值。
  // p[i][j][0] 是边权(最大值)
  // G[i][j][0] 是边权
  struct LCA {
    vector<vector<array<int, 2>>> G, p;
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
  
    void add(int u, int v, int w) {
      G[u].push_back({w, v});
      G[v].push_back({w, u});
    }
    
    void dfs(int pos, int fa) {
      p[pos][0][1] = fa;
      for (auto &[w, v] : G[pos]) {
        if (v == fa) continue;
        d[v] = d[pos] + 1;
        p[v][0][0] = w;
        dfs(v, pos);
      }
    }
  
    void init() {
      flag = true; 
      dfs(s, s); 
      for (int level = 1; (1 << level) <= n; level++) {
        for (int i = 1; i <= n; i++) {
          p[i][level][1] = p[p[i][level - 1][1]][level - 1][1];
          p[i][level][0] = max(p[i][level - 1][0], p[p[i][level - 1][1]][level - 1][0]);
        }
      }
    }
    // 返回 [0] 最大边权 [1] lca ，如果最大边权不存在，返回0
    array<int, 2> lca(int a, int b) {
      assert(flag);
      if (d[a] < d[b]) swap(a, b); 
      int i, j;
      for (i = 0; (1 << i) <= d[a]; i++);  
      i--; 
      int max_edge = 0;
      
      for (j = i; j >= 0; j--) {
        if (d[a] - (1 << j) >= d[b]) {
          max_edge = max(max_edge, p[a][j][0]);
          a = p[a][j][1];
        }
      }
      if (a == b) return {max_edge, b}; 
      for (j = i; j >= 0; j--) {
        if (p[a][j][1] != p[b][j][1]) {
          max_edge = max(max_edge, p[a][j][0]);
          max_edge = max(max_edge, p[b][j][0]);
          a = p[a][j][1];
          b = p[b][j][1];
        }
      }
      max_edge = max(max_edge, p[a][0][0]);
      max_edge = max(max_edge, p[b][0][0]);
      return {max_edge, p[a][0][1]};
    }
  };

  vector<array<int, 3>> edges;
  DSU T1;
  LCA T2; 

  Bottleneck(int n): T1(n), T2(n, 1) {}
  
  void add(int u, int v, int w) {
    edges.push_back({w, u, v});
  }

  void build() {
    sort(edges.begin(), edges.end());
    for(auto &[w, u, v] : edges) {
      if (T1.merge(u, v)) {
        T2.add(u, v, w);
      }
    }
    T2.init(); 
  }

  int query(int a, int b) {
    if (T1.find(a) != T1.find(b)) return 0;
    return T2.lca(a, b)[0]; 
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
  return 0;
}

