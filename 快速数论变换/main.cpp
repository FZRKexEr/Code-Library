#include <bits/stdc++.h>
using namespace std;

// NTT 快速数论变换
// 注意:
// 1. 模数 998244353 和 1004535809 的原根是 3
// 2. 998244353 能整除最大 2 ^ 23
// 3. 1004535809 能整除最大 2 ^ 21
// 4. MOD, g, ig 定义成全局 const 会在编译阶段优化取模运算
// 5. NTT 一般比 FFT 快

const long long MOD = 998244353;
const long long g = 3, ig = 332748118;

struct Poly {
  vector<long long> c;
  vector<int> rev;

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

  Poly &operator *= (Poly b) {
    return (*this) = (*this) * b;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m; cin >> n >> m;
  vector<long long> a(n + 1), b(m + 1);
  for (int i = 0; i <= n; i++) cin >> a[i];
  for (int i = 0; i <= m; i++) cin >> b[i];

  Poly A(a), B(b);
  Poly C = A * B;

  for (auto &it : C.c) cout << it << " ";

  return 0;
}
