//
//  main.cpp
//  2022-11-09 21:16
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define endl '\n'


// 数学常用工具
// 主要目的是给 cf 用, 现场赛可以选择摘抄
// 1. 快速幂
// 2. 组合数 (需要大模数)
// 3. 阶乘
// 4. 逆元
// 5. 线性筛

const int MOD = 998244353;

namespace Math {

  long long power(long long a, long long b) {
    assert(b >= 0);
    long long base = a % MOD, ans = 1ll;
    while (b) {
      if (b & 1) ans = 1ll * ans * base % MOD;
      base = 1ll * base * base % MOD;
      b >>= 1;
    }
    return ans;
  }
  long long inv(long long x) {
    return power(x, MOD - 2);
  }

  vector<int> is, prinum;

  void sieve (int n) {
    is.resize(n + 1, 0);
    prinum.reserve(n + 1);
    for (int i = 2; i <= n; i++) {
      if (!is[i]) prinum.push_back(i);
      for (const auto &it : prinum) {
        if (it * i > n) break;
        is[i * it] = 1;
        if (i % it == 0) break;
      }
    }
  }

  vector<int> fac;
  void get_fac(int n) {
    fac.resize(n + 1, 1);
    for (int i = 1; i <= n; i++) {
      fac[i] = 1ll * fac[i - 1] * i % MOD;
    }
  }
  int C(int a, int b) {
    if (b > a) return 0;
    assert(max(a, b) < (int) fac.size());
    return 1ll * fac[a] * inv(fac[a - b]) % MOD * inv(fac[b]) % MOD;
  }
}


signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);


  return 0;
}
