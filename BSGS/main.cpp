//
//  main.cpp
//  2022-08-05 13:59
//
//  Created by liznb
//
#include <bits/stdc++.h>
using namespace std;

// 朴素 BSGS
// 1. a ^ x = b (mod p)
// 2. MOD 必须是质数
// 3. 复杂度 O(p ^ 0.5)

struct BSGS {
  long long MOD, a, b;
  vector<long long> x;

  long long power(long long a, long long b) {
    assert(b >= 0);
    long long base = a, ans = 1ll;
    while (b) {
      if (b & 1) ans = 1ll * ans * base % MOD;
      base = 1ll * base * base % MOD;
      b >>= 1;
    }
    return ans;
  }

  BSGS(long long _MOD, long long _a, long long _b) : MOD(_MOD), a(_a), b(_b) {
    long long limit = ceil(sqrt(MOD));
    b %= MOD;
    map<long long, long long> hash;
    for (long long i = 0ll, val = b; i <= limit; i++) {
      hash[val] = i;
      (val *= a) %= MOD;
    }

    long long step = power(a, limit);

    for (long long i = 0ll, val = 1; i <= limit; i++) {
      if (hash.find(val) != hash.end()) {
        long long res = i * limit - hash[val];
        if (res >= 0) x.push_back(res);
      }
      (val *= step) %= MOD;
    }
    sort(x.begin(), x.end());
  }
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int p, a, b; cin >> p >> a >> b;
  BSGS T(p, a, b);

  if (T.x.size() == 0) cout << "no solution";
  else cout << T.x[0];

  return 0;
}

