//
//  main.cpp
//  2022-12-28 22:17
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'


// 树的重心
// 1. 树的重心到所有点的距离之和最小
// 2. 用换根 dp 求出每一个点到所有点的距离，然后判断重心
// 3. 重心只有可能有 1 个或者 2 个
// 4. 以树的重心为根时，所有子树的大小都不超过整棵树大小的一半。(想象一下小球重量就可以简单证明)
// 5. 把两棵树通过一条边相连得到一棵新的树，那么新的树的重心在连接原来两棵树的重心的路径上。
// 6. 在一棵树上添加或删除一个叶子，那么它的重心最多只移动一条边的距离。
// 7. 树可以带点权，这样也存在重心, 但是要改代码(想象一下不同重量的小球)
//
// 常见应用:
// 1. 无根树求重心转有根树，然后求树 hash, 注意判断重心数量

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n; cin >> n;
  vector<vector<int>> g(n + 1);

  for (int i = 1; i <= n - 1; i++) {
    int u, v; cin >> u >> v;
    g[u].emplace_back(v);
    g[v].emplace_back(u);
  }

  vector<long long> cnt(n + 1, 0), d(n + 1, 0);
  function<void(int, int)> dfs1 = [&] (int pos, int fa) {
    cnt[pos] = 1;
    for (auto &v : g[pos]) {
      if (v == fa) continue;
      dfs1(v, pos);
      d[pos] += cnt[v] + d[v];
      cnt[pos] += cnt[v];
    }
  };
  dfs1(1, -1);

  long long dis = d[1], ans = 1e18;
  vector<int> p; // 存放重心
  function<void(int, int)> dfs2 = [&] (int pos, int fa) {
    if (dis < ans) {
      ans = dis;
      p = {pos};
    } else if (dis == ans) {
      p.emplace_back(pos);
    }

    for (auto &v : g[pos]) {
      if (v == fa) continue;
      dis -= cnt[v];
      dis += n - cnt[v];
      dfs2(v, pos);
      dis += cnt[v];
      dis -= n - cnt[v];
    }
  };

  dfs2(1, -1);

  return 0;
}
