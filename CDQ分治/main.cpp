//
//  main.cpp
//  2022-07-14 00:21
//
//  Created by liznb
//  

#include <bits/stdc++.h>
using namespace std;

#define int long long

// CDQ分治
// 1. 需要先把问题转化成三维偏序问题。
// 2. 第一维排序，第二维双指针，第三维 线段树/树状数组
// 3. 注意下标！
//
// 例题: 二维最长不下降序列

struct BIT {
  vector<int> tree;
  int limit;
  BIT (int n) : tree(n + 1, 0), limit(n) {}

  inline int lowbit(int x) { return x & (-x); }

  void modify(int x, int val) { // 单点修改
    assert(x >= 1);
    while (x <= limit) {
      tree[x] = max(val, tree[x]);     
      x += lowbit(x);
    }
  }

  void init(int x) {
    assert(x >= 1);
    while (x <= limit) {
      tree[x] = 0;
      x += lowbit(x);
    }
  }

  int query(int x) {
    assert(x >= 0);
    int ans = 0;
    while (x) {
      ans = max(tree[x], ans);
      x -= lowbit(x);
    }
    return ans;
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
   
  int z; cin >> z; 
  while (z--) {
    int n; cin >> n;
    vector<array<int, 3>> a(n + 1); 
    vector<int> dis;
    for (int i = 1; i <= n; i++) {

      // 注意下标，CDQ分治排序后下标会乱, 一定需要保存下标。
      a[i][0] = i;

      cin >> a[i][1];
      dis.push_back(a[i][1]);
    }
    for (int i = 1; i <= n; i++) {
      cin >> a[i][2];
      dis.push_back(a[i][2]);
    }

    sort(dis.begin(), dis.end()); 
    dis.erase(unique(dis.begin(), dis.end()), dis.end());

    for (int i = 1; i <= n; i++) {
      a[i][1] = lower_bound(dis.begin(), dis.end(), a[i][1]) - dis.begin() + 1;
      a[i][2] = lower_bound(dis.begin(), dis.end(), a[i][2]) - dis.begin() + 1;
    }
    
    vector<int> dp(n + 1, 0);

    // 树状数组重复使用可以快不少。不重复使用也行。
    BIT T(dis.size());

    function<void(int, int)> CDQ = [&] (int l, int r) {
      if (l == r) {
        dp[l]++;
        return;
      }
      int mid = (l + r) >> 1;
      CDQ(l, mid);

      sort(a.begin() + l, a.begin() + mid + 1, [&](auto i, auto j) { return i[1] < j[1]; });
      sort(a.begin() + mid + 1, a.begin() + r + 1, [&](auto i, auto j) { return i[1] < j[1]; });

      int p = l;
      for (int i = mid + 1; i <= r; i++) {
        while (p <= mid && a[p][1] < a[i][1]) {

          // 注意下标！！！
          T.modify(a[p][2], dp[a[p][0]]);

          p++;
        }
        if (a[i][2] != 1) {
          dp[a[i][0]] = max(dp[a[i][0]], T.query(a[i][2] - 1));
        }
      }

      for (int i = l; i < p; i++) {
        T.init(a[i][2]);
      }

      sort(a.begin() + l, a.begin() + mid + 1);
      sort(a.begin() + mid + 1, a.begin() + r + 1);
      CDQ(mid + 1, r);
    };
    
    CDQ(1, n);

    int ans = 0;
    for (int i = 1; i <= n; i++) {
      ans = max(ans, dp[i]);
    }
    cout << ans << endl;
  }
  return 0;
}
