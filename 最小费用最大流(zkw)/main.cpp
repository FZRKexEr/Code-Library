//
//  main.cpp
//  2022-04-14 11:53
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

// 特性说明：
// 1. 自环会忽略，重边不影响。
// 2. edges 参数分别是 u, v, residual, cost
// 3. 可以在费用负环图跑, SPFA 已加入 SLF + swap 优化

struct ZKW_MCMF {
  vector<array<int, 4>> edges;    
  vector<vector<int>> G;
  vector<int> vis, d;
  const long long INF = 0x3f3f3f3f3f3f3f3f;
  
  ZKW_MCMF(int n) : G(n + 1), vis(n + 1), d(n + 1) {}

  int s, t; long long cost;
  
  void add(int u, int v, int cap, int cost) {
    edges.push_back({u, v, cap, cost});
    edges.push_back({v, u, 0, -cost});
    G[u].push_back((int) edges.size() - 2);
    G[v].push_back((int) edges.size() - 1);
  }

  bool SPFA() {
    fill(d.begin(), d.end(), INF);
    fill(vis.begin(), vis.end(), 0);

    deque<int> que; que.push_front(t);
    vis[t] = 1; d[t] = 0;
    
    while (!que.empty()) {
      int x = que.front(); que.pop_front();
      vis[x] = false;
      for (auto &it : G[x]) {
        auto &[v, u, r, c] = edges[it ^ 1];
        if (u != v && r && d[u] + c < d[v]) {
          d[v] = d[u] + c;
          if (!vis[v]) {
            vis[v] = true;
            que.push_back(v);
            if (que.front() > que.back()) {
              swap(que.front(), que.back());
            }
          }
        }
      }
    }
    return d[s] != INF;
  }

  int DFS(int x, int low) {
    vis[x] = true; 
    if (x == t) return low;
    int flow = 0, f;
    for (auto &it : G[x]) {
      auto &[u, v, r, c] = edges[it]; 
      if (d[v] == d[x] - c && r && !vis[v]) {
        f = DFS(v, min(low, r));
        flow += f; 
        low -= f; 
        r -= f;
        edges[it ^ 1][2] += f;
        cost += 1ll * f * c;
        if (!low) break;
      }
    }
    return flow;
  }
  long long Mincost(int s, int t, int & flow) {
    this->s = s, this->t = t;
    flow = 0, cost = 0ll;
    while (SPFA()) {
      do {
        fill(vis.begin(), vis.end(), 0);
        flow += DFS(s, INF);
      } while (vis[t]);
    }
    return cost;
  }

};

signed main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  int n, m, s, t;
  cin >> n >> m >> s >> t;

  ZKW_MCMF mcmf(n);

  for (int i = 1; i <= m; i++) {
    int u, v, w, c;
    cin >> u >> v >> w >> c;
    mcmf.add(u, v, w, c);
  }

  int flow = 0; 
  int cost = mcmf.Mincost(s, t, flow);
  cout << flow << " " <<  cost << endl;
  return 0;
}
