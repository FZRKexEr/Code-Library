#include <bits/stdc++.h>
using namespace std;

// MTT (任意模数NTT)
// 注意:
// 1. 模数 998244353, 1004535809, 469762049 的原根是 3
// 2. 998244353 - 1 能整除最大 2 ^ 23
// 3. 1004535809 - 1 能整除最大 2 ^ 21
// 4. 469762049 - 1 能整除最大 2 ^ 26
// 5. 998244353 * 1004535809 * 469762049 大约 4e26
// 6. 答案必须小于 4e26, 即 n * A * A < 4e26
// 7. MOD, g, ig 定义成全局 const 会在编译阶段优化取模运算
// 8. Mint 必须用构造函数初始化，因为 a, b, c 必须是规范的。
//
// 关于速度:
// 1. 因为做了三次NTT并且最后涉及__int128, 所以速度不快。
// 2. n = 1e5 A = 1e9 时耗时 2s
//
// https://www.luogu.com.cn/problem/P4245

long long power(long long a, long long b, long long MOD) {
  a %= MOD;
  long long ans = 1;
  while (b) {
    if (b & 1) ans = ans * a % MOD;
    a = a * a % MOD;
    b >>= 1;
  }
  return ans;
}

const long long g = 3;
const long long MOD1 = 998244353, MOD2 = 1004535809, MOD3 = 469762049;
const __int128 ALL = (__int128) MOD1 * MOD2 * MOD3;
const long long ig1 = power(g, MOD1 - 2, MOD1), ig2 = power(g, MOD2 - 2, MOD2), ig3 = power(g, MOD3 - 2, MOD3);
const long long P1 = power(MOD2 * MOD3, MOD1 - 2, MOD1);
const long long P2 = power(MOD1 * MOD3, MOD2 - 2, MOD2);
const long long P3 = power(MOD1 * MOD2, MOD3 - 2, MOD3);

struct Mint {
  long long a, b, c;

  Mint() {}
  Mint(long long _a, long long _b, long long _c) {
    a = (_a % MOD1 + MOD1) % MOD1;
    b = (_b % MOD2 + MOD2) % MOD2;
    c = (_c % MOD3 + MOD3) % MOD3;
  }

  long long crt(long long p) {
    __int128 ans = 0;
    ans = (ans + (__int128) MOD2 * MOD3 * a % ALL * P1 % ALL) % ALL;
    ans = (ans + (__int128) MOD1 * MOD3 * b % ALL * P2 % ALL) % ALL;
    ans = (ans + (__int128) MOD1 * MOD2 * c % ALL * P3 % ALL) % ALL;
    return ans % p;
  }

  friend Mint operator + (const Mint x, const Mint y) {
    return {(x.a + y.a) % MOD1, (x.b + y.b) % MOD2, (x.c + y.c) % MOD3};
  }
  friend Mint operator - (const Mint x, const Mint y) {
    return {(x.a - y.a + MOD1) % MOD1, (x.b - y.b + MOD2) % MOD2, (x.c - y.c + MOD3) % MOD3};
  }
  friend Mint operator * (const Mint x, const Mint y) {
    return {x.a * y.a % MOD1, x.b * y.b % MOD2, x.c * y.c % MOD3};
  }
  Mint &operator += (const Mint y) {
    return (*this) = (*this) + y;
  }
  Mint &operator *= (const Mint y) {
    return (*this) = (*this) * y;
  }
};

struct Poly {
  vector<Mint> c;
  vector<int> rev;

  Poly(vector<Mint> &_c) : c(_c) {
    assert((int) c.size());
  }

  Mint power(Mint a, Mint b) {
    Mint ans = {1, 1, 1};
    while (b.a || b.b || b.c) {
      if (b.a && b.a & 1) ans.a = ans.a * a.a % MOD1;
      if (b.b && b.b & 1) ans.b = ans.b * a.b % MOD2;
      if (b.c && b.c & 1) ans.c = ans.c * a.c % MOD3;
      b.a >>= 1, b.b >>= 1, b.c >>= 1;
      a *= a;
    }
    return ans;
  }

  void dft(int limit, int o) {
    c.resize(limit, {0, 0, 0});
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

      Mint b = {(MOD1 - 1) / fa, (MOD2 - 1) / fa, (MOD3 - 1) / fa};
      Mint step = power(o == 1 ? (Mint) {g, g, g} : (Mint) {ig1, ig2, ig3}, b);

      for (int i = 0; i < limit; i += fa) {
        Mint w = {1, 1, 1};
        for (int j = i; j < i + son; j++) {
          Mint x = c[j], y = c[j + son] * w;
          c[j] = x + y;
          c[j + son] = x - y;
          w *= step;
        }
      }
    }
    if (o == -1) {
      Mint ilimit = power({limit, limit, limit}, {MOD1 - 2, MOD2 - 2, MOD3 - 2});
      for (int i = 0; i < limit; i++)
        c[i] = c[i] * ilimit;
    }
  }

  friend Poly operator * (Poly a, Poly b) {
    int limit = 1;
    int len = (int) a.c.size() + (int) b.c.size() - 1;
    while (limit < len) limit <<= 1;

    a.dft(limit, 1); b.dft(limit, 1);

    vector<Mint> res(limit);
    for (int i = 0; i < limit; i++) {
      res[i] = a.c[i] * b.c[i];
    }

    Poly ans(res);

    ans.dft(limit, -1);
    ans.c.resize(len);
    return ans;
  }

  Poly &operator *= (Poly b) {
    return (*this) = (*this) * b;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m, p; cin >> n >> m >> p;
  vector<Mint> a(n + 1), b(m + 1);

  for (auto &it : a) {
    long long val; cin >> val;
    it = {val, val, val};
  }
  for (auto &it : b) {
    long long val; cin >> val;
    it = {val, val, val};
  }

  Poly A(a), B(b);
  A *= B;

  for (int i = 0; i < (int) A.c.size(); i++) {
    cout << A.c[i].crt(p) << " ";
  }

  return 0;
}
