#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

// 01BFS
// 只有0/1边权的时候的最短路
// 复杂度 O(n + m)

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, s; cin >> n >> s;
  vector<vector<pair<long long, int>>> g(n + 1);

  const long long INF = 1e18;

  auto bfs = [&](int st) {
    deque<pair<long long, int>> que;
    vector<long long> d(n + 1, INF);
    que.push_front({0, st}); d[st] = 0;
    while (!que.empty()) {
      auto [dis, u] = que.front(); que.pop_front();
      if (dis > d[u]) continue;
      for (auto [w, v] : g[u]) {
        if (w == 0 && d[u] < d[v]) {
          d[v] = d[u];
          que.push_front({d[v], v});
        } else if(d[v] > d[u] + 1) {
          d[v] = d[u] + 1;
          que.push_back({d[v], v});
        }
      }
    }
    return d;
  };

  bfs(1);

  return 0;
}
