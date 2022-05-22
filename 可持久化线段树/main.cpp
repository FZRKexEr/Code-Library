//
//  main.cpp
//  2022-05-22 15:38
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 可持久化线段树
// 1. vector<int> ver 保存版本, add_ver 会添加一个版本.
// 2. 注意：访问版本一定用 ver[i], 访问版本一定用 ver[i], 访问版本一定用 ver[i]
// 3. 如果操作范围是连续的 1-n, 且 n 较小，可以直接 O(n) build.
// 4. modify 不增加版本修改(主要用于不使用 build 的初始化), pmodify 是可持久化修改
//
// 优化空间的方法:
// 1. 关闭 long long
// 2. Info 里面把无关的变量删了
// 3. T.tree.reserve() , 一般大小是 log2(n) * m , 可以减少很多空间

struct Segment_Tree {

  struct Info {
    int sum; 
    Info () { sum = 0; }
  };

  struct Tree {
    int ls, rs; // 左儿子，右儿子 编号
    int l, r; // 左边界，右边界
    Info dat;
    Tree(int a, int b, int c, int d) : ls(a), rs(b), l(c), r(d) {} 
  };

  vector<Tree> tree; 
  vector<int> ver;

  Segment_Tree(int l, int r) {
    tree.push_back({-1, -1, l, r});
    ver.push_back((int) tree.size() - 1);
  }
 
  int add_ver(int l, int r) {
    tree.push_back({-1, -1, l, r}); 
    ver.push_back((int) tree.size() - 1);
    return (int) tree.size() - 1;
  }

  int insert(int l, int r) {
    tree.push_back({-1, -1, l, r});
    return (int) tree.size() - 1;
  }
 
  Info merge(const Info &a, const Info &b) {
    Info res;
    res.sum = a.sum + b.sum;
    return res;
  }

  void add(int pos, int val) {
    tree[pos].dat.sum += val;
  }

  void push_up(int pos) {
    tree[pos].dat.sum = 0; 
    if (tree[pos].ls != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].ls].dat);
    if (tree[pos].rs != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].rs].dat);
  }

  void build(int pos, vector<int> &arr) {
    if (tree[pos].l == tree[pos].r) {
      tree[pos].dat.sum = arr[tree[pos].l];
      return;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    if (tree[pos].ls == -1) tree[pos].ls = insert(tree[pos].l, mid);
    if (tree[pos].rs == -1) tree[pos].rs = insert(mid + 1, tree[pos].r);
    build(tree[pos].ls, arr); 
    build(tree[pos].rs, arr);
    push_up(pos);
  }

  // 普通修改，不增加版本  
  void modify(int pos, int goal, int val) {
    if (tree[pos].l == tree[pos].r) {
      add(pos, val);
      return;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    if (goal <= mid) {
      if (tree[pos].ls == -1) tree[pos].ls = insert(tree[pos].l, mid);
      modify(tree[pos].ls, goal, val);
    } else {
      if (tree[pos].rs == -1) tree[pos].rs = insert(mid + 1, tree[pos].r);
      modify(tree[pos].rs, goal, val);
    }
    push_up(pos);
  }
  
  void pmodify(int base, int pos, int goal, int val) {
    if (tree[pos].l == tree[pos].r) {
      tree[pos].dat = tree[base].dat;  
      add(pos, val);
      return;        
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    if (goal <= mid) {
      tree[pos].rs = tree[base].rs;
      tree[pos].ls = insert(tree[pos].l, mid);
      if (tree[base].ls == -1) tree[base].ls = insert(tree[base].l, mid);
      pmodify(tree[base].ls, tree[pos].ls, goal, val);
    } else {
      tree[pos].ls = tree[base].ls;
      tree[pos].rs = insert(mid + 1, tree[pos].r);
      if (tree[base].rs == -1) tree[base].rs = insert(mid + 1, tree[base].r);
      pmodify(tree[base].rs, tree[pos].rs, goal, val);
    }
    push_up(pos);
  }

  Info query(int pos, int l, int r) {
    if (tree[pos].l >= l && tree[pos].r <= r) {
      return tree[pos].dat;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    Info res;  //注意初始值

    if (l <= mid && tree[pos].ls != -1) {
      res = merge(res, query(tree[pos].ls, l, r));
    }
    if (r >= mid + 1 && tree[pos].rs != -1) {
      res = merge(res, query(tree[pos].rs, l, r));
    }
    push_up(pos);
    return res;
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n, m; cin >> n >> m; 
  Segment_Tree T(1, n);

  T.tree.reserve(24 * 1e6); 
  
  vector<int> a(n + 1, 0);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }

  T.build(T.ver[0], a);
  
  for (int i = 1; i <= m; i++) {
    int v, o; cin >> v >> o;
    if (o == 1) {
      int goal, val;
      cin >> goal >> val;
      T.add_ver(1, n);
      val -= T.query(T.ver[v], goal, goal).sum; 
      T.pmodify(T.ver[v], T.ver[i], goal, val); 
    } else {
      int goal;
      cin >> goal;
      T.add_ver(1, n);
      T.pmodify(T.ver[v], T.ver[i], goal, 0);
      cout << T.query(T.ver[i], goal, goal).sum << endl; 
    }
  }

  return 0;
}
