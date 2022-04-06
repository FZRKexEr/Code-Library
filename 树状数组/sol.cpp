//
//  sol.cpp
//  2022-04-06 17:22
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
#define LOCAL
using namespace std;

// 二元索引树, Binary Indexed Tree, Fenwick树, 树状数组
// 特性:
// 1. 只能处理具有前缀和性质数组。
// 2. 下标绝对不能为 0
struct BIT {
  vector<int> tree;
  int limit;
  BIT (int n) : tree(n + 1, 0), limit(n) {}

  inline int lowbit(int x) { return x & (-x); }

  void modify(int x, int val) { // 单点修改
    assert(x >= 1);
    while (x <= limit) {
      tree[x] += val;     
      x += lowbit(x);
    }
  }

  int query(int x) {
    assert(x >= 0);
    int ans = 0;
    while (x) {
      ans += tree[x];
      x -= lowbit(x);
    }
    return ans;
  }

  int sum(int l, int r) {
    if (r < l) return 0;
    assert(l >= 1 && r >= 1);
    return query(r) - query(l - 1); 
  }
};

signed main() {
  //file();
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
   
  return 0;
}
