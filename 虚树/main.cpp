//
//  main.cpp
//  2022-05-27 14:21
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;

// 虚树
// 原理说明:
// 1. 把一棵树里的关键点构造成一个虚树。
// 2. 虚树里只有关键点和关键点的 lca
// 3. 找到所有关键点的 lca 的方法是对 dfs 序相邻的两个点找 lca，然后去重。
//
// 细节:
// 1. 虚树保存在 tree 里，注意，只有从 root (s) 能访问到的点才是虚树内的点。
// 2. 传入build 的数组需要 0 indexed
// 2. 做虚树题的时候记得及时清空数组，且不要 memset(模版内已经及时清空了)。
// 3. 这个模板只会建树，不会在建树的时候做额外操作。
//
// 使用流程:
// 1. Virtual_Tree T(n, root);
// 2. T.add(u, v); 自动双向
// 3. T.init();
// 4. T.build(key_point);

struct Virtual_Tree {
  vector<vector<int>> G, tree, p;
  vector<int> d, ord;
  int n, s, tot; 

  Virtual_Tree(int _n, int _s) : G(_n + 1), tree(_n + 1), d(_n + 1, 1), ord(_n + 1), n(_n), s(_s) {
    p.resize(_n + 1);
    int level = 0;
    while ((1 << level) <= _n) level++;
    for (int i = 0; i <= _n; i++) {
      p[i].resize(level + 1);
    }
  }

  int flag = false;

  void add(int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }
  
  void dfs(int pos, int fa) {
    ord[pos] = ++tot;
    p[pos][0] = fa;
    for (auto &v : G[pos]) {
      if (v == fa) continue;
      d[v] = d[pos] + 1;
      dfs(v, pos);
    }
  }

  void init() {
    flag = true; 
    tot = 0;
    dfs(s, s); 
    for (int level = 1; (1 << level) <= n; level++) {
      for (int i = 1; i <= n; i++) {
        p[i][level] = p[p[i][level - 1]][level - 1];
      }
    }
  }

  int lca(int a, int b) {
    assert(flag);
    if (d[a] < d[b]) swap(a, b); 
    int i, j;
    for (i = 0; (1 << i) <= d[a]; i++);  
    i--; 
    for (j = i; j >= 0; j--) {
      if (d[a] - (1 << j) >= d[b]) {
        a = p[a][j];
      }
    }
    if (a == b) return b; 
    for (j = i; j >= 0; j--) {
      if (p[a][j] != p[b][j]) {
        a = p[a][j];
        b = p[b][j];
      }
    }
    return p[a][0];
  }
  
  // key 0-indexed
  void build(vector<int> key) {
    sort(key.begin(), key.end(), [&](auto a, auto b) { return ord[a] < ord[b]; });
    vector<int> que;
    for (int i = 0; i < (int) key.size() - 1; i++) {
      que.push_back(lca(key[i], key[i + 1]));
    }
    que.push_back(s);
    key.insert(key.end(), que.begin(), que.end());
    sort(key.begin(), key.end()); 
    key.erase(unique(key.begin(), key.end()), key.end());
    sort(key.begin(), key.end(), [&](auto a, auto b) { return ord[a] < ord[b]; });
    assert(key[0] == s);

    for (auto &it : key) {
      tree[it].clear();
    }

    vector<int> stk; stk.push_back(s);
    for (int i = 1; i < (int) key.size(); i++) {
      while (lca(stk.back(), key[i]) != stk.back()) stk.pop_back();
      tree[stk.back()].push_back(key[i]);
      tree[key[i]].push_back(stk.back());
      stk.push_back(key[i]);
    }
  }
};

signed main() {
  // https://www.luogu.com.cn/problem/P2495
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
  int n; cin >> n; 
  Virtual_Tree T(n, 1);
  vector<vector<array<int, 2>>> G(n + 1);
  for (int i = 1; i <= n - 1; i++) {
    int u, v, w; cin >> u >> v >> w;
    G[u].push_back({w, v});
    G[v].push_back({w, u});
    T.add(u, v);
  }

  T.init();

  vector<int> dp(n + 1, 0), is(n + 1, 0), w(n + 1);

  function<void(int, int, int)> init = [&] (int pos, int fa, int minn) {
    w[pos] = minn;
    for (auto &[w, v] : G[pos]) {
      if (v == fa) continue;
      init(v, pos, min(minn, w)); 
    }
  };

  init(1, -1, 0x3f3f3f3f3f3f3f3f);

  int m; cin >> m;
  for (int i = 1; i <= m; i++) {
    int k; cin >> k; 
    vector<int> h(k);
    for (int j = 0; j < k; j++) {
      cin >> h[j];  
      is[h[j]] = 1;
    }
    T.build(h); 
    function<int(int, int)> dfs = [&] (int pos, int fa) {
      dp[pos] = 0;
      for (auto &v : T.tree[pos]) {
        if (v == fa) continue;
        int res = dfs(v, pos);
        if (is[v]) dp[pos] += w[v];
        else dp[pos] += min(w[v], res); 
      }
      return dp[pos];
    };
    dfs(1, -1);
    cout << dp[1] << endl;
    for (auto &it : h) {
      dp[it] = 0;
      is[it] = 0;
    }
  }

  return 0;
}
