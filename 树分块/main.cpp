//
//  2023-03-19 19:43
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

// 树分块 P2325
//
// 可以把整颗树分成大小为 B 到 3B 的块
// 每块有一个根，块内所有点到根的路径上的点都在块内
// A块的根可能在B块内
// A块的根可能也是C块的根

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, B; cin >> n >> B;

  vector<vector<int>> G(n + 1);
  for (int i = 1; i <= n - 1; i++) {
    int u, v; cin >> u >> v;
    G[u].emplace_back(v);
    G[v].emplace_back(u);
  }

  // belong 和 root 是分块结果
  vector<int> remain, belong(n + 1), root;
  function<void(int, int)> dfs = [&](int pos, int fa) {
    int base = remain.size();

    for (auto v : G[pos]) {
      if (v == fa) continue;
      dfs(v, pos);
      if ((int) remain.size() - base >= B) {
        root.emplace_back(pos);
        while ((int) remain.size() > base) {
          belong[remain.back()] = (int) root.size();
          remain.pop_back();
        }
      }
    }
    remain.emplace_back(pos);
  };

  dfs(1, -1);
  while (!remain.empty()) {
    belong[remain.back()] = (int) root.size();
    remain.pop_back();
  }
  // 分块结束

  cout << root.size() << endl;
  for (int i = 1; i <= n; i++) {
    cout << belong[i] << " ";
  }
  cout << endl;
  for (int i = 0; i < (int) root.size(); i++) {
    cout << root[i] << " ";
  }

  return 0;
}

