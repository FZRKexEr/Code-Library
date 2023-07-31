#include<bits/stdc++.h>
#define int long long
using namespace std;

const int N = 3e5 + 9;

const long long inf = 1e18;

template<typename T> struct PQ {
  long long sum = 0;
  priority_queue<T, vector<T>, greater<T>> Q;
  void push(T x) { x.w -= sum; Q.push(x); }
  T pop() { auto ans = Q.top(); Q.pop(); ans.w += sum; return ans; }
  int size() { return Q.size(); }
  void add(long long x) { sum += x; }
  void merge(PQ &&x) {
    if (size() < x.size()) swap(sum, x.sum), swap(Q, x.Q);
    while (x.size()) {
      auto tmp = x.pop();
      tmp.w -= sum;
      Q.push(tmp);
    }
  }
};
struct edge {
  int u, v; long long w;
  bool operator > (const edge &rhs) const { return w > rhs.w; }
};
struct DSU {
  vector<int> par;
  DSU (int n) : par(n, -1) {}
  int root(int i) { return par[i] < 0 ? i : par[i] = root(par[i]); }
  void set_par(int c, int p) { par[c] = p; }
};

// 复杂度 O(m + nlogn)
// 返回树中除root外的父节点
// DMST 接受从 0 开始编号的点!!! 且不能有重边，自环。答案必须存在。 
// Takes ~300ms for n = 2e5
vector<int> DMST(int n, int root, const vector<edge> &edges) {
  vector<int> u(2 * n - 1, -1), par(2 * n - 1, -1);
  edge par_edge[2 * n - 1];
  vector<int> child[2 * n - 1];

  PQ<edge> Q[2 * n - 1];
  for (auto e : edges) Q[e.v].push(e);
  for (int i = 0; i < n; i++) Q[(i + 1) % n].push({i, (i + 1) % n, inf});

  int super = n;
  DSU dsu(2 * n - 1);
  int head = 0;
  while (Q[head].size()) {
    auto x = Q[head].pop();
    int nxt_root = dsu.root(x.u);
    if (nxt_root == head) continue;
    u[head] = nxt_root;
    par_edge[head] = x;
    if (u[nxt_root] == -1) head = nxt_root;
    else {
      int j = nxt_root;
      do {
        Q[j].add(-par_edge[j].w);
        Q[super].merge(move(Q[j]));
        assert(u[j] != -1);
        child[super].push_back(j);
        j = dsu.root(u[j]);
      } while (j != nxt_root);
      for (auto u : child[super]) par[u] = super, dsu.set_par(u, super);
      head = super++;
    }
  }
  vector<int> res(2 * n - 1, -1);
  queue<int> q; q.push(root);
  while (q.size()) {
    int u = q.front();
    q.pop();
    while (par[u] != -1) {
      for (auto v : child[par[u]]) {
        if (v != u) {
          res[par_edge[v].v] = par_edge[v].u;
          q.push(par_edge[v].v);
          par[v] = -1;
        }
      }
      u = par[u];
    }
  }
  res[root] = root; res.resize(n);
  return res;
}

int pre(int n, int root, vector<edge> &edges) {
  vector<edge> res; 
  map<int, int> w[n], vis[n];
  for (auto &e : edges) {
    if (vis[e.u][e.v]) w[e.u][e.v] = min(w[e.u][e.v], e.w);
    else {
      vis[e.u][e.v] = 1;
      w[e.u][e.v] = e.w;
    }
  }
  for (int i = 0; i < n; i++) {
    for (auto &it : w[i]) {
      if (i != it.first) res.push_back({i, it.first, it.second});
    }
  }

  edges = res;

  queue<int> que; que.push(root);
  vector<int> used(n, 0); used[root] = 1; 

  while (!que.empty()) {
    int x = que.front(); que.pop();      
    for (auto &it : w[x]) {
      if (used[it.first]) continue;       
      used[it.first] = 1;
      que.push(it.first);
    }
  }
  for (int i = 0; i < n; i++) 
    if (!used[i]) return 0;
  return 1;
}

signed main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  int n, m, root; cin >> n >> m >> root;
  root--;

  vector<edge> edges(m); 
  for (auto &e : edges) {
    cin >> e.u >> e.v >> e.w;
    e.u--, e.v--;  // 注意DMST接受从0开始的点。
  }

  if (!pre(n, root, edges)) { //去除重边，自环，判断是否存在答案。
    cout << -1 << endl; 
    return 0;
  }
   
  auto res = DMST(n, root, edges); 

  map<int, int> w[n];   
  for (auto &e : edges) w[e.v][e.u] = e.w;

  long long ans = 0ll; 
  for (int i = 0; i < n; i++) {
    if (i == root) continue;
    ans += w[i][res[i]];
  }
  cout << ans << endl; 
  return 0;
}

// https://judge.yosupo.jp/problem/directedmst
// http://www.cs.tau.ac.il/~zwick/grad-algo-13/directed-mst.pdf
