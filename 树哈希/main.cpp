//
//  main.cpp
//  2022-12-28 17:55
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define endl '\n'

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int m; cin >> m;

  map<array<int, 2>, int> ord;

  for (int k = 1; k <= m; k++) {

    int n; cin >> n;
    vector<vector<int>> g(n + 1);
    for (int j = 1; j <= n; j++) {
      int fa; cin >> fa;
      if (fa) {
        g[j].emplace_back(fa);
        g[fa].emplace_back(j);
      }
    }

    // hash 开始
    const int MOD = 2147483647;
    const array<int, 2> P = {48271, 16807};

    auto h = [&](int x, const int p) {
      return 1ll * x * p % MOD;
    };

    // O(n) 有根树 hash
    vector<array<int, 2>> h_root(n + 1);
    function<void(int, int)> get_hash = [&](int pos, int fa) {
      h_root[pos] = {1, 1};
      for (auto &v : g[pos]) {
        if (v == fa) continue;
        get_hash(v, pos);
        for (int i = 0; i < 2; i++)
          h_root[pos][i] = (1ll * h_root[pos][i] + h(h_root[v][i], P[i])) % MOD;
      }
    };
    get_hash(1, -1);

    // O(n) 换根dp, 无根树 hash, 需要先做一次有根树 hash
    vector<array<int, 2>> h_rootless(n + 1);
    function<void(int, int)> get_hash_rootless = [&](int pos, int fa) {
      for (auto &v : g[pos]) {
        if (v == fa) continue;
        for (int i = 0; i < 2; i++) 
          h_rootless[v][i] = (h((h_rootless[pos][i] - h(h_root[v][i], P[i]) + MOD) % MOD, P[i]) + h_root[v][i]) % MOD;
        get_hash_rootless(v, pos);
      }
    };
    h_rootless[1] = h_root[1];
    get_hash_rootless(1, -1);
    // hash 结束
    
    array<int, 2> ans = {0, 0};
    for (int i = 1; i <= n; i++) {
      for (int j = 0; j < 2; j++) {
        ans[j] = ans[j] ^ h_rootless[i][j];
      }
    }

    if (ord[ans] == 0) ord[ans] = k;
    cout << ord[ans] << endl;
  }

  return 0;
}
