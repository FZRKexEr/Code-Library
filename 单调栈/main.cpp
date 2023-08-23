//
//  main.cpp
//  2023-08-23 22:14
//
//  Created by LiZnB
//

#include <bits/stdc++.h>
using namespace std;
#define endl '\n'

// 单调栈求左右第一个小于/大于的数
// 输入一个排列

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n; cin >> n;
  vector<int> p(n);
  for (int i = 0; i < n; i++) {
    cin >> p[i];
  }

  vector<int> lmin(n, -1), rmin(n, n), lmax(n, -1), rmax(n, n);
  vector<int> smin, smax;
  for (int i = 0; i < n; i++) {
    while (!smin.empty() && p[i] < p[smin.back()]) {
      rmin[smin.back()] = i;
      smin.pop_back();
    }
    if (!smin.empty()) {
      lmin[i] = smin.back();
    }
    smin.push_back(i);
    while (!smax.empty() && p[i] > p[smax.back()]) {
      rmax[smax.back()] = i;
      smax.pop_back();
    }
    if (!smax.empty()) {
      lmax[i] = smax.back();
    }
    smax.push_back(i);
  }

  return 0;
}



