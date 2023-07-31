//
//  main.cpp
//
//  Created by LiZnB on 2020/12/1.
//

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, MOD; cin >> n >> MOD;

  vector<int> inv(n + 1);

  inv[1] = 1;
  for (int i = 2; i <= n; i++) {
    inv[i] = 1ll * (MOD - MOD / i) * inv[MOD % i] % MOD;
  }

  for (int i = 1; i <= n; i++) {
    cout << inv[i] << '\n';
  }

  return 0;
}
