//
//  main.cpp
//  2022-07-08 20:30
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

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

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  cout << power(3, MOD - 2);

  return 0;
}

