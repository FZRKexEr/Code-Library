//
//  main.cpp
//  2022-07-07 19:38
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 1. 筛法求欧拉函数 O(n)
struct Euler {
  vector<int> phi, is, prinum;
  
  Euler(int n) : phi(n + 1), is(n + 1, 0) {
    prinum.reserve(n + 1); 
    phi[1] = 1;
    for (int i = 2; i <= n; i++) {
      if (!is[i]) prinum.push_back(i), phi[i] = i - 1;
      for (auto &it : prinum) {
        if (it * i > n) break;
        is[i * it] = 1;
        if (i % it == 0) {
          phi[i * it] = phi[i] * it;
          break;
        } else {
          phi[i * it] = phi[i] * (it - 1);
        }
      }
    }
  }
};

// 2. 单点欧拉函数 O(sqrt(n)) (可以用PR优化)
int euler_phi(int n) {
  int ans = n;
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      ans = ans / i * (i - 1);
      while (n % i == 0) n /= i;
    }
  }
  if (n > 1) ans = ans / n * (n - 1);
  return ans;
}


int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  Euler T(100);   

  return 0;
}

