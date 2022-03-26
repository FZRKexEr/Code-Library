#include <bits/stdc++.h>
using namespace std;

// Dijkstra  K 短路 + K 短路计数 (严格)
// 能处理 0 - n , 重边和自环不影响
// 注意根据数据范围替换 INF MOD
// 复杂度大概是 O(K(M + N)logN)

const int INF = 0x3f3f3f3f;
const int MOD = 100003;

struct Dijkstra {
  struct Edge {
    int w, v;

    Edge(int a, int b) : w(a), v(b) {}
    bool operator < (const Edge &oth) const {
      if (w != oth.w) return w < oth.w;
      return v < oth.v;
    }
    bool operator > (const Edge &oth) const {
      if (w != oth.w) return w > oth.w;
      return v > oth.v;
    }
  };
  vector<vector<Edge>> G; 
  vector<vector<int>> d, vis, cnt;
  int K; // 求 1-K 短路
  
  Dijkstra(int n, int k) : K(k) {
    G.resize(n + 1); 
    d.resize(n + 1, vector<int> (k, INF)); 
    vis.resize(n + 1, vector<int> (k, 0)); 
    cnt.resize(n + 1, vector<int> (k, 0));
  }

  void add(int u, int v, int w) {  // 有向边
    G[u].push_back({w, v}); // (距离, 序号) w 在前!
  }
  
  void solve(int s) {
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> que;           
    // 距离, 序号, 第K短
    que.push({0, s, 0});
    d[s][0] = 0; 
    cnt[s][0] = 1;

    while (!que.empty()) {
      auto x = que.top(); que.pop();
      int xw = x[0], xv = x[1], xo = x[2]; 
      
      if (vis[xv][xo]) continue;
      vis[xv][xo] = 1;

      for (auto &it : G[xv]) {
        int dis = it.w + xw;
        int xcnt = cnt[xv][xo];

        for (int i = 0; i < K; i++) {
          if (d[it.v][i] > dis) {
            swap(d[it.v][i], dis);
            swap(cnt[it.v][i], xcnt);
            que.push({d[it.v][i], it.v, i});
          } else if (d[it.v][i] == dis) {
            (cnt[it.v][i] += xcnt) %= MOD;
            break;
          }
        }
      }
    }
  }
};
int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  return 0;
}

