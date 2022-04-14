//
//  sol.cpp
//  2022-04-14 00:44
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;

// 特性说明：
// 1. 放在循环里，一次性使用。
// 2. 自环会忽略。重边不影响。
// 2. SLF + swap 优化，如果队尾小于队首，就 swap 它们。
// 3. 没有负环返回 1，有负环返回0
// 4. 注意 INF 只能最多允许两个加数不爆 long long
// 5. 必须 define int long long

struct SPFA {
  const int INF = 0x3f3f3f3f3f3f3f3f;
  vector<vector<array<int, 2>>> G;
  vector<int> d, inq, cnt;

  SPFA(int n) : G(n + 1), d(n + 1, INF), inq(n + 1, 0), cnt(n + 1, 0) {}
  
  void add(int u, int v, int w) {
    G[u].push_back({w, v});
  }

  int BFS(int n, int s) {
    deque<int> que; que.push_back(s);
    inq[s] = 1; d[s] = 0;
    while (!que.empty()) {
      int u = que.front(); que.pop_front();
      inq[u] = false;
      for (auto &[w, v] : G[u]) {
        if (v != u && d[v] > d[u] + w) {
          d[v] = d[u] + w;
          if (!inq[v]) {
            que.push_back(v);
            if (que.front() > que.back()) {
              swap(que.front(), que.back());
            }
            inq[v] = 1;
            if (++cnt[v] > n) return 0;
          }
        }
      }
    }
    return 1;
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n, m, s; cin >> n >> m >> s; 

  SPFA T(n);
  for (int i = 1; i <= m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    T.add(u, v, w);
  }
  T.BFS(n, s);
  for (int i = 1; i <= n; i++) {
    cout << T.d[i] << " ";
  }
  return 0;
}

