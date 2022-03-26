//
//  组合数.cpp
//  2022-03-26 00:52
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;

struct Combination {
  // 务必 #define int long long
  // 适用于小模数和大模数
  // 复杂度 O(n)
  vector<int> fac, cnt;
  int MOD;

  Combination(int n, int p) : fac(n + 1), cnt(n + 1), MOD(p) {
    fac[0] = fac[1] = 1; cnt[0] = cnt[1] = 0;
    for (int i = 2; i <= n; i++) {
      fac[i] = fac[i - 1] * i;
      cnt[i] = cnt[i - 1];
      while (fac[i] % MOD == 0) {
        cnt[i]++;
        fac[i] /= MOD;
      }
      fac[i] %= MOD;
    }
  } 

  int power(int a, int b) {
    assert(b >= 0);
    int base = a % MOD, ans = 1ll;
    while (b) {
      if (b & 1) ans = ans * base % MOD;
      base = base * base % MOD;
      b >>= 1;
    }
    return ans;
  }

  int inv(int x) {
    return power(x, MOD - 2);
  }

  int C(int a, int b) { // C_a^b
    if (b > a) return 0;
    if (cnt[a] != cnt[b] + cnt[a - b]) return 0;
    return fac[a] * inv(fac[b] * fac[a - b] % MOD) % MOD;
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
  return 0;
}

