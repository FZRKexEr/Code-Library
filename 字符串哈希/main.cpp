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
// 1. 纯 int 环境不会错，所以无需 #define int long long
// 2. 注意调用函数是 1 indexed，只有 string 是 0 indexed
// 3. 初始化O(n), 查询子串 hash O(1), 添加长度为 len 的字符串 O(len)
// 4. 不能初始化一个空串
// 5. 关于哈希错误率: 单哈希 1e6 次匹配错误率 0.001, 双哈希 1e6 次匹配错误率 0.000001

struct Hash {
  const array<int, 2> MOD = {127657753, 987654319};
  
  const array<int, 2> P = {137, 277};

  int n;
  string s; // 0 - indexed
  vector<array<int, 2>> hs; // 1 - indexed
  vector<array<int, 2>> pw; // 0 - indexed

  Hash(string _s) {
    assert(_s.length());
    n = _s.length(); 
    s = _s; 
    hs.resize(n + 1);
    pw.resize(n + 1);
    
    pw[0] = {1, 1};
    for (int i = 1; i <= n; i++) {
      for (int j = 0; j < 2; j++) {
        pw[i][j] = 1ll * pw[i - 1][j] * P[j] % MOD[j];
      }
    }
    hs.resize(n + 1, {0, 0}); 
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < 2; j++) {
        hs[i + 1][j] = (1ll * hs[i][j] * P[j] % MOD[j] + s[i]) % MOD[j];
      }
    }
  }

  void add(string _s) {
    assert(_s.length());
    int old_n = n;
    n += _s.length(); 
    s += _s;
    hs.resize(n + 1);
    pw.resize(n + 1);
    for (int i = old_n + 1; i <= n; i++) {
      for (int j = 0; j < 2; j++) {
        pw[i][j] = 1ll * pw[i - 1][j] * P[j] % MOD[j];
      }
    }
    hs.resize(n + 1, {0, 0}); 
    for (int i = old_n; i < n; i++) {
      for (int j = 0; j < 2; j++) {
        hs[i + 1][j] = (1ll * hs[i][j] * P[j] % MOD[j] + s[i]) % MOD[j];
      }
    }
  }

  array<int, 2> get_hash(int l, int r) { // 1 - indexed
    assert(1 <= l && l <= r && r <= n); 
    array<int, 2> ans;
    for (int i = 0; i < 2; i++) {
      ans[i] = (hs[r][i] - 1ll * hs[l - 1][i] * pw[r - l + 1][i] % MOD[i] + MOD[i]) % MOD[i];
    }
    return ans;
  }
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n; cin >> n; 
  vector<int> a;
  
  for (int i = 1; i <= n; i++) {
    string s; 
    cin >> s;
    Hash hs(s);
    a.push_back(hs.get_hash(1, (int) s.length())[0]);
  }

  sort(a.begin(), a.end());
  a.erase(unique(a.begin(), a.end()), a.end());
  
  cout << a.size();
  return 0;
}

