//
//  main.cpp
//  2022-07-16 20:07
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

struct BIT2d{
  vector<vector<array<long long, 4>>> tree;
  int maxn, maxm;

  BIT2d(int n, int m) : tree(n + 1, vector<array<long long, 4>> (m + 1, {0ll, 0ll, 0ll, 0ll})), maxn(n), maxm(m){}

  inline int lowbit(int x) {
    return x & (-x);
  }

  void modify(int x, int y, long long val) {
    assert(x > 0 && y > 0);
    for (int k = x; k <= maxn; k += lowbit(k)) {
      for (int l = y; l <= maxm; l += lowbit(l)) {
        tree[k][l][0] += val;
        tree[k][l][1] += x * val;
        tree[k][l][2] += y * val;
        tree[k][l][3] += x * y * val;
      }
    }
  }

  long long query(int x, int y) {
    assert(x >= 0 && y >= 0);
    long long ans = 0ll;
    for (int i = x; i > 0; i -= lowbit(i)) {
      for (int j = y; j > 0; j -= lowbit(j)) {
        ans += 1ll * (x + 1) * (y + 1) * tree[i][j][0];
        ans -= 1ll * (y + 1) * tree[i][j][1];
        ans -= 1ll * (x + 1) * tree[i][j][2];
        ans += 1ll * tree[i][j][3];
      }
    }
    return ans;
  }
  // 左上 右下
  void add(int x1, int y1, int x2, int y2, long long val) {
    modify(x1, y1, val);  
    modify(x2 + 1, y1, -val);
    modify(x1, y2 + 1, -val);
    modify(x2 + 1, y2 + 1, val);
  }

  // 左上 右下
  long long get_sum(int x1, int y1, int x2, int y2) {
    return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

