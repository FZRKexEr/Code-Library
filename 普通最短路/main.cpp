#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m, s; cin >> n >> m >> s;
  vector<vector<array<long long, 2>>> g(n + 1);
  for (int i = 1; i <= m; i++) {
    int u, v, w; cin >> u >> v >> w;
    g[u].push_back({w, v});
  }

  // Dijkstra
  const long long INF = 1e18;
  auto dijkstra = [&]() {
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> que;
    vector<long long> d(n + 1, INF);
    que.emplace(0, s); d[s] = 0;
    while (!que.empty()) {
      auto [dis, u] = que.top(); que.pop();
      if (dis > d[u]) continue;
      for (auto [w, v] : g[u]) {
        if (w + dis < d[v]) {
          d[v] = w + dis;
          que.emplace(d[v], v);
        }
      }
    }
    return d;
  };
  // Dijkstra

  auto ans = dijkstra();
  for (int i = 1; i <= n; i++) {
    cout << ans[i] << " "[i == n];
  }

  return 0;
}
