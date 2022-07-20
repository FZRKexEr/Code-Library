//
//  main.cpp
//  2022-05-09 17:00
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;


// 普通线段树
// 1. 修改成区间覆盖直接把 += 换成 =
// 2. 支持区间和，区间最大值，区间最小值。
// 3. 提速方法：
//    a. 删掉不需要维护的东西。
//    b. 不要用 modify 初始化，用 build
//    c. 如果只用 sum 和 add, 请使用树状数组。
//    d. 关闭 define int long long
//
// 4. 使用步骤:
//    a. 初始化传入点数, 默认 1 - n 可维护
//    b. build(1, 1, n, arr); arr 是一个初始 vector, 注意空间开够。
//    c. query(1, l, r).maxi or sum or mini
//    d. modify(1, l, r, val); 区间加/覆盖

struct Segment_Tree {
  struct Info {
    long long sum, maxi, mini; 
  };

  struct Tree {
    int l, r; 
    Info val;
  };  
  vector<Tree> tree;
  vector<long long> lazy;

  Segment_Tree(int n) {
    tree.resize(n * 4 + 10);  
    lazy.resize(n * 4 + 10);
  }

  // 合并两个儿子
  Info merge(Info a, Info b) {
    Info res;
    res.sum = a.sum + b.sum;
    res.maxi = max(a.maxi, b.maxi);
    res.mini = min(a.mini, b.mini);
    return res;
  }

  void build(int pos, int l, int r, vector<int> &arr) {

    tree[pos].l = l; tree[pos].r = r;
    lazy[pos] = 0ll;

    int m = l + (r - l) / 2;
    if (l == r) {
      tree[pos].val.sum = arr[l]; 
      tree[pos].val.maxi = arr[l];
      tree[pos].val.mini = arr[l];
      return;
    } 
    build(pos << 1, l, m, arr);
    build(pos << 1 | 1, m + 1, r, arr);
    tree[pos].val = merge(tree[pos << 1].val, tree[pos << 1 | 1].val);
  }

  // 对树上一个位置进行修改
  void add(int pos, long long val) {
    lazy[pos] += val;
    tree[pos].val.sum += (tree[pos].r - tree[pos].l + 1) * val;
    tree[pos].val.maxi += val;
    tree[pos].val.mini += val;
  }

  // 传递懒人标记
  void push_down(int pos) {
    if (lazy[pos] == 0) return; 
    add(pos << 1, lazy[pos]); 
    add(pos << 1 | 1, lazy[pos]);
    lazy[pos] = 0;
  } 

  void modify(int pos, int l, int r, long long val) {
    assert(l <= r);
    if (tree[pos].l >= l && tree[pos].r <= r) {
      add(pos, val);
      return;
    }  
    push_down(pos);
    int m = tree[pos].l + (tree[pos].r - tree[pos].l) / 2;
    if (l <= m) modify(pos << 1, l, r, val);
    if (r > m) modify(pos << 1 | 1, l, r, val);
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

