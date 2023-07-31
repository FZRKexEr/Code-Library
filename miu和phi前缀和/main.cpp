//
//  main.cpp
//  2022-10-15 01:26
//  Created by liznb
//
#include <bits/stdc++.h>
#define endl '\n'
using namespace std;
#define int long long

// 莫比乌斯函数 和 欧拉函数前缀和 (min25)
// 1. 2 ^ 31 内无需取模
// 2. 莫比乌斯函数前缀和思路: 
//    f(p) = -1, 显然是个常数单项式,
//    令 F(p) = 1, 求质数个数即可。
// 3. 欧拉函数前缀和思路
//    f(p) = p - 1, f(p^k) = p^k - p^(k-1) 显然是低次多项式
//    令 F(p) = 1 和 F(p) = p, 就行
//
// 题目: https://www.luogu.com.cn/problem/P4213

namespace Min_25 {
  vector<int> prinum, is;
  vector<long long> sp;

  void sieve(int n) {
    is.assign(n + 1, 0);
    prinum.assign(1, 0);
    sp.assign(1, 0);

    for (int i = 2; i <= n; i++) {
      if (is[i] == 0) {
        sp.emplace_back(sp.back() + 1ll * i);
        prinum.emplace_back(i);
      }
      for (int j = 1; j < (int) prinum.size(); j++) {
        if (1ll * prinum[j] * i > n) break;
        is[prinum[j] * i] = 1;
        if (i % prinum[j] == 0) break;
      }
    }
  }

  void solve(long long n) {
    long long limit = sqrt(n);
    sieve(limit);

    int maxp = (int) prinum.size() - 1;

    vector<long long> dis;
    vector<int> idx[2];
    idx[0].resize(limit + 1);
    idx[1].resize(limit + 1);

    auto get_idx = [&](long long x) {
      return x <= limit ? idx[0][x] : idx[1][n / x];
    };

    for (long long i = 1, nxt; i <= n; i = nxt) {
      nxt = n / (n / i) + 1;
      dis.emplace_back(n / i);
      if (n / i <= limit) idx[0][n / i] = (int) dis.size() - 1;
      else idx[1][n / (n / i)] = (int) dis.size() - 1;
    }

    vector<long long> g0(dis.size()), g1(dis.size());

    for (int i = 0; i < (int) dis.size(); i++) {
      long long val = dis[i];
      g0[i] = val - 1;
      g1[i] = (2 + val) * (val - 1) / 2;
    }

    for (int i = 1; i <= maxp; i++) {
      for (int j = 0; j < (int) dis.size(); j++) {
        if (1ll * prinum[i] * prinum[i] > dis[j]) break;
        int goal = get_idx(dis[j] / prinum[i]);
        g0[j] = g0[j] - (long long) 1 * (g0[goal] - (i - 1));
        g1[j] = g1[j] - (long long) prinum[i] * (g1[goal] - sp[i - 1]);
      }
    }

    function<long long(long long, int)> Smiu = [&] (long long x, int y) {
      if (prinum[y] >= x) return (long long)0;
      int ord = get_idx(x);
      long long ans = -g0[ord] - -y;
      for (int i = y + 1; i <= maxp && 1ll * prinum[i] * prinum[i] <= x; i++) {
        long long pe = prinum[i];
        ans = ans + -1 * Smiu(x / pe, i);
      }
      return ans;
    };

    function<long long(long long, int)> Sphi = [&] (long long x, int y) {
      if (prinum[y] >= x) return (long long)0;
      int ord = get_idx(x);
      long long ans = (g1[ord] - g0[ord]) - (sp[y] - y);
      for (int i = y + 1; i <= maxp && 1ll * prinum[i] * prinum[i] <= x; i++) {
        long long pe = prinum[i];
        for (int e = 1; pe <= x; e++, pe = pe * prinum[i]) {
          long long val = pe;
          ans = ans + (val - (val / prinum[i])) * (Sphi(x / pe, i) + (e != 1));
        }
      }
      return ans;
    };

    cout << Sphi(n, 0) + 1 << " " << Smiu(n, 0) + 1 << endl;
  }
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int z; cin >> z;
  while (z--) {
    long long n; cin >> n;
    Min_25::solve(n);
  }

  return 0;
}
