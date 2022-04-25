//
//  main.cpp
//  2022-04-25 00:04
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;


// 说明:
// 1. 没有在纯 int 环境测试，所以最好开 #define int long long
// 2. 注意调用函数是 1 indexed，只有 string 是 0 indexed
// 3. 初始化O(n), 查询子串 hash O(1) 

struct Hash {
  const array<int, 2> MOD = {127657753, 987654319};
  const array<int, 2> P = {137, 277};

  int n;
  string s; // 0 - indexed
  vector<array<int, 2>> hs; // 1 - indexed
  vector<array<int, 2>> pw; // 0 - indexed

  Hash(string _s) {
    n = _s.length(); 
    s = _s; 
    hs.resize(n + 1);
    pw.resize(n);
    
    pw[0] = P;
    for (int i = 1; i < n; i++) {
      for (int j = 0; j < 2; j++) {
        pw[i][j] = 1ll * pw[i - 1][j] * P[j] % MOD[j];
      }
    }
    hs.resize(n + 1, {0, 0}); 
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < 2; j++) {
        hs[i + 1][j] = (hs[i][j] + 1ll * pw[n - i - 1][j] * s[i] % MOD[j]) % MOD[j]; 
      }
    }
  }
  array<int, 2> get_hash(int l, int r) { // 1 - indexed
    assert(1 <= l && l <= r && r <= n); 
    array<int, 2> ans;
    for (int i = 0; i < 2; i++) {
      ans[i] = (hs[r][i] - 1ll * hs[l - 1][i] * pw[r - l + 1][i] + MOD[i]) % MOD[i];
    }
    return ans;
  }
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  

  return 0;
}

