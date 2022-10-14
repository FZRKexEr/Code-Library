//
//  main.cpp
//  2022-10-14 14:40
//
//  Created by liznb
//

#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

// modint 类型

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
      return (v + a.v) >= p ? v + a.v - p : v + a.v;
    }
    Modint operator - (const Modint &a) const {
      return v < a.v ? v - a.v + p : v - a.v;
    }
    Modint operator -() const {
      return v ? p - v : v;
    }
    Modint operator * (const Modint &a) const {
      return 1ll * v * a.v % p;
    }
};

using mint = Modint<int, 5>;

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  return 0;
}
