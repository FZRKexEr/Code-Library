//
//  main.cpp
//  2022-10-12 15:51
//  Created by liznb
//
#include <bits/stdc++.h>
#define endl '\n'
using namespace std;
#define int long long

// Min_25 筛
//
// 1. 复杂度 O(n^{0.75} / logn)
// 2. 实测 n = 1e10 32位编译器 632ms
// 3. 要求:
//    a. f(p^k) 能快速求出来
//    b. f(p) 最好是低阶多项式
//    c. 构造的 F(p) 是完全积性函数，例如 F(i)=i^k

const int MOD = 1'000'000'000 + 7;
const int INV6 = 166666668;
const int INV2 = 500000004;

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
    bool operator == (const Modint &b) const {
      return v == b.v;
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

using mint = Modint<long long, MOD>;

namespace Min_25 {
  // prinum, sp 从 1 开始
  vector<int> prinum, is;
  array<vector<mint>, 2> sp; // sp[0/1] 表示 p^1 和 p^2 的前缀和

  void sieve(int n) {
    is.assign(n + 1, 0);
    prinum.clear(), sp[0].clear(), sp[1].clear();
    prinum.emplace_back(0);
    sp[0].emplace_back(0);
    sp[1].emplace_back(0);

    for (int i = 2; i <= n; i++) {
      if (is[i] == 0) {
        // sp 需要修改，代表 sum of prime
        sp[0].emplace_back(sp[0].back() + i);
        sp[1].emplace_back(sp[1].back() + (mint)i * i);
        prinum.push_back(i);
      }
      for (int j = 1; j < (int) prinum.size(); j++) {
        if (1ll * prinum[j] * i > n) break;
        is[prinum[j] * i] = 1;
        if (i % prinum[j] == 0) break;
      }
    }
  }

  long long solve(long long n) {
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

    vector<array<mint, 2>> g(dis.size());

    // 初始化 g(i, 0)，需要修改
    for (int i = 0; i < (int) dis.size(); i++) {
      mint val = dis[i];
      g[i][0] = (1 + val) * val * INV2 - 1;
      (g[i][1] = val * (val + 1) % MOD * ((2 * val % MOD + 1) % MOD) % MOD * INV6 % MOD - 1 + MOD) %= MOD;
    }

    for (int i = 1; i <= maxp; i++) { 
      for (int j = 0; j < (int) dis.size(); j++) {
        if (1ll * prinum[i] * prinum[i] > dis[j]) break;
        int goal = get_idx(dis[j] / prinum[i]);
        // f(p) 部分需要修改, 本题中是 p ^ 1 和 p ^ 2
        g[j][0] = (g[j][0] - (1ll * prinum[i] * ((g[goal][0] - sp[0][i - 1] + MOD) % MOD) % MOD) + MOD) % MOD;
        g[j][1] = (g[j][1] - (1ll * prinum[i] * prinum[i] % MOD * ((g[goal][1] - sp[1][i - 1] + MOD) % MOD) % MOD) + MOD) % MOD;
      }
    }

    function<long long(long long, int)> S = [&] (long long x, int y) {
      if (prinum[y] >= x) return 0ll;
      int ord = get_idx(x);

      // 单项式合成多项式需要修改, 这里是 S(n, j) 的前两项
      long long ans = ((g[ord][1] - g[ord][0] + MOD) % MOD - ((sp[1][y] - sp[0][y] + MOD) % MOD) + MOD) % MOD;

      for (int i = y + 1; i <= maxp && 1ll * prinum[i] * prinum[i] <= x; i++) {
        long long pe = prinum[i];
        for (int e = 1; pe <= x; e++, pe = pe * prinum[i]) {
          long long val = pe % MOD;
          // f(p_k^e) 部分需要修改，本题中是 val * (val - 1)
          (ans += val * ((val - 1 + MOD) % MOD) % MOD * (S(x / pe, i) + (e != 1)) % MOD) %= MOD;
        }
      }
      return ans;
    };

    return (S(n, 0) + 1) % MOD;
  }
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  long long n; cin >> n;
  cout << Min_25::solve(n) << endl;

  return 0;
}
