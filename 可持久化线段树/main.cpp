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
// 
// 前缀和性质:
// 不同的版本按照时间顺序具有前缀和性质，可以相减

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
  vector<int> a(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];      
  }
  vector<int> b = a;
  sort(b.begin() + 1, b.end());
  b.erase(unique(b.begin() + 1, b.end()), b.end());
  vector<int> ord(n + 1); 
  for (int i = 1; i <= n; i++) {
    ord[i] = lower_bound(b.begin() + 1, b.end(), a[i]) - b.begin();
  }
  Segment_Tree T(1, (int) b.size() - 1);   

  for (int i = 1; i <= n; i++) {
    T.add_ver(1, (int) b.size() - 1);
    T.pmodify(T.ver[i - 1], T.ver[i], ord[i], 1);
  }

  for (int i = 1; i <= m; i++) {
    int l, r, k; cin >> l >> r >> k;
    // 区间第 k 小
    function<int(int, int, int)> kth = [&] (int posl, int posr, int k) {
      int cntl = 0; 
      assert(posr != -1);
      if (T.tree[posr].l == T.tree[posr].r) return T.tree[posr].l;
      if (T.tree[posr].ls != -1) cntl += T.tree[T.tree[posr].ls].dat.sum;
      if (posl != -1 && T.tree[posl].ls != -1) cntl -= T.tree[T.tree[posl].ls].dat.sum;
      if (cntl >= k) {
        if (posl == -1) return kth(-1, T.tree[posr].ls, k);
        else return kth(T.tree[posl].ls, T.tree[posr].ls, k);
      } else {
        if (posl == -1) return kth(-1, T.tree[posr].rs, k - cntl);
        else return kth(T.tree[posl].rs, T.tree[posr].rs, k - cntl);
      }
    };
    cout << b[kth(T.ver[l - 1], T.ver[r], k)] << endl; 
  }
   
  return 0;
}
