//
//  main.cpp
//  2022-07-07 19:38
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 1. 筛法求欧拉函数 O(n)
struct Euler {
  vector<int> phi, is, prinum;
  
  Euler(int n) : phi(n + 1), is(n + 1, 0) {
    prinum.reserve(n + 1); 
    phi[1] = 1;
    for (int i = 2; i <= n; i++) {
      if (!is[i]) prinum.push_back(i), phi[i] = i - 1;
      for (auto &it : prinum) {
        if (it * i > n) break;
        is[i * it] = 1;
        if (i % it == 0) {
          phi[i * it] = phi[i] * it;
          break;
        } else {
          phi[i * it] = phi[i] * (it - 1);
        }
      }
    }
  }
};

// 2. 单点欧拉函数 O(sqrt(n)) (朴素)
long long euler_phi(long long n) {
  long long ans = n;
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      ans = ans / i * (i - 1);
      while (n % i == 0) n /= i;
    }
  }
  if (n > 1) ans = ans / n * (n - 1);
  return ans;
}

// 3. 单点欧拉函数 O(n ^ 0.25) (PR 优化)
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
  int detect(long n, long long a) {
    if(n == a) return 1;
    if(a % n == 0) return 1;
    long long now = n - 1, lst = 1;
    if(fpow(a, now, n) != 1) 
      return 0;
    while(!(now & 1)) {
      now /= 2;
      long long p = fpow(a, now, n);
      if(lst == 1 && p != 1 && p != n - 1)
        return 0;
      lst = p;
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

long long euler_phi_pr(long long n) {
  auto prime = Pollard_Rho::DCOMP(n);
  long long ans = n;
  for (auto &it : prime) {
    ans = ans / it.first * (it.first - 1);  
  }
  return ans;
}


int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

