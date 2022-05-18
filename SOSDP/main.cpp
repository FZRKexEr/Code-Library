//
//  main.cpp
//  2022-05-18 16:59
//
//  Created by liznb
//  
//  https://codeforces.com/contest/165/problem/E


#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n; cin >> n;

  int limit = (1ll << 22) - 1;
  vector<int> a(limit + 1, -1);
  vector<int> b(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> b[i];
    a[b[i]] = b[i];
  }
  
  // SOSDP
  // 适用情况：
  // 1. 求子集的和/最大值/最小值/积
  // 2. 一般不能直接看出来用 SOSDP 需要转化
  //
  // 原理:
  // f[mask][j] 表示与 mask 前 j 位(从0位开始) 不同的子集(也可以完全相同)的答案统计 。
  // 两种转移:
  // 1. f[mask][j] <= f[mask][j - 1] 无论第 j 位是 0/1 第 j 位不变。
  // 2. f[mask][j] <= f[mask ^ (1 << j)][j - 1] (条件 mask >> j & 1) 让第 j 位变成 0.
  //
  // 理解：
  // SOSDP 也可以父集枚举
  // SOSDP 实质上是 FWT 或的正变换
  //
  // 下面是是简易写法
  // 注意: 
  // 1. 22 和 limit 一定要跑满。即使不是简易写法也要跑满。
  // 2. 一定从 0 开始
  // 3. 第二层循环顺序并不重要，这不是背包。只存在 0->1
  //
  for (int i = 0; i <= 22; i++) {
    for (int j = 0; j <= limit; j++) {
      // 这里 max 也可以是 min, +=, *= 之类的。
      a[j] = max((j >> i & 1) ? a[j ^ (1 << i)] : -1, a[j]);
    }
  }

  for (int i = 1; i <= n; i++) {
    int val = (~b[i]) & ((1ll << 22) - 1);
    assert(val < limit);
    cout << a[val] << " ";
  }
  
  return 0;
}

