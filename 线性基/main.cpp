//
//  线性基.cpp
//  2022-04-10 21:53
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

// Basis Vector
// 功能：
// 1. 求一组数能异或得到的最小值/最大值，kth小值。  
// 2. 判断一个数能不能被一组数异或得到
// 特性：
// 1. k 小值中，最小值是 0

struct Basis {
  vector<int> a; 
  void insert(int x) {
    for (auto &it : a) x = min(x, x ^ it);
    if (!x) return;
    for (auto &it : a) it = min(it, it ^ x);
    a.push_back(x);
    sort(a.begin(), a.end());
  }

  bool can(int x) {
    for (auto &it : a) x = min(x, x ^ it); 
    return !x;
  }

  int maxn(int x = 0) {
    for (auto &it : a) x = max(x, x ^ it);
    return x;
  }

  int kth(int k) {
    if (k > (1ll << (int) a.size())) return -1; 
    k--; int ans = 0;
    for (int i = 0; i < (int) a.size(); i++) {
      if (k >> i & 1) ans ^= a[i];
    }
    return ans;
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
   
   
  return 0;
}
