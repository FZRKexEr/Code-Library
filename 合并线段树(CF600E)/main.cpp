//
//  cf600e-线段树合并.cpp
//  2022-03-30 15:41
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

#ifdef ONLINE_JUDGE 
#pragma GCC optimize("O3","unroll-loops") 
#pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt") 
#endif

void file() {
#ifndef ONLINE_JUDGE
  freopen("in.txt", "r", stdin);
  // freopen("out.txt", "w", stdout);
#endif
}
const long long INF = 0x3f3f3f3f3f3f3f3f;


vector<int> c, ans;
vector<vector<int>> G;
struct Merge_Segment_Tree {

  struct Info {
    int maxn, res;
    Info () { maxn = 0, res = 0; }
    Info operator + (Info &oth) {
      Info re; 
      if (maxn > oth.maxn) {
        re.maxn = maxn;
        re.res = res; 
      } 
      if (maxn == oth.maxn) {
        re.maxn = maxn;
        re.res += oth.res + res;
      }
      if (maxn < oth.maxn) {
        re.maxn = oth.maxn;
        re.res = oth.res;
      }
      return re;
    }
  };

  struct Tree {
    int ls, rs; // 左儿子，右儿子 编号
    int l, r; // 左边界，右边界
    Info dat;
    Tree(int a, int b, int c, int d) : ls(a), rs(b), l(c), r(d) {} 
  };

  vector<Tree> tree; 

  Merge_Segment_Tree(int n, int l, int r) {
    for (int i = 0; i < n; i++)
      tree.push_back({-1, -1, l, r});
  }

  int insert(int l, int r) {
    tree.push_back({-1, -1, l, r});
    return (int) tree.size() - 1;
  }
 
  void add(int pos, int val, int val2) {
    tree[pos].dat.maxn += val;
    tree[pos].dat.res = val2;
  }

  void push_up(int pos) {
    tree[pos].dat.maxn = -INF;
    tree[pos].dat.res = 0;
    if (tree[pos].ls != -1) tree[pos].dat = tree[pos].dat + tree[tree[pos].ls].dat;
    if (tree[pos].rs != -1) tree[pos].dat = tree[pos].dat + tree[tree[pos].rs].dat;
  }

  void modify(int pos, int goal, int val) {
    if (tree[pos].l == tree[pos].r) {
      add(pos, val, tree[pos].l);
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

  // 把 b merge 到 a
  int merge(int a, int b) { // n 1e5 值域 1e5 时，约等于 log(1e5)
    if (a == -1) return b; 
    if (b == -1) return a;
    if (tree[a].l == tree[a].r) {
      tree[a].dat.maxn += tree[b].dat.maxn;
      tree[a].dat.res = tree[a].l;
      return a;
    }
    tree[a].ls = merge(tree[a].ls, tree[b].ls);
    tree[a].rs = merge(tree[a].rs, tree[b].rs);
    push_up(a);
    return a;
  }
};

void dfs(Merge_Segment_Tree &T, int pos, int fa) {
  for (auto &v : G[pos]) {
    if (v == fa) continue;
    dfs(T, v, pos);
    T.merge(pos - 1, v - 1); // 把 v merge 到 pos
  }
  T.modify(pos - 1, c[pos], 1);
  ans[pos] = T.tree[pos - 1].dat.res; 
}

signed main() {
  file();
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n; cin >> n;  
  c.resize(n + 1);
  G.resize(n + 1);
  ans.resize(n + 1);

  for (int i = 1; i <= n; i++) {
    cin >> c[i]; 
  }

  for (int i = 1; i < n; i++) {
    int u, v; cin >> u >> v;
    G[u].push_back(v);
    G[v].push_back(u);
  }

  Merge_Segment_Tree T(n, 1, n);
  dfs(T, 1, -1);

  for (int i = 1; i <= n; i++)  {
    cout << ans[i] << " "; 
  }

  return 0;
}

