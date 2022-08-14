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
//    b. build(1, 1, n, arr); arr 是一个初始 vector, 注意空间开够。
//    c. query(1, l, r).maxn or sum or minn
//    d. modify(1, l, r, val); 区间加
//    e. cover(1, l, r, val) 区间覆盖

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
};

signed main() {
  int n, m; cin >> n >> m;  

  Segment_Tree T(n);

  vector<int> a(n + 1, 0);
  T.build(1, 1, n, a);

  for (int i = 1; i <= m; i++) {
    int k, l, r, val;
    cin >> k >> l >> r;
    if (k == 1) {
      cin >> val;
      T.modify(1, l, r, val); 
    } else if (k == 2) {
      cin >> val;
      T.cover(1, l, r, val); 
    } else {
      cout << T.query(1, l, r).sum << endl;  
    }
  }

  return 0;
}
