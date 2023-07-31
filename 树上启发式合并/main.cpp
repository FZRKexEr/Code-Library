#include <bits/stdc++.h>
using namespace std;
#define endl '\n'

// Dsu On Tree
// 核心思想: 
// 1. 先进入轻儿子统计答案, 然后删除对 cnt 的贡献
// 2. 然后进入重儿子统计答案，不删除对 cnt 的贡献
// 3. 暴力遍历轻儿子, 把亲儿子对 cnt 的贡献，加入到重儿子留下来的 cnt 中
// 
// 例题: cf600e

const int N = 1e6 + 10;
vector<int> G[N];

struct dsu_on_tree {

  // cnt: 子树中每个颜色的出现次数
  // st, ft: dfs序的开始和结束
  // ver: 时间戳对应的点
  // res: 保存 pos 点的答案
  int cnt[N], st[N], ft[N], ver[N];
  int sz[N], color[N], tm = 0;
  long long mxans = -1, ans = -1;
  long long res[N];

  dsu_on_tree() {
    memset(cnt, 0, sizeof(cnt));
  }

  // 预处理子树大小
  void pre(int pos, int fa) {
    sz[pos] = 1;
    for (auto v : G[pos]) {
      if (v == fa) continue;
      pre(v, pos);
      sz[pos] += sz[v];
    }
  }

  // 用 dsu on tree 得到每个子树的信息
  void dfs(int pos, int fa, bool keep) {
    int mx = -1, bigChild = -1;
    st[pos] = ++tm; // 更新 dfs 序
    ver[tm] = pos;
    for (auto v : G[pos]) {
      if (v == fa) continue;
      if (sz[v] > mx) {
        mx = sz[v];
        bigChild = v;
      }
    }
    for (auto v : G[pos]) { // 去轻儿子
      if (v == fa || v == bigChild) continue;
      dfs(v, pos, 0);
    }
    if (bigChild != -1) { // 去重儿子
      dfs(bigChild, pos, 1);
    }
    ft[pos] = tm;

    // 答案维护
    for (auto v : G[pos]) { // 轻儿子答案合并到重儿子
      if (v == fa || v == bigChild) continue;

      // 用 dfs 序遍历子树, 更新答案，不同题目这部分不相同。
      for (int i = st[v]; i <= ft[v]; i++) {
        cnt[color[ver[i]]]++;
        if (cnt[color[ver[i]]] > mxans) {
          mxans = cnt[color[ver[i]]];
          ans = color[ver[i]];
        } else if (cnt[color[ver[i]]] == mxans) {
          ans += color[ver[i]];
        }
      }
    }

    cnt[color[pos]]++; // 加入根的信息，现在cnt里面就是以pos为根的答案

    // 更新答案，不同题目这里不相同。
    if (cnt[color[pos]] > mxans) {
      mxans = cnt[color[pos]];
      ans = color[pos];
    } else if (cnt[color[pos]] == mxans) {
      ans += color[pos];
    }

    res[pos] = ans; // pos 为根的答案存进 res[pos]

    if (keep == 0) { // 不是重儿子就要清除对答案对贡献
      mxans = -1, ans = -1;
      for (int i = st[pos]; i <= ft[pos]; i++) {
        cnt[color[ver[i]]]--;
      }
    }
  }
} T;

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n; cin >> n;
  for (int i = 1; i <= n; i++) cin >> T.color[i];
  for (int i = 1; i <= n - 1; i++) {
    int u, v; cin >> u >> v;
    G[u].push_back(v);
    G[v].push_back(u);
  }
  T.pre(1, -1);
  T.dfs(1, -1, 1);
  for (int i = 1; i <= n; i++)
    cout << T.res[i] << " ";
   
  return 0;
}
