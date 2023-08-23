//
//  main.cpp
//  2022-07-24 20:34
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

#define int long long

// 拉格朗日插值
// 1. 给 n 个点，确定一个最高次是 n - 1 的多项式, 
//    求出这个多项式在位置 k 的值: f(k)。
// 2. 如果 x 是连续的那么就可以 O(n) 求 f(k), 需要推式子。
// 3. 如果 x 是无规律的，可以 O(n ^ 2) 求。
// 4. 需要注意的是传入的点的 x 坐标必须两两不同。
// 5. 公式: f(k) = sum_{0}^{n} (yi \prod_{i != j} ((k - xj) / (xi - xj)))
// 6. a: 0-index

struct Lagrange {
  // 务必使用 #define int long long
  vector<array<int, 2>> a;
  int MOD;

  Lagrange(vector<array<int, 2>> &_a, int _MOD) : a(_a), MOD(_MOD) {}

  int power(int a, int b) {
    assert(b >= 0);
    int base = a, ans = 1;
    while (b) {
      if (b & 1) ans = 1ll * ans * base % MOD;
      base = 1ll * base * base % MOD;
      b >>= 1;
    }
    return ans;
  } 

  int inv(int x) {
    return power(x, MOD - 2);
  }

  // O(n ^ 2), x 无特殊规律。
  int f1(int k) {
    int ans = 0;
    for (int i = 0; i < (int) a.size(); i++) {
      int res = 1;
      for (int j = 0; j < (int) a.size(); j++) {
        if (i == j) continue;
        res = 1ll * res * (k - a[j][0] + MOD) % MOD * power((a[i][0] - a[j][0] + MOD) % MOD, MOD - 2) % MOD;
      }
      ans = (ans + 1ll * res * a[i][1]) % MOD;
    }
    return ans;
  }

  // O(n), X 连续且为 0 到 a.size() - 1
  // 注意是否从小到大排好序了！
  int f2(int k) {
    vector<int> fac(a.size()), pre(a.size()), suf(a.size()); 
    fac[0] = 1;
    pre[0] = k - a[0][0];
    suf.back() = k - (a.back()[0]);
    for (int i = 1; i < (int) fac.size(); i++) {
      fac[i] = fac[i - 1] * i % MOD;
      pre[i] = pre[i - 1] * ((k - a[i][0] + MOD) % MOD) % MOD;
    }
    for (int i = (int) a.size() - 2; i >= 0; i--) {
      suf[i] = suf[i + 1] * ((k - a[i][0] + MOD) % MOD) % MOD;
    }
    int ans = 0ll;
    for (int i = 0; i < (int) a.size(); i++) {
      int res = 1ll;
      if (i) res = res * pre[i - 1] % MOD;
      if (i != (int) a.size() - 1) res = res * suf[i + 1] % MOD;
      res = res * inv(fac[i]) % MOD * inv(fac[(int) a.size() - 1 - i]) % MOD;
      if (((int) a.size() - 1 - i) % 2) res = (res * -1 + MOD) % MOD;
      ans = (ans + a[i][1] * res % MOD) % MOD;
    }
    return ans;
  }

};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n, k; cin >> n >> k;
  vector<array<int, 2>> a(n); 

  for (int i = 0; i < n; i++) {
    cin >> a[i][0] >> a[i][1];
  }

  Lagrange T(a, 998244353);

  cout << T.f1(k) << " " << T.f2(k);
  
  return 0;
}

