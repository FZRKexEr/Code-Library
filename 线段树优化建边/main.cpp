#include <bits/stdc++.h>
#define int long long
using namespace std;

// 线段树优化建边
//
// 1.注意这里开了 8n 空间，如果只有 u 到 [l, r] 或者 [l, r]到 u，只需要 4n 空间
// 2. 如果要 [l1, r1] 到 [l2, r2], 只需要建立一个虚点

struct Sgt {
  int n;
  vector<int> id;
  vector<vector<pair<int, int>>> g;

  Sgt(int _n) : n(_n), id(_n + 1), g(_n * 8 + 1) {
    build(1, 1, n);
  }

  void build(int pos, int tl, int tr) {
    if (tl == tr) {
      id[tl] = pos;
      g[pos].emplace_back(pos + 4 * n, 0);
      g[pos + 4 * n].emplace_back(pos, 0);
      return;
    }
    int m = (tl + tr) / 2;
    g[pos].emplace_back(pos << 1, 0);
    g[pos].emplace_back(pos << 1 | 1, 0);
    g[(pos << 1) + 4 * n].emplace_back(pos + 4 * n, 0);
    g[(pos << 1 | 1) + 4 * n].emplace_back(pos + 4 * n, 0);
    build(pos << 1, tl, m);
    build(pos << 1 | 1, m + 1, tr);
  }
  // o = 1  u到 [l, r], o = 0 [l, r]到 u
  void modify(int l, int r, int u, int w, int o, int pos, int tl, int tr) {
    if (tl >= l && tr <= r) {
      if (o) {
        g[id[u]].emplace_back(pos, w);
      } else {
        g[pos + 4 * n].emplace_back(id[u], w);
      }
      return;
    }
    int m = (tl + tr) / 2;
    if (l <= m) modify(l, r, u, w, o, pos << 1, tl, m);
    if (r > m) modify(l, r, u, w, o, pos << 1 | 1, m + 1, tr);
  }

  void solve(int s) {
    const long long INF = 1e18;
    auto dijkstra = [&]() {
      priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> que;
      vector<long long> d(n * 8 + 1, INF);

      que.emplace(0, id[s]); d[id[s]] = 0;

      while (!que.empty()) {
        auto [dis, u] = que.top(); que.pop();
        if (dis > d[u]) continue;
        for (auto [v, w] : g[u]) {
          if (w + dis < d[v]) {
            d[v] = w + dis;
            que.emplace(d[v], v);
          }
        }
      }
      return d;
    };
    auto d = dijkstra();
    for (int i = 1; i <= n; i++) {
      if (d[id[i]] == INF) cout << -1 << " ";
      else cout << d[id[i]] << " ";
    }
    cout << endl;
  }
};

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, q, s; cin >> n >> q >> s;

  Sgt T(n);

  for (int i = 1; i <= q; i++) {
    int t; cin >> t;
    if (t == 1) {
      int v, u, w; cin >> v >> u >> w;
      T.modify(u, u, v, w, 1, 1, 1, n);
    } else if (t == 2) {
      int v, l, r, w; cin >> v >> l >> r >> w;
      T.modify(l, r, v, w, 1, 1, 1, n);
    } else {
      int v, l, r, w; cin >> v >> l >> r >> w;
      T.modify(l, r, v, w, 0, 1, 1, n);
    }
  }

  T.solve(s);

  return 0;
}

