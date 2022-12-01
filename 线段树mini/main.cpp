//
//  main.cpp
//  2022-11-08 20:41
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define endl '\n'

// 线段树mini 求和

const long long INF = 1e18;
struct Sgt_Sum {
  vector<long long> tree, lazy;
  Sgt_Sum(int n) {
    tree.resize(4 * (n + 1), 0);
    lazy.resize(4 * (n + 1), 0);
  }
  void push_down(int pos, int len) {
    if (lazy[pos]) {
      lazy[pos << 1] += lazy[pos];
      lazy[pos << 1 | 1] += lazy[pos];
      tree[pos << 1] += lazy[pos] * ((len + 1) / 2);
      tree[pos << 1 | 1] += lazy[pos] * (len / 2);
      lazy[pos] = 0;
    }
  }
  void build(int pos, int tl, int tr) {
    if (tl == tr) {
      cin >> tree[pos];
      return;
    }
    int m = (tl + tr) / 2;
    build(pos << 1, tl, m);
    build(pos << 1 | 1, m + 1, tr);
    tree[pos] = tree[pos << 1] + tree[pos << 1 | 1];
  }
  void modify(int l, int r, long long val, int pos, int tl, int tr) {
    if (tl >= l && tr <= r) {
      lazy[pos] += val;
      tree[pos] += val * (tr - tl + 1);
      return;
    }
    push_down(pos, tr - tl + 1);
    int m = (tl + tr) / 2;
    if (l <= m) modify(l, r, val, pos << 1, tl, m);
    if (r > m) modify(l, r, val, pos << 1 | 1, m + 1, tr);
    tree[pos] = tree[pos << 1] + tree[pos << 1 | 1];
  }
  long long query(int l, int r, int pos, int tl, int tr) {
    if (tl >= l && tr <= r) return tree[pos];
    push_down(pos, tr - tl + 1);
    int m = (tl + tr) / 2;
    long long res = 0;
    if (l <= m) res += query(l, r, pos << 1, tl, m);
    if (r > m) res += query(l, r, pos << 1 | 1, m + 1, tr);
    tree[pos] = tree[pos << 1] + tree[pos << 1 | 1];
    return res;
  }
};

// 线段树mini 求最大值
struct Sgt_Max {
  vector<long long> tree, lazy;
  Sgt_Max(int n) {
    tree.resize(4 * (n + 1), 0);
    lazy.resize(4 * (n + 1), 0);
  }
  void push_down(int pos) {
    if (lazy[pos]) {
      lazy[pos << 1] += lazy[pos];
      lazy[pos << 1 | 1] += lazy[pos];
      tree[pos << 1] += lazy[pos];
      tree[pos << 1 | 1] += lazy[pos];
      lazy[pos] = 0;
    }
  }
  void build(int pos, int tl, int tr) {
    if (tl == tr) {
      cin >> tree[pos];
      return;
    }
    int m = (tl + tr) / 2;
    build(pos << 1, tl, m);
    build(pos << 1 | 1, m + 1, tr);
    tree[pos] = max(tree[pos << 1], tree[pos << 1 | 1]);
  }
  void modify(int l, int r, long long val, int pos, int tl, int tr) {
    if (tl >= l && tr <= r) {
      lazy[pos] += val;
      tree[pos] += val;
      return;
    }
    push_down(pos);
    int m = (tl + tr) / 2;
    if (l <= m) modify(l, r, val, pos << 1, tl, m);
    if (r > m) modify(l, r, val, pos << 1 | 1, m + 1, tr);
    tree[pos] = max(tree[pos << 1], tree[pos << 1 | 1]);
  }
  long long query(int l, int r, int pos, int tl, int tr) {
    if (tl >= l && tr <= r) return tree[pos];
    push_down(pos);
    int m = (tl + tr) / 2;
    long long res = -INF;
    if (l <= m) res = max(res, query(l, r, pos << 1, tl, m));
    if (r > m) res = max(res, query(l, r, pos << 1 | 1, m + 1, tr));
    tree[pos] = max(tree[pos << 1], tree[pos << 1 | 1]);
    return res;
  }
};

// 线段树mini 单点修改，区间查询
struct Sgt {
  using node = long long;
  vector<node> tree;
  Sgt(int n) {
    tree.resize(4 * (n + 1), 0);
  }
  void build(int pos, int tl, int tr) {
    if (tl == tr) {
      cin >> tree[pos];
      return;
    }
    int m = (tl + tr) / 2;
    build(pos << 1, tl, m);
    build(pos << 1 | 1, m + 1, tr);
    tree[pos] = max(tree[pos << 1], tree[pos << 1 | 1]);
  }
  void modify(int p, long long val, int pos, int tl, int tr) {
    if (tl == tr) {
      tree[pos] += val;
      return;
    }
    int m = (tl + tr) / 2;
    if (p <= m) modify(p, val, pos << 1, tl, m);
    if (p > m) modify(p, val, pos << 1 | 1, m + 1, tr);
    tree[pos] = max(tree[pos << 1], tree[pos << 1 | 1]);
  }
  long long query(int l, int r, int pos, int tl, int tr) {
    if (tl >= l && tr <= r) return tree[pos];
    int m = (tl + tr) / 2;
    long long res = -INF;
    if (l <= m) res = max(res, query(l, r, pos << 1, tl, m));
    if (r > m) res = max(res, query(l, r, pos << 1 | 1, m + 1, tr));
    return res;
  }

  // 线段树二分，注意单点修改没有 push_down, 区间修改需要加上
  int find_first_knowingly(int pos, int tl, int tr, const function<bool(const node&)> &f) {
    if (tl == tr) return tl;
    int m = (tl + tr) / 2;
    if (f(tree[pos << 1])) return find_first_knowingly(pos << 1, tl, m, f);
    else return find_first_knowingly(pos << 1 | 1, m + 1, tr, f);
  }
  int find_first(int l, int r, int pos, int tl, int tr, const function<bool(const node&)> &f) {
    if (tl >= l && tr <= r) {
      if (!f(tree[pos])) return -1;
      return find_first_knowingly(pos, tl, tr, f);
    }
    int m = (tl + tr) / 2;
    int res = -1;

    if (l <= m) res = find_first(l, r, pos << 1, tl, m, f);
    if (r > m && res == -1) res = find_first(l, r, pos << 1 | 1, m + 1, tr, f);
    return res;
  }
};

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m; cin >> n >> m;

  Sgt_Max T(n);
  T.build(1, 1, n);
  mt19937 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());

  for (int i = 1; i <= m; i++) {
    int l, r; cin >> l >> r;
    cout << T.query(l, r, 1, 1, n) << endl;
  }
  return 0;
}
