//
//  main.cpp
//  2022-05-21 16:43
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
#define LOCAL
using namespace std;

// 合并线段树
// 特性说明：
// 1. 在 vector<Tree> tree; 中初始有 n 个权值线段树(动态开点), 用于合并
// 2. 使用 merge_tree , 合并两个线段树(可以是子树), 需要保证传入的两个下标对应的 l, r 相同
// 3. merge_tree 后原本的 a, b 两个树可以被视作作废(实际不完全作废)。返回新树的下标。
// 4. 访问初始的 n 个root，一定！要用 root[i], 因为 root[i] 会因为 merge_tree 变动，类比 Treap
// 5. root[pos] = merge_tree(pos, v); 是 merge_tree 的正确用法，一定要让 merge_tree 的返回值被用到！
//
// 针对不同的题需要重写模板的一部分，修改顺序如下:
// 1. Info 结构体(注意初始值)
// 2. merge 函数
// 3. add 函数
// 4. merge_tree 函数
// 5. 检查 query, push_up 里的初始值是否赋值正确
// 6. 一般来说 modify 函数绝对不会被修改 ！

const long long INF = 0x3f3f3f3f3f3f3f3f;
struct Segment_Tree {

  struct Info {
    long long sum, maxn; 
    Info () { sum = 0, maxn = 0; } // 注意初始值
  };

  struct Tree {
    int ls, rs; // 左儿子，右儿子 编号
    int l, r; // 左边界，右边界
    Info dat;
    Tree(int a, int b, int c, int d) : ls(a), rs(b), l(c), r(d) {} 
  };

  vector<Tree> tree; 
  vector<int> root;

  // 初始化 n 棵线段树 1-indexed
  Segment_Tree (int n, int l, int r) {
    root.resize(n + 1);
    iota(root.begin(), root.end(), 0);
    tree.resize(n + 1, {-1, -1, l, r});  
  }

  int insert(int l, int r) {
    tree.push_back({-1, -1, l, r});
    return (int) tree.size() - 1;
  }
   
  int merge_tree(int a, int b) {
    if (a == -1) return b;       
    if (b == -1) return a;
    // merge_tree 只会在叶子结点合并, 注意这里的合并不能使用两个儿子的 merge 函数! 要单独写
    if (tree[a].l == tree[a].r) {
      // 需要重写!!!
      tree[a].dat.maxn += tree[b].dat.maxn; 
      tree[a].dat.sum = tree[a].l;
      return a;
    }
    tree[a].ls = merge_tree(tree[a].ls, tree[b].ls);
    tree[a].rs = merge_tree(tree[a].rs, tree[b].rs);
    push_up(a); 
    return a;
  }

  // 同一棵树里两个儿子合并, 注意不能用在不同树的相同结点合并。
  // merge 函数每一道题都需要重写!!!
  Info merge(Info a, Info b) {
    Info res;
    if (a.maxn < b.maxn) swap(a, b);
    if (a.maxn == b.maxn) {
      res.maxn = a.maxn; 
      res.sum = a.sum + b.sum;
    } else {
      res.maxn = a.maxn; 
      res.sum = a.sum;
    }
    return res;
  }

  void add(int pos, int val) {
    tree[pos].dat.sum = tree[pos].l;   // 需要重写
    tree[pos].dat.maxn += val;
  }

  void push_up(int pos) {
    tree[pos].dat.sum = 0;            // 可能需要修改的地方
    tree[pos].dat.maxn = -INF; 
    if (tree[pos].ls != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].ls].dat);
    if (tree[pos].rs != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].rs].dat);
  }

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

  Info query(int pos, int l, int r) {
    if (tree[pos].l >= l && tree[pos].r <= r) {
      return tree[pos].dat;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    Info res; res.maxn = -INF; //注意初始值 可能需要修改的地方
                               
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


signed main() {
//  file();
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n; cin >> n;
  vector<int> a(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  vector<vector<int>> G(n + 1);
  for (int i = 1; i <= n - 1; i++) {
    int u, v; cin >> u >> v;
    G[u].push_back(v); 
    G[v].push_back(u);
  }

  Segment_Tree T(n, -1e9, 1e9);

  for (int i = 1; i <= n; i++) {
    T.modify(T.root[i], a[i], 1);
  }

  vector<int> ans(n + 1, 0);
  function<void(int, int)> dfs = [&] (int pos, int fa) {
    for (auto &v : G[pos]) {
      if (v == fa) continue;
      dfs(v, pos);
      T.root[pos] = T.merge_tree(T.root[pos], v); 
    }
    ans[pos] = T.tree[T.root[pos]].dat.sum;
  };

  dfs(1, -1);
  for (int i = 1; i <= n; i++) {
    cout << ans[i] << " ";
  }
   
  return 0;
}
