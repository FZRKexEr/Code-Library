//
//  main.cpp
//  2022-04-05 20:06
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

struct DSU {
  vector<int> f, dep, size;   
  DSU(int n) : f(n + 1), dep(n + 1, 0), size(n + 1, 1) { iota(f.begin(), f.end(), 0); }

  inline int find(int x) {
    while (x != f[x]) x = f[x] = f[f[x]];
    return x;
  }

  inline bool merge(int x, int y) {
    x = find(x), y = find(y); 

    if (x == y) return false;
    if (dep[x] > dep[y]) swap(x, y);
    if (dep[x] == dep[y]) dep[y]++;

    f[x] = y;    
    size[y] += size[x];
    return true;
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

