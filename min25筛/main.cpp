#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

// Min_25 筛
//
// 1. 复杂度 O(n^{0.75} / logn)
// 2. n = 1e10 632ms 单个位置
// 3. n = 1e10 909ms sqrt(n) 个位置
// 3. 速度敏感, 不要 define int long long
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
    friend Modint operator + (const Modint &a, const Modint &b) {
      return (a.v + b.v) % p;
    }
    friend Modint operator - (const Modint& a, const Modint &b) {
      return (a.v - b.v + p) % p;
    }
    friend Modint operator * (const Modint &a, const Modint &b) {
      return 1ll * a.v * b.v % p;
    }
    Modint operator -() const {
      return Modint(-v);
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
  vector<int> prinum, is;
  vector<mint> sp1, sp2; 

  void sieve(int n) {
    is.assign(n + 1, 0);
    sp1.clear(); sp2.clear(); prinum.clear();
    prinum.emplace_back(0);
    sp1.emplace_back(0);
    sp2.emplace_back(0);

    for (int i = 2; i <= n; i++) {
      if (is[i] == 0) {
        sp1.emplace_back(sp1.back() + i);
        sp2.emplace_back(sp2.back() + (mint)i * i);
        prinum.push_back(i);
      }
      for (int j = 1; j < (int) prinum.size(); j++) {
        if (1ll * prinum[j] * i > n) break;
        is[prinum[j] * i] = 1;
        if (i % prinum[j] == 0) break;
      }
    }
  }

  mint solve(long long n) {
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

    vector<mint> g1(dis.size()), g2(dis.size());

    for (int i = 0; i < (int) dis.size(); i++) {
      // 注意减去 F(1)
      mint val = dis[i];
      g1[i] = (1 + val) * val * INV2 - 1;
      g2[i] = val * (val + 1) * (2 * val + 1) * INV6 - 1;
    }

    for (int i = 1; i <= maxp; i++) {
      for (int j = 0; j < (int) dis.size(); j++) {
        if (1ll * prinum[i] * prinum[i] > dis[j]) break;
        int goal = get_idx(dis[j] / prinum[i]);
        g1[j] = g1[j] - (mint) prinum[i] * (g1[goal] - sp1[i - 1]);
        g2[j] = g2[j] - (mint) prinum[i] * prinum[i] * (g2[goal] - sp2[i - 1]);
      }
    }

    // 注释部分是求单次 min25, 比求 sqrt(n) 个位置 min25 快
    // function<mint(long long, int)> S = [&] (long long x, int y) {
    //   if (prinum[y] >= x) return (mint)0;
    //   int ord = get_idx(x);

    //   // 单项式合成多项式需要修改, 这里是 S(n, j) 的前两项
    //   mint ans = (g2[ord] - g1[ord]) - (sp2[y] - sp1[y]);

    //   for (int i = y + 1; i <= maxp && 1ll * prinum[i] * prinum[i] <= x; i++) {
    //     long long pe = prinum[i]; // pe 不能取模, 且无需修改
    //     for (int e = 1; pe <= x; e++, pe = pe * prinum[i]) {
    //       // f(p_k^e) 部分需要修改，本题中是 val * (val - 1)
    //       mint val = pe;
    //       ans = ans + val * (val - 1) * (S(x / pe, i) + (e != 1));
    //     }
    //   }
    //   return ans;
    // };
    // return S(n, 0) + 1;

    // dis[i] 是 n 的一个整除结果
    // g1[i] 的意义是前 dis[i] 里面的质数  p 之和
    // g2[i] 的意义是前 dis[i] 里面的质数  p*p 之和
    vector<mint> s(dis.size());
    for (int i = 0; i < (int)dis.size(); i++) {
      // g2[i] - g1[i] 需要修改, 意义是前 dis[i]里面的质数的 f(p) 之和
      s[i] = g2[i] - g1[i];
    }

    for (int i = maxp; i >= 1; i--) {
      for (int j = 0; j < (int) dis.size() && 1ll * prinum[i] * prinum[i] <= dis[j]; j++) {
        long long pe = prinum[i];
        for (int e = 1; pe * prinum[i] <= dis[j]; e++, pe = pe * prinum[i]) {
          // sp2[i] - sp1[i] 需要修改, 意义是前 i个质数的 f(p) 之和
          // (mint) pe * (pe - 1) 和 (mint)(pe * prinum[i]) * (pe * prinum[i] - 1) 需要修改
          // 意义是 f(p^e)和 f(p^{e + 1}) 的值，这需要快速求出。 在这个例子里，f(p^e) = p^e * (p^e - 1)
          s[j] = s[j] + (mint) pe * (pe - 1) * (s[get_idx(dis[j] / pe)] - (sp2[i] - sp1[i])) + (mint)(pe * prinum[i]) * (pe * prinum[i] - 1);
        }
      }
    }

    return s[get_idx(n)] + 1;
  }
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  long long n; cin >> n;
  cout << Min_25::solve(n) << endl;

  return 0;
}

