//
//  main.cpp
//  2023-08-16 00:25
//
//  Created by liznb
//

#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

// 点分治
//
// 无根树，有边权，问距离等于k的点对是否存在。
// 
// 思路: 
// 1. 以树的重心为根，找出经过这个根的所有点对(u, v)
// 2. 然后去子树里面找重心，重复这个步骤，可以证明最多只找 logn 次重心。
//
// 复杂度: 桶排序, 单次点分治 nlogn, m次查询复杂度 mnlogn

int has[10000000 + 10];
vector<int> in_has;

struct Point_Divide {

  vector<vector<pair<int, int>>> g;
  vector<int> vis, size, len, ans;
  int n, m;

  Point_Divide(int _n, int _m) : n(_n), m(_m) {
    g.resize(n + 1);
    vis.resize(n + 1, 0);
    size.resize(n + 1, 0);
    len.resize(n + 1, 0);
    ans.resize(m + 1, 0);
  }

  void add(int u, int v, int w) {
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
  }

  void get_size(int pos, int fa) {
    size[pos] = 1;
    for (auto &[v, w] : g[pos]) {
      if (v == fa || vis[v]) continue;
      get_size(v, pos);
      size[pos] += size[v];
    }
  }

  int get_center(int pos, int fa, int root) {
    int weight = size[root] - size[pos];
    int center = -1;
    for (auto &[v, w] : g[pos]) {
      if (v == fa || vis[v]) continue;
      weight = max(weight, size[v]);
      center = max(get_center(v, pos, root), center);
    }
    if (weight <= size[root] / 2) center = max(center, pos);
    return center;
  }

  void get_length(int pos, int fa) {
    for (auto &[v, w] : g[pos]) {
      if (v == fa || vis[v]) continue;
      len[v] = len[pos] + w;
      get_length(v, pos);
    }
  }

  void solve(int pos, vector<int> query) {
    // 固定步骤
    // 1. 预处理 size, center
    // 2. 处理以 center 为根的各个子树的 length
    // 3. 枚举 center 的每个儿子, find 然后 add, 避免混淆
    // 4. find 的时候遍历所有询问。
    
    get_size(pos, -1);
    int center = get_center(pos, -1, pos);
    len[center] = 0;
    get_length(center, -1);

    // find 需要改
    auto find = [&](auto self, int pos, int fa) -> void {
      for (int i = 1; i <= m; i++) { // 处理 m 个询问
        int it = query[i];
        if (it - len[pos] >= 0 && has[it - len[pos]]) ans[i] = 1;
      }
      for (auto &[v, w] : g[pos]) {
        if (v == fa || vis[v]) continue;
        self(self, v, pos);
      }
    };

    // add 需要改
    auto add = [&](auto self, int pos, int fa) -> void {
      if (len[pos] <= 10000000) {
        has[len[pos]] = 1;
        in_has.emplace_back(len[pos]);
      }
      for (auto &[v, w] : g[pos]) {
        if (v == fa || vis[v]) continue;
        self(self, v, pos);
      }
    };

    // 桶排序可能需要改 map
    for (auto &it : in_has) has[it] = 0;
    in_has.clear();

    in_has.emplace_back(0);
    has[0] = 1;

    for (auto &[v, w] : g[center]) {
      if (vis[v]) continue;
      find(find, v, center);
      add(add, v, center);
    }
    vis[center] = 1;
    for (auto &[v, w] : g[center]) {
      if (vis[v]) continue;
      solve(v, query);
    }
  }
};


int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m; cin >> n >> m;

  Point_Divide T(n, m);
  for (int i = 1; i <= n - 1; i++) {
    int u, v, w; cin >> u >> v >> w;
    T.add(u, v, w);
  }

  vector<int> query(m + 1);

  for (int i = 1; i <= m; i++) {
    int k; cin >> k;
    query[i] = k;
  }

  T.solve(1, query);

  for (int i = 1; i <= m; i++) {
    if (T.ans[i]) cout << "AYE" << endl;
    else cout << "NAY" << endl;
  }

  return 0;
}
