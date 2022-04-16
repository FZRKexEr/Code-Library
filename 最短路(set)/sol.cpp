//
//  最短路(set + 计数).cpp
//  2022-03-26 00:49
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// Dijkstra 最短路，最短路计数
// 能处理 0 - n , 重边和自环不影响
// 注意根据数据范围替换 INF MOD
// 直接在 main 里面声明

const int INF = 0x3f3f3f3f;
const int MOD = 1e9 + 7;

struct Dijkstra {
  struct Edge {
    int w, v;

    Edge(int a, int b) : w(a), v(b) {}
    bool operator < (const Edge &oth) const {
      if (w != oth.w) return w < oth.w;
      return v < oth.v;
    }
  };
  
  vector<vector<Edge>> G; 
  vector<int> d, cnt;
  
  Dijkstra(int n) : G(n + 1), d(n + 1, INF), cnt(n + 1, 0) {}

  void add(int u, int v, int w) {  // 有向边
    G[u].push_back({w, v}); // (距离, 序号) w 在前!
  }
  
  void solve(int s) {
    d[s] = 0; cnt[s] = 1;
    set<Edge> que; // (距离, 序号) 

    for (int i = 0; i < (int)d.size(); i++) {
      que.insert({d[i], i}); // (距离, 序号) 
    }
    
    while (que.empty() == false) {
      int x = que.begin()->v;
      que.erase(que.begin());

      for (auto &it : G[x]) {
        int v = it.v, w = it.w;

        if (d[v] > d[x] + w) {
          que.erase({d[v], v}); // (距离, 序号) 
          d[v] = d[x] + w;
          que.insert({d[v], v});
          cnt[v] = cnt[x];
        } else if (d[v] == d[x] + w) {
          (cnt[v] += cnt[x]) %= MOD;
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

