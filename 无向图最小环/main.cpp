//
//  main.cpp
//  2022-04-12 11:53
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

// 无向图最小环
// 思想:
// 枚举环中相邻两条边，i-k, k-j. 加上 i-j 不经过 k 的最短路，更新答案。

const int N = 200;
int G[N][N], f[N][N];

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n, m; cin >> n >> m;  

  const int INF = 0x3f3f3f3f;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      f[i][j] = G[i][j] = INF;
    }
  }

  for (int i = 1; i <= m; i++) {
    int u, v, w; cin >> u >> v >> w;
    f[u][v] = f[v][u] = w;
    G[u][v] = G[v][u] = w;
  }

  for (int i = 1; i <= n; i++) f[i][i] = 0;

  int ans = INF;

  for (int k = 1; k <= n; k++) {
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        if (i == j || i == k || k == j) continue;
        ans = min(ans, f[i][j] + G[i][k] + G[k][j]);
      }
    }
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        f[i][j] = min(f[i][k] + f[k][j], f[i][j]);
      }
    }
  }
  if (ans == INF) 
    cout << "No solution.";
  else 
    cout << ans;
  return 0;
}
