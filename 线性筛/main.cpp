//
//  main.cpp
//  2022-10-27 19:47
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

namespace Sieve {
  vector<int> is, prinum;

  void sieve (int n) {
    is.resize(n + 1, 0);
    prinum.reserve(n + 1);
    for (int i = 2; i <= n; i++) {
      if (!is[i]) prinum.push_back(i);
      for (const auto &it : prinum) {
        if (it * i > n) break;
        is[i * it] = 1;
        if (i % it == 0) break;
      }
    }
  }
}


signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  return 0;
}
