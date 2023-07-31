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
// 1.https://www.luogu.com.cn/problem/P5043 (本题，题目很水，测不出hash强度)
// 2.https://codeforces.com/gym/104090/problem/G (简单 hash 会被卡)
//
// 哈希方式: f(pos) = 1 + sum{h(f(v))} mod MOD
//
// 功能:
// 1. 有根树哈希(单哈) O(n)
// 2. 无根树哈希(单哈) O(n)
//
// 注意
// 1. 这个单哈也很难卡，如果实在不放心，就改改 MOD 再跑一次。
// 2. 有根树 hash 更考验 hash 强度。无根树 hash 乱哈都能过。
// 

int single(int n, vector<vector<int>> &g);

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int m; cin >> m;

  map<int, int> ord;

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

    int ans = single(n, g);

    if (ord[ans] == 0) ord[ans] = k;
    cout << ord[ans] << endl;
  }

  return 0;
}


// 单哈版
int single(int n, vector<vector<int>> &g) {

  // hash 开始
  const int MOD = 998244353;

  // 经过实践，这个 hash 函数很难卡。
  auto h = [&](int x) {
    return (1ll * x * x % MOD * x % MOD * 1237123 % MOD + 19260817) % MOD;
  };

  // h_pro(比 h 慢4被左右，但是几乎冲突不了, 最好套个快读) 可以用来看是不是 h 弱了导致 wa. 注意返回的是 unsigned int
  auto h_pro = [&](int x) { mt19937 rng(x); return rng(); };

  vector<int> h_root(n + 1), h_rootless(n + 1);
  // 有根树 hash
  function<void(int, int)> get_hash = [&](int pos, int fa) {
    h_root[pos] = 1;
    for (auto &v : g[pos]) {
      if (v == fa) continue;
      get_hash(v, pos);
      h_root[pos] = (1ll * h_root[pos] + h(h_root[v])) % MOD;
    }
  };
  get_hash(1, -1);
  // 无根树 hash, 换根dp, 需要先做一次有根树 hash
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

  return res;
}
