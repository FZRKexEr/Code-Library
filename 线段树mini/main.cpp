//
//  main.cpp
//  2022-11-08 20:41
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define endl '\n'


// 线段树mini
// 各种功能的线段树的简短实现
// 方便现场赛手敲


// 线段树mini 求和

const long long INF = 1e18;
struct Sgt_Sum {
  using Node = long long;
  vector<Node> tree, lazy;
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

// 线段树mini 求最大值/最小值 + 线段树二分
struct Sgt_MinMax {
  struct Node {
    long long maxn, minn;
  };
  vector<Node> tree;
  vector<long long> lazy;
  Sgt_MinMax(int n) {
    tree.resize(4 * (n + 1), {0, 0});
    lazy.resize(4 * (n + 1), 0);
  }
  void push_down(int pos) {
    if (lazy[pos]) {
      lazy[pos << 1] += lazy[pos];
      lazy[pos << 1 | 1] += lazy[pos];
      tree[pos << 1].maxn += lazy[pos];
      tree[pos << 1 | 1].maxn += lazy[pos];
      tree[pos << 1].minn += lazy[pos];
      tree[pos << 1 | 1].minn += lazy[pos];
      lazy[pos] = 0;
    }
  }
  void build(int pos, int tl, int tr, vector<long long> &arr) {
    if (tl == tr) {
      tree[pos] = {arr[tl], arr[tl]};
      return;
    }
    int m = (tl + tr) / 2;
    build(pos << 1, tl, m, arr);
    build(pos << 1 | 1, m + 1, tr, arr);
    tree[pos].minn = min(tree[pos << 1].minn, tree[pos << 1 | 1].minn);
    tree[pos].maxn = max(tree[pos << 1].maxn, tree[pos << 1 | 1].maxn);
  }
  void modify(int l, int r, long long val, int pos, int tl, int tr) {
    if (tl >= l && tr <= r) {
      lazy[pos] += val;
      tree[pos].maxn += val;
      tree[pos].minn += val;
      return;
    }
    push_down(pos);
    int m = (tl + tr) / 2;
    if (l <= m) modify(l, r, val, pos << 1, tl, m);
    if (r > m) modify(l, r, val, pos << 1 | 1, m + 1, tr);
    tree[pos].minn = min(tree[pos << 1].minn, tree[pos << 1 | 1].minn);
    tree[pos].maxn = max(tree[pos << 1].maxn, tree[pos << 1 | 1].maxn);
  }
  Node query(int l, int r, int pos, int tl, int tr) {
    if (tl >= l && tr <= r) return tree[pos];
    push_down(pos);
    int m = (tl + tr) / 2;
    long long res_min = INF, res_max = -INF;
    if (l <= m) {
      Node res = query(l, r, pos << 1, tl, m);
      res_max = max(res_max, res.maxn);
      res_min = min(res_min, res.minn);
    }
    if (r > m) {
      Node res = query(l, r, pos << 1 | 1, m + 1, tr);
      res_max = max(res_max, res.maxn);
      res_min = min(res_min, res.minn);
    }
    return {res_max, res_min};
  }
  // 线段树二分
  int find_first_knowingly(int pos, int tl, int tr, const function<bool(const Node&)> &f) {
    if (tl == tr) return tl;
    push_down(pos);
    int m = (tl + tr) / 2;
    if (f(tree[pos << 1])) return find_first_knowingly(pos << 1, tl, m, f);
    else return find_first_knowingly(pos << 1 | 1, m + 1, tr, f);
  }
  int find_first(int l, int r, int pos, int tl, int tr, const function<bool(const Node&)> &f) {
    if (tl >= l && tr <= r) {
      if (!f(tree[pos])) return -1;
      return find_first_knowingly(pos, tl, tr, f);
    }
    push_down(pos);
    int m = (tl + tr) / 2;
    int res = -1;

    if (l <= m) res = find_first(l, r, pos << 1, tl, m, f);
    if (r > m && res == -1) res = find_first(l, r, pos << 1 | 1, m + 1, tr, f);
    return res;
  }
  int find_last_knowingly(int pos, int tl, int tr, const function<bool(const Node&)> &f) {
    if (tl == tr) return tl;
    push_down(pos);
    int m = (tl + tr) / 2;
    if (f(tree[pos << 1 | 1])) return find_last_knowingly(pos << 1 | 1, m + 1, tr, f);
    else return find_last_knowingly(pos << 1, tl, m, f);
  }
  int find_last(int l, int r, int pos, int tl, int tr, const function<bool(const Node&)> &f) {
    if (tl >= l && tr <= r) {
      if (!f(tree[pos])) return -1;
      return find_last_knowingly(pos, tl, tr, f);
    }
    push_down(pos);
    int m = (tl + tr) / 2;
    int res = -1;

    if (r > m) res = find_last(l, r, pos << 1 | 1, m + 1, tr, f);
    if (l <= m && res == -1) res = find_last(l, r, pos << 1, tl, m, f);
    return res;
  }
};

// 线段树mini 单点修改，区间查询最大值
struct Sgt {
  using Node = long long;
  vector<Node> tree;
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
  int find_first_knowingly(int pos, int tl, int tr, const function<bool(const Node&)> &f) {
    if (tl == tr) return tl;
    int m = (tl + tr) / 2;
    if (f(tree[pos << 1])) return find_first_knowingly(pos << 1, tl, m, f);
    else return find_first_knowingly(pos << 1 | 1, m + 1, tr, f);
  }
  int find_first(int l, int r, int pos, int tl, int tr, const function<bool(const Node&)> &f) {
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
  int find_last_knowingly(int pos, int tl, int tr, const function<bool(const Node&)> &f) {
    if (tl == tr) return tl;
    int m = (tl + tr) / 2;
    if (f(tree[pos << 1 | 1])) return find_last_knowingly(pos << 1 | 1, m + 1, tr, f);
    else return find_last_knowingly(pos << 1, tl, m, f);
  }
  int find_last(int l, int r, int pos, int tl, int tr, const function<bool(const Node&)> &f) {
    if (tl >= l && tr <= r) {
      if (!f(tree[pos])) return -1;
      return find_last_knowingly(pos, tl, tr, f);
    }
    int m = (tl + tr) / 2;
    int res = -1;

    if (r > m) res = find_last(l, r, pos << 1 | 1, m + 1, tr, f);
    if (l <= m && res == -1) res = find_last(l, r, pos << 1, tl, m, f);
    return res;
  }
};


// 线段树mini 区间修改/查询 gcd
// 利用性质 gcd(a, b, c) = gcd(a, b - a, c - b), 维护 差分gcd 和区间最左和最右值
// 常数很大，main() 里面尽量减少 modify/query 次数
// 复杂度 O(nlog^2n)
//
struct Sgt_gcd {
  using Node = array<long long, 3>;
  vector<Node> tree;
  vector<long long> lazy;

  Sgt_gcd(int n) {
    tree.resize(4 * (n + 1), {0, 0, 0});
    lazy.resize(4 * (n + 1), 0);
  }
  void push_down(int pos, int len) {
    if (lazy[pos]) {
      lazy[pos << 1] += lazy[pos];
      lazy[pos << 1 | 1] += lazy[pos];

      tree[pos << 1][0] += lazy[pos];
      tree[pos << 1 | 1][0] += lazy[pos];
      tree[pos << 1][2] += lazy[pos];
      tree[pos << 1 | 1][2] += lazy[pos];

      lazy[pos] = 0;
    }
  }
  void build(int pos, int tl, int tr, vector<long long> &arr) {
    if (tl == tr) {
      tree[pos][0] = tree[pos][2] = arr[tl];
      return;
    }
    int m = (tl + tr) / 2;
    build(pos << 1, tl, m, arr);
    build(pos << 1 | 1, m + 1, tr, arr);

    tree[pos][0] = tree[pos << 1][0];
    tree[pos][2] = tree[pos << 1 | 1][2];
    tree[pos][1] = gcd(gcd(tree[pos << 1][1], tree[pos << 1 | 1][0] - tree[pos << 1][2]), tree[pos << 1 | 1][1]);
  }
  void modify(int l, int r, long long val, int pos, int tl, int tr) {
    assert(l <= r);
    if (tl >= l && tr <= r) {
      lazy[pos] += val;

      tree[pos][0] += val;
      tree[pos][2] += val;

      return;
    }
    push_down(pos, tr - tl + 1);
    int m = (tl + tr) / 2;
    if (l <= m) modify(l, r, val, pos << 1, tl, m);
    if (r > m) modify(l, r, val, pos << 1 | 1, m + 1, tr);

    tree[pos][0] = tree[pos << 1][0];
    tree[pos][2] = tree[pos << 1 | 1][2];
    tree[pos][1] = gcd(gcd(tree[pos << 1][1], tree[pos << 1 | 1][0] - tree[pos << 1][2]), tree[pos << 1 | 1][1]);
  }

  long long query(int l, int r, int pos, int tl, int tr) {
    if (tl >= l && tr <= r) {
      return gcd(tree[pos][0], tree[pos][1]);
    }

    push_down(pos, tr - tl + 1);

    int m = (tl + tr) / 2;
    long long res = 0;
    if (l <= m) res = gcd(res, query(l, r, pos << 1, tl, m));
    if (r > m) res = gcd(res, query(l, r, pos << 1 | 1, m + 1, tr));
    return res;
  }
};


signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int l = 2, r = 4, n = 5;

  Sgt_MinMax T(n);
  vector<long long> a = {0, 1, 2, 3, 4, 5};
  T.build(1, 1, n, a);

  // 找到 l, r 区间内第一个大于 2 的数
  // f 的意义是判断左/右区间能否进入
  int p = T.find_first(l, r, 1, 1, n, [&](const Sgt_MinMax::Node& it) {
    if (it.maxn > 2) return true;
    return false;
  });

  cout << p << endl;

  return 0;
}
