#include <bits/stdc++.h>
using namespace std;

// 多项式ln
// 求 G(x) = lnA(x)
// G'(x) = A'(x) / A(x) = A'(x) * invA(x)
// 然后积分即可
// 
// 特性：
// 1. A(0) 必须等于 1
// 2. G(0) 默认等于 0
//
// 测试数据
// input
// 5
// 1 2 3 4 5
// output
// 0 2 1 665496236 499122177

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

  Poly() {}
  Poly(vector<mint> &_c) : c(_c) {}

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

  Poly inv() {
    int limit = 1, len = c.size();;
    while (limit < len) limit <<= 1;
    c.resize(limit, 0);

    Poly ans, f;
    ans.c.emplace_back(power(c[0], MOD - 2));

    for (int son = 1, fa = son << 1; son < limit; son <<= 1, fa <<= 1) {
      f.c.assign(c.begin(), c.begin() + fa);
      f.c.insert(f.c.begin() + fa, fa, 0);
      ans.c.resize(fa << 1, 0);

      ans.dft(fa << 1, 1), f.dft(fa << 1, 1);

      for (int i = 0; i < (fa << 1); i++) {
        ans.c[i] = ((mint)2 - f.c[i] * ans.c[i]) * ans.c[i];
      }
      ans.dft(fa << 1, -1);
      ans.c.resize(fa);
    }
    ans.c.resize(len);
    c.resize(len);
    return ans;
  }

  // 注意，多项式求导和积分都很容易。
  Poly ln() {
    int len = c.size();
    Poly da(c), inva = inv();
    for (int i = 0; i < (int) da.c.size(); i++) {
      da.c[i] = da.c[i + 1] * (i + 1);
    }
    da.c.pop_back();
    Poly ans = da * inva;
    ans.c.resize(len);
    for (int i = len - 2; i >= 0; i--) {
      ans.c[i + 1] = ans.c[i] * power(i + 1, MOD - 2);
    }
    ans.c[0] = 0;
    return ans;
  }
};

int main() {

  ios::sync_with_stdio(false);
  cin.tie(0);

  int n; cin >> n;

  vector<mint> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  Poly A(a);
  A = A.ln();
  for (int i = 0; i < n; i++) {
    cout << A.c[i] << " ";
  }
  return 0;
}
