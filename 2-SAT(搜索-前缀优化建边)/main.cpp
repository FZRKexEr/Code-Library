//
//  main.cpp
//  2022-04-22 20:36
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 这个模板未完成!!!
// 不要用!!!

struct TwoSAT{
  vector<vector<int>> G;
  vector<bool> vis;
  stack<int> stk;
  int n;

  TwoSAT(int _n) : G(_n * 2 + 2), vis(_n * 2 + 2, 0) { n = _n; }
 
  void addor(int a, int at, int b, int bt) {
    a += a + at;
    b += b + bt;
    G[a ^ 1].push_back(b); 
    G[b ^ 1].push_back(a); 
  }

  void addxor(int a, int at, int b, int bt) {
    a += a + at;
    b += b + bt;
    G[a].push_back(b ^ 1);
    G[b].push_back(a ^ 1);
  }

  bool dfs(int pos) {
    if (vis[pos ^ 1]) return false;
    if (vis[pos]) return true;
    vis[pos] = 1;
    stk.push(pos);
    for (auto &v : G[pos]) {
      if (!dfs(v)) return false;
    }
    return true;
  }

  bool sol() {
    random_device rd;
    mt19937 seed(rd());
    vector<int> ord(n + 1);
    iota(ord.begin(), ord.end(), 0);
    shuffle(ord.begin() + 1, ord.end(), seed); // 随机选点 dfs
    
    for (int i = 1; i <= n; i++) {
      int it = ord[i];
      if (!vis[it * 2 + 1] && !vis[it * 2]) {
        while (!stk.empty()) stk.pop();
        if (!dfs(it * 2)) {
          while (!stk.empty()) {
            vis[stk.top()] = 0;
            stk.pop();
          }
          if (!dfs(it * 2 + 1)) return false;
        }
      }
    }
    return true;
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0); 
  
  
  return 0;
}

