//
//  权值线段树(动态开点).cpp
//  2022-03-29 09:16
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

/*
 * 关于权值线段树:
 * 1. 经典问题：求数列第K大（区间第K大用可持久化线段树）
 *
 * 关于动态开点：
 * 1. 能不用就不用，能离散化就离散化
 * 2. 已知用处：可持久化数据结构，线段树合并，强制在线不能离散化的题。
 *
 * 模板特性：
 * 1. 支持区间修改，区间查询。
 * 2. 支持负数权值
 *
 * 优化：
 * 1. 模板内置了 sum，maxn，minn。可以根据题目要求删除一些，减少空间消耗。
 * 2. 还可以进一步优化空间：把 l 和 r 也删掉，直接在函数里传参
 * 3. 参考：逆序对问题：n = 5e5, 数值范围 1e9, 做了1，2优化后消耗 75mb 空间。
 *
 * 注意:
 * 1. !!!!! pos 是 0-indexed !!!!
 * 2. 如果问题能用普通线段树解决，就不要用这个。空间复杂度 Knlogn（K是大常数）
 * 3. 先删除不用的变量，否则很容易 MLE
 * 4. lazy 用 pair 是为了保证区间覆盖成 0 也没问题
 */

struct Segment_Tree {

  struct Info {
    int sum;
    Info () { sum = 0; }
  };

  struct Tree {
    int ls, rs; // 左儿子，右儿子 编号
    Info dat;
    Tree(int a, int b) : ls(a), rs(b) {}
  };

  vector<Tree> tree;
  vector<pair<int, bool>> lazy;

  int il, ir;

  Segment_Tree(int l, int r) : il(l), ir(r) {
    tree.emplace_back(-1, -1);
    lazy.emplace_back(0, 0);
  }

  int insert() {
    tree.emplace_back(-1, -1);
    lazy.emplace_back(0, 0);
    return (int) tree.size() - 1;
  }

  Info merge(Info a, Info b) {
    Info res;
    res.sum = a.sum + b.sum;
    return res;
  }

  void add(int pos, int tl, int tr, long long val) {
    tree[pos].dat.sum = val * (tr - tl + 1);
    lazy[pos] = {val, 1};
  }

  void push_up(int pos) {
    tree[pos].dat.sum = 0;
    if (tree[pos].ls != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].ls].dat);
    if (tree[pos].rs != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].rs].dat);
  }

  void push_down(int pos, int tl, int tr) {
    if (lazy[pos].second) {
      int mid = (tl + tr) >> 1;
      if (tree[pos].ls == -1) tree[pos].ls = insert();
      if (tree[pos].rs == -1) tree[pos].rs = insert();
      add(tree[pos].ls, tl, mid, lazy[pos].first);
      add(tree[pos].rs, mid + 1, tr, lazy[pos].first);
      lazy[pos] = {0, 0};
    }
  }

  void modify(int pos, int tl, int tr, int l, int r, long long val) {
    if (tl >= l && tr <= r) {
      add(pos, tl, tr, val);
      return;
    }
    push_down(pos, tl, tr);
    int mid = (tl + tr) >> 1;
    if (l<= mid) {
      if (tree[pos].ls == -1) tree[pos].ls = insert();
      modify(tree[pos].ls, tl, mid, l, r, val);
    }
    if (r > mid) {
      if (tree[pos].rs == -1) tree[pos].rs = insert();
      modify(tree[pos].rs, mid + 1, tr, l, r, val);
    }
    push_up(pos);
  }

  Info query(int pos, int tl, int tr, int l, int r) {
    if (tl >= l && tr <= r) {
      return tree[pos].dat;
    }

    push_down(pos, tl, tr);
    int mid = (tl + tr) >> 1;
    Info res;

    if (l <= mid && tree[pos].ls != -1) {
      res = merge(res, query(tree[pos].ls, tl, mid, l, r));
    }
    if (r >= mid + 1 && tree[pos].rs != -1) {
      res = merge(res, query(tree[pos].rs, mid + 1, tr, l, r));
    }
    push_up(pos);
    return res;
  }

  void modify(int l, int r, long long val) { modify(0, il, ir, l, r, val); }
  Info query(int l, int r) { return query(0, il, ir, l, r); }
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  Segment_Tree T(1, 1e9); 
  
  return 0;
}
