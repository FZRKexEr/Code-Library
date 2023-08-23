//
//  main.cpp
//  2022-09-06 19:41
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 动态开点树状数组
// 说明：
// 1. 单点修改 区间查询
// 2. pbds 或者 unordered_map 会快一点
// 3. CF C++ 20 使用 pbds 会 CE
// 4. 范围大时，速度很慢!, 比动态开点线段树慢, 如果需要更快，
//    可以考虑把第一维换成普通树状数组。
//
// 注意! 
// 1. 如果区间范围不在 int 内，最好 define int long long
// 2. 一维动态开点，首选动态开点线段树, 或者离散化。
//
// 附 pbds:
// #include <bits/extc++.h>
// using namespace __gnu_pbds;
// gp_hash_table<int, T> tree;

template<class T>
struct BIT {

  unordered_map<int, T> tree;
  int n;

  BIT() {}
  BIT(int _n) : n(_n) {}

  void modify(int x, T val) {
    assert(x >= 1);
    while (x <= n) {
      tree[x] += val;
      x += x & -x;
    }
  }

  T query(int x) {
    assert(x >= 0);
    T ans = 0;
    while (x) {
      ans += tree[x];
      x -= x & -x;
    }
    return ans;
  }
};

// 二维动态开点树状数组
// 单点修改，区间查询
template<class T>
struct BIT2d {

  unordered_map<int, BIT<T>> tree;

  int n, m;

  BIT2d(int _n, int _m) : n(_n), m(_m) {}
  
  void modify(int x, int y, T val) {
    assert(x >= 1 && y >= 1);
    while (x <= n) {
      if (tree.find(x) == tree.end()) tree[x].n = m;
      tree[x].modify(y, val);
      x += x & -x;
    }
  }

  T query(int x, int y) {
    assert(x >= 0 && y >= 0);
    T ans = 0;
    while (x) {
      if (tree.find(x) == tree.end()) tree[x].n = m;
      ans += tree[x].query(y);
      x -= x & -x;
    }
    return ans;
  }

  T get_range(int x1, int y1, int x2, int y2) {
    return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  BIT2d<long long> T(1e9, 1e9);

  T.modify((int) 1e9 - 1, (int) 1e9 - 1, 100);

  cout << T.get_range(1, 1, (int) 1e9, (int) 1e9);

  return 0;
}

