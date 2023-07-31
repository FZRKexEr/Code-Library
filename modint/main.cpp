//
//  main.cpp
//  2022-10-14 14:40
//
//  Created by liznb
//

#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

// modint
//
// 1. 注意 % 会被编译器优化，比 + - 取模还快
// 2. 现场赛请手动输入输出, 可以参照下面的友元函数
//
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

// MOD 自己写
const int MOD = ;
using mint = Modint<int, MOD>;

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  return 0;
}
