//
//  main.cpp
//  2022-04-11 15:08
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

// 格雷码 Gray Code
// 功能说明：
// 1. 构造 K 位格雷码(一个 1<<k 长的序列, 相邻两个数二进制相差一位) 
// 特性：
// 1. k >=0 

struct Gray {
  vector<int> a; 
  Gray(int k) {
    a.resize(1ll << k, 0);
    for (int i = 1; i < (1ll << k); i++) {
      if (i & 1) a[i] = a[i - 1] ^ 1;
      else a[i] = a[i - 1] ^ (1 << (__builtin_ctzll(a[i - 1]) + 1));
    }
  }
};

signed main() {
  //file();
  ios::sync_with_stdio(false); 
  cin.tie(0);
    
  return 0;
}
