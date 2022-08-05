//
//  main.cpp
//  2022-07-07 01:02
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 原根
// 功能：
// 1. 求最小原根 O(n ^ 0.5) 跑不满
// 2. 求所有原根 O(nlogn) 跑满
// 注意:
// 1. 求最小原根，如果不存在，返回 -1
// 2. 求所有原根，如果不存在，返回空 vector
// 优化:
// 1. 如果 n 很小(1e6 左右):
//   a. 可以用线性筛预处理欧拉函数和原根是否存在
//   b. 线性筛预处理后可以 logn 枚举一个数的质因子/ 也可以直接埃筛
//   c. 求最小原根复杂度可以降到 O(n ^ 0.25 logn)


struct Primitive_Root {

  // 单次朴素求欧拉函数 O((n ^ 0.5) / logn)
  long long euler_phi(long long n) {
    long long ans = n;
    for (int i = 2; 1ll * i * i <= n; i++) {
      if (n % i == 0) {
        ans = ans / i * (i - 1);
        while (n % i == 0) n /= i;
      }
    }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
  }

  long long power(long long a, long long b, long long p) {
    assert(b >= 0);
    long long base = a, ans = 1ll;
    while (b) {
      if (b & 1ll) ans = 1ll * ans * base % p;
      base = 1ll * base * base % p;
      b >>= 1ll;
    }
    return ans;
  }

  // 判断是否存在原根 O(m ^ 0.5)
  // 即 m 是否属于 2, 4, p^n, 2 * p^n
  bool exist(long long m) {
    if (m == 2 || m == 4) return true;
    if (m % 2 == 0) m /= 2;
    if (m % 2 == 0) return false;
    for (int i = 3; 1ll * i * i <= m; i++) {
      if (m % i == 0) {
        while (m % i == 0) m /= i;
        if (m != 1) return false;
        return true;
      }
    }
    if (m != 1) return true; 
    return false;
  }

  // 求最小原根, 如果不存在原根，返回-1
  // 粗略估计复杂度 O(m ^ 0.5) , 但跑不满，实测很快。
  long long Minimum_root(long long m) {
    if (!exist(m)) return -1;

    long long phi_m = euler_phi(m);

    vector<long long> prime;
    long long val = phi_m;

    for (int i = 2; 1ll * i * i <= val; i++) {
      if (val % i == 0) {
        while (val % i == 0) val /= i;
        prime.push_back(i);
      }
    }

    if (val != 1) prime.push_back(val);

    for (int i = 1; ; i++) {
      if (gcd(i, m) != 1) continue;
      int flag = true;
      for (auto &it : prime) {
        if (power(i, phi_m / it, m) == 1) {
          flag = false;
          break;
        }
      }
      if (flag) return i;
    }
  }

  // 求所有原根
  // 复杂度上界 O(nlogn)
  vector<long long> all_root(long long m) {
    if (!exist(m)) return {};

    long long g = Minimum_root(m);
    vector<long long> root;
    long long phi_m = euler_phi(m); 
    long long res = g;
    
    for (int i = 1; i <= phi_m; i++) {
      if (gcd(i, phi_m) == 1) root.push_back(res);
      res = res * g % m;
    }

    sort(root.begin(), root.end());
    return root;
  }
};

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  Primitive_Root T;

  const int MOD = 998244353;
  int g = T.Minimum_root(MOD);
  cout << g << endl;

  return 0;
}

