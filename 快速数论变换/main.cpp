#include <bits/stdc++.h>
using namespace std;

// NTT 快速数论变换 (modint 版)
// 注意:
// 1. 模数 998244353 和 1004535809 的原根是 3
// 2. 998244353 能整除最大 2 ^ 23
// 3. 1004535809 能整除最大 2 ^ 21
// 4. MOD, g, ig 定义成全局 const 会在编译阶段优化取模运算
// 5. NTT 一般比 FFT 快

template <typename T, const T p>
class Modint {
  private:
    T v;
  public:
    constexpr Modint() : v(0) {}
    Modint(const T& x) {
      v = x % p;
      v = v < 0 ? v + p : v;
    }
    const T& operator ()(void) const { return v; }
    Modint operator + (const Modint &a) const {
      return (v + a.v) % p;
    }
    Modint operator - (const Modint &a) const {
      return (v - a.v + p) % p;
    }
    Modint operator -() const {
      return Modint(-v);
    }
    Modint operator * (const Modint &a) const {
      return 1ll * v * a.v % p;
    }
    // 下面是网络比赛专用, 现场比赛不用写
    friend istream& operator >> (istream& io, Modint& a) {
      T x; io >> x;
      a = Modint(x);
      return io;
    }
    friend ostream& operator << (ostream& io, const Modint& a) {
      io << a();
      return io;
    }
};

const int MOD = 998244353;
using mint = Modint<int, MOD>;


const long long g = 3, ig = 332748118;

struct Poly {
  vector<mint> c;
  vector<int> rev;

  Poly(vector<mint> &_c) : c(_c) {
    assert((int) c.size());
  }

  mint power(mint a, mint b) {
    mint base = a, ans = 1;
    while (b()) {
      if (b() & 1) ans = ans * base;
      base = base * base;
      b = b() / 2;
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
      mint step = power(o == 1 ? g : ig, (MOD - 1) / fa);
      for (int i = 0; i < limit; i += fa) {
        mint w = 1;
        for (int j = i; j < i + son; j++) {
          mint x = c[j], y = c[j + son] * w;
          c[j] = x + y;
          c[j + son] = x - y;
          w = w * step;
        }
      }
    }
    if (o == -1) {
      mint ilimit = power(limit, MOD - 2);
      for (int i = 0; i < limit; i++)
        c[i] = c[i] * ilimit;
    }
  }

  friend Poly operator * (Poly a, Poly b) {
    int limit = 1;
    int len = (int) a.c.size() + (int) b.c.size() - 1;
    while (limit < len) limit <<= 1;

    a.dft(limit, 1); b.dft(limit, 1);

    vector<mint> res(limit);
    for (int i = 0; i < limit; i++) {
      res[i] = a.c[i] * b.c[i];
    }

    Poly ans(res);

    ans.dft(limit, -1);
    ans.c.resize(len);
    return ans;
  }

  Poly &operator *= (const Poly &b) {
    return (*this) = (*this) * b;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m; cin >> n >> m;
  vector<mint> a(n + 1), b(m + 1);
  for (int i = 0; i <= n; i++) cin >> a[i];
  for (int i = 0; i <= m; i++) cin >> b[i];

  Poly A(a), B(b);
  Poly C = A * B;

  for (const auto &it : C.c) cout << it << " ";

  return 0;
}
