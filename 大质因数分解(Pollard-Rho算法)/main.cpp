//
//  main.cpp
//  2022-07-07 20:28
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// Pollard_Rho 算法
// 期望复杂度 O(n ^ 0.25)
// 来自 Rockdu

namespace NT {
  long long gcd(long long a, long long b) {
    return b ? gcd(b, a % b) : a;
  }
  long long mul(long long a, long long b, long long m) {
    long long s = a * b - (long long)((long double)a * b / m + 0.5) * m;
    return s < 0 ? s + m : s;
  }
  long long fpow(long long x, long long a, long long m) {
    long long ans = 1;
    while(a) {
      if(a & 1) ans = mul(ans, x, m);
      x = mul(x, x, m), a >>= 1;
    }
    return ans;
  }
}

namespace Miller_Rabin {
  using namespace NT;
  long long p[15] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  int detect(long long n, long long a) {
    if(n == a) return 1;
    if(a % n == 0) return 1;
    long long now = n - 1, lst = 1;
    if(fpow(a, now, n) != 1) 
      return 0;
    while(!(now & 1)) {
      now /= 2;
      long long fp = fpow(a, now, n);
      if(lst == 1 && fp != 1 && fp != n - 1)
        return 0;
      lst = fp;
    }
    return 1;
  }
  
  long long MR(long long n) {
    if(n < 2) return 0;
    for(int i = 0; i < 7; ++i) {
      if(!detect(n, p[i])) 
        return 0;
    }
    return 1;
  }
}

namespace Pollard_Rho {
  using namespace NT;
  using namespace Miller_Rabin;
  long long f(long long x, long long C, long long p) {
    return (mul(x, x, p) + C) % p;
  }
  long long Rand() {return (rand() << 15) + rand();}
  long long Randll() {return (Rand() << 31) + Rand();}
  
  long long PR(long long n) {
    if(n == 4) return 2;
    if(MR(n)) return n;
    while(1) {
      long long C = Randll() % (n - 1) + 1;
      long long s = 0, t = 0;
      long long acc = 1;
      do {
        for(int i = 1; i <= 128; ++i) {
          t = f(f(t, C, n), C, n);
          s = f(s, C, n);
          long long tmp = mul(acc, abs(t - s), n);
          if(s == t || tmp == 0)
            break;
          acc = tmp;
        }
        long long d = gcd(n, acc);
        if(d > 1) return d;
      } while(s != t);
    }
  }
  
  // 返回的 first 是质因子, second 是质因子个数, 从小到大排序。
  typedef pair<long long, int > pii;
  vector<pii> DCOMP(long long n) {
    vector<pii> ret;
    while(n != 1) {
      long long p = PR(n);
      while(!MR(p)) 
        p = PR(p);
      int c = 0;
      while(n % p == 0) n /= p, ++c;
      ret.push_back(make_pair(p, c));
    }
    sort(ret.begin(), ret.end());
    return ret;
  }
}

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int z; cin >> z; 
  
  while (z--) {
    long long n; cin >> n;
    auto res = Pollard_Rho::DCOMP(n); 

    int cnt = 0;
    for (auto &it : res) cnt += it.second;
    // cnt 等于 1 说明 n 是质数。
    if (cnt == 1) cout << "Prime" << endl;
    else cout << res.back().first << endl;
  }

  return 0;
}

