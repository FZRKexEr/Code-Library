//
//  main.cpp
//  2022-10-08 14:48
//
//  Created by liznb
//

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

// 多项式求逆
// 核心公式: g(x) = b(x) * (2 - f(x) * b(x)) (mod x ^ n)
// g(x) 是 f(x) mod (x ^ n) 的逆
// b(x) 是 f(x) mod (x ^ (n / 2)) 的逆
// 注意 2 是个常数，可以视为 C0 = 2 其它系数全部是 0 的多项式
//

const long long MOD = 998244353;
const long long g = 3, ig = 332748118;

struct Poly {
  vector<long long> c;
  vector<int> rev;

  Poly() {}
  Poly(vector<long long> &_c) : c(_c) {
    assert((int) c.size());
  }

  long long power(long long a, long long b) {
    long long base = a, ans = 1ll;
    while (b) {
      if (b & 1) ans = 1ll * ans * base % MOD;
      base = 1ll * base * base % MOD;
      b >>= 1;
    }
    return ans;
  }

  void dft(int limit, int o) {
    c.resize(limit, 0);
    rev.resize(limit, 0);

    for (int i = 0; i < limit; i++) {
      rev[i] = rev[i >> 1] >> 1;
      if (i & 1) rev[i] |= limit >> 1;
    }
    for (int i = 0; i < limit; i++) {
      if (i < rev[i]) {
        swap(c[i], c[rev[i]]);
      }
    }

    for (int d = 0; (1 << d) < limit; d++) {
      int son = 1 << d, fa = son << 1;
      long long step = power(o == 1 ? g : ig, (MOD - 1) / fa);
      for (int i = 0; i < limit; i += fa) {
        long long w = 1;
        for (int j = i; j < i + son; j++) {
          long long x = c[j], y = (1ll * c[j + son] * w) % MOD;
          c[j] = (x + y) % MOD;
          c[j + son] = (x - y + MOD) % MOD;
          (w *= step) %= MOD;
        }
      }
    }
    if (o == -1) {
      long long ilimit = power(limit, MOD - 2);
      for (int i = 0; i < limit; i++)
        c[i] = 1ll * c[i] * ilimit % MOD;
    }
  }

  friend Poly operator + (Poly a, Poly b) {
    assert(a.c.size() == b.c.size());
    for (int i = 0; i < (int) a.c.size(); i++) {
      a.c[i] = (a.c[i] + b.c[i] + MOD) % MOD;
    }
    return a;
  }

  friend Poly operator - (Poly a, Poly b) {
    assert(a.c.size() == b.c.size());
    for (int i = 0; i < (int) a.c.size(); i++) {
      a.c[i] = (a.c[i] - b.c[i] + MOD) % MOD;
    }
    return a;
  }

  friend Poly operator * (Poly a, Poly b) {
    int limit = 1;
    int len = (int) a.c.size() + (int) b.c.size() - 1;
    while (limit < len) limit <<= 1;

    a.dft(limit, 1); b.dft(limit, 1);

    vector<long long> res(limit);
    for (int i = 0; i < limit; i++) {
      res[i] = 1ll * a.c[i] * b.c[i] % MOD;
    }

    Poly ans(res);

    ans.dft(limit, -1);
    ans.c.resize(len);
    return ans;
  }

  Poly &operator += (const Poly &b) {
    return (*this) = (*this) + b;
  }

  Poly &operator -= (const Poly &b) {
    return (*this) = (*this) - b;
  }

  Poly &operator *= (const Poly &b) {
    return (*this) = (*this) * b;
  }
  
};

Poly inv(Poly f) {
  int limit = 1, len = f.c.size(); ;
  while (limit < len) limit <<= 1;
  f.c.resize(limit);

  Poly inv; inv.c.emplace_back(f.power(f.c[0], MOD - 2));
  Poly two; two.c.emplace_back(2);

  for (int d = 0; (1 << d) < limit; d++) {
    int son = 1 << d, fa = son << 1;
    while ((int) two.c.size() < fa) two.c.emplace_back(0);

    Poly t;
    t.c.insert(t.c.begin(), f.c.begin(), f.c.begin() + fa);

    inv.c.resize(fa);

    Poly res = t * inv;
    
    res.c.resize(fa);
    inv = inv * (two - res);
    inv.c.resize(fa);
  }

  inv.c.resize(len);
  return inv;
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n; cin >> n;
  vector<int> a(n);

  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  Poly A(a);
  Poly ans = inv(A);

  for (int i = 0; i < n; i++) {
    cout << ans.c[i] << " ";
  }

  return 0;
}
