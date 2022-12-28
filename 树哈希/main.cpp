//
//  main.cpp
//  2022-12-28 17:55
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define endl '\n'

// 树哈希
// 
// https://www.luogu.com.cn/problem/P5043
//
// 哈希方式: f(pos) = 1 + sum{h(f(v))} mod MOD
//
// 功能:
// 1. 有根树哈希(双哈) O(n)
// 2. 无根树哈希(双哈) O(n)
//
// 注意
// 1. 如果嫌麻烦就直接写单哈, 目测单哈也不会冲突
// 2. MOD = (1ll << 31) - 1, 不要随便改, 这个值和两个 P 配合可以保证随机数周期很长

int single(int n, vector<vector<int>> &g);

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

    assert(ans[0] == single(n, g));

    if (ord[ans] == 0) ord[ans] = k;
    cout << ord[ans] << endl;
  }

  return 0;
}


// 单哈版
int single(int n, vector<vector<int>> &g) {

  // hash 开始
  const int MOD = 2147483647;
  const int P = 48271;

  auto h = [&](int x) {
    return 1ll * x * P % MOD;
  };
  vector<int> h_root(n + 1), h_rootless(n + 1);

  function<void(int, int)> get_hash = [&](int pos, int fa) {
    h_root[pos] = 1;
    for (auto &v : g[pos]) {
      if (v == fa) continue;
      get_hash(v, pos);
      h_root[pos] = (1ll * h_root[pos] + h(h_root[v])) % MOD;
    }
  };
  get_hash(1, -1);

  int res = 0;
  function<void(int, int)> get_hash_rootless = [&](int pos, int fa) {
    res = res ^ h_rootless[pos];
    for (auto &v : g[pos]) {
      if (v == fa) continue;
      h_rootless[v] = (h((h_rootless[pos] - h(h_root[v]) + MOD) % MOD) + h_root[v]) % MOD;
      get_hash_rootless(v, pos);
    }
  };
  h_rootless[1] = h_root[1];
  get_hash_rootless(1, -1);

  int ans = 0;
  for (int i = 1; i <= n; i++) {
    ans = ans ^ h_rootless[i];
  }
  return ans;
}



