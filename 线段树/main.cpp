//
//  main.cpp
//  2022-05-09 17:00
//
//  Created by liznb
//  
#include <bits/stdc++.h>

using namespace std;

// 普通线段树
// 1. 同时支持区间和，区间最大值，区间最小值, 区间覆盖。
// 2. 提速方法：
//    a. 删掉不需要维护的东西。
//    b. 不要用 modify 初始化，用 build
//    c. 如果只用 sum 和 add, 请使用树状数组。
//    d. 关闭 define int long long
//    e. 使用区间覆盖来整体初始化
//
// 3. 使用步骤:
//    a. 初始化传入点数, 默认 1 - n 可维护
//    b. query(l, r).maxn or sum or minn
//    c. modify(l, r, val); 区间加
//    d. cover(l, r, val) 区间覆盖

struct Segment_Tree {
  struct Info {
    long long sum, maxn, minn; 
  };

  struct Tree {
    int l, r; 
    Info val;
  };  

  vector<Tree> tree;
  vector<long long> lazy;
  vector<pair<bool, long long>> tag;

  Segment_Tree(int n) {
    tree.resize(n * 4 + 10);
    lazy.resize(n * 4 + 10);
    tag.resize(n * 4 + 10);
    vector<int> arr(n + 1, 0);
    build(1, n, arr);
  }

  Segment_Tree(int n, vector<int> &arr) {
    tree.resize(n * 4 + 10);
    lazy.resize(n * 4 + 10);
    tag.resize(n * 4 + 10);
    build(1, n, arr);
  }

  Info merge(Info a, Info b) {
    Info res;
    res.sum = a.sum + b.sum;
    res.maxn = max(a.maxn, b.maxn);
    res.minn = min(a.minn, b.minn);
    return res;
  }

  void build(int pos, int l, int r, vector<int> &arr) {
    assert(l <= r);
    tree[pos].l = l; tree[pos].r = r;
    lazy[pos] = 0ll;
    tag[pos] = make_pair(false, 0);

    int m = l + (r - l) / 2;
    if (l == r) {
      tree[pos].val.sum = arr[l]; 
      tree[pos].val.maxn = arr[l];
      tree[pos].val.minn = arr[l];
      return;
    }
    build(pos << 1, l, m, arr);
    build(pos << 1 | 1, m + 1, r, arr);
    tree[pos].val = merge(tree[pos << 1].val, tree[pos << 1 | 1].val);
  }


  // 对树上一个位置进行修改, o = 1 表示普通修改，o = 0 表示覆盖 
  void add(int pos, long long val, int o) {
    if (o) {
      lazy[pos] += val;
      tree[pos].val.sum += (tree[pos].r - tree[pos].l + 1) * val;
      tree[pos].val.maxn += val;
      tree[pos].val.minn += val;
    } else {
      lazy[pos] = 0;
      tag[pos] = make_pair(true, val);
      tree[pos].val.sum = (tree[pos].r - tree[pos].l + 1) * val;
      tree[pos].val.maxn = val;
      tree[pos].val.minn = val;
    }
  }

  // 传递lazy 和 tag
  void push_down(int pos) {
    if (lazy[pos]) {
      if (tag[pos].first) {
        add(pos << 1, tag[pos].second, 0);
        add(pos << 1 | 1, tag[pos].second, 0);
        tag[pos].first = false;
      }
      add(pos << 1, lazy[pos], 1); 
      add(pos << 1 | 1, lazy[pos], 1);
      lazy[pos] = 0;
    }
    if (tag[pos].first) {
      add(pos << 1, tag[pos].second, 0);
      add(pos << 1 | 1, tag[pos].second, 0);
      tag[pos].first = false;
    }
  } 

  void modify(int pos, int l, int r, long long val) {
    assert(l <= r);
    if (tree[pos].l >= l && tree[pos].r <= r) {
      add(pos, val, 1);
      return;
    }  
    push_down(pos);
    int m = tree[pos].l + (tree[pos].r - tree[pos].l) / 2;
    if (l <= m) modify(pos << 1, l, r, val);
    if (r > m) modify(pos << 1 | 1, l, r, val);
    tree[pos].val = merge(tree[pos << 1].val, tree[pos << 1 | 1].val);
  } 

  void cover(int pos, int l, int r, long long val) {
    assert(l <= r);
    if (tree[pos].l >= l && tree[pos].r <= r) {
      add(pos, val, 0);
      return;
    }  
    push_down(pos);
    int m = tree[pos].l + (tree[pos].r - tree[pos].l) / 2;
    if (l <= m) cover(pos << 1, l, r, val);
    if (r > m) cover(pos << 1 | 1, l, r, val);
    tree[pos].val = merge(tree[pos << 1].val, tree[pos << 1 | 1].val);
  } 
  
  Info query(int pos, int l, int r) {
    assert(l <= r);
    if (tree[pos].l >= l && tree[pos].r <= r) {
      return tree[pos].val;
    }   
    push_down(pos);
    int m = tree[pos].l + (tree[pos].r - tree[pos].l) / 2;
    Info res;

    if (l <= m && r > m) {
      res = merge(query(pos << 1, l, r), query(pos << 1 | 1, l , r));
    } else if (l > m) {
      res = query(pos << 1 | 1, l, r);
    } else if (r <= m) {
      res = query(pos << 1, l, r);
    }

    tree[pos].val = merge(tree[pos << 1].val, tree[pos << 1 | 1].val);
    return res;
  }

  void build(int l, int r, vector<int> &arr) { build(1, l, r, arr); }
  void modify(int l, int r, long long val) { modify(1, l, r, val); }
  void cover(int l, int r, long long val) { cover(1, l, r, val); }
  Info query(int l, int r) { return query(1, l, r); };
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
   
  int n, q; cin >> n >> q;   

  vector<int> arr(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> arr[i];
  }

  Segment_Tree T(n, arr);

  for (int i = 1; i <= q; i++) {
    int o; cin >> o;
    if (o == 1) {
      int l, r, x; cin >> l >> r >> x;
      T.modify(l, r, x);
    } else {
      int l, r; cin >> l >> r;
      cout << T.query(l, r).sum << '\n';
    }
  }

  return 0;
}
