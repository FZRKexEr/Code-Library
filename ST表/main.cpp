//
//  main.cpp
//  2023-04-21 22:39
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'


// ST 表
// 一般情况 ST 表没什么用，但是如果涉及区间 GCD，ST 表可以让复杂度少个 log，这就很关键了。

struct ST {
  vector<vector<int>> f;
  vector<int> log;

  ST(int n, vector<int> &a) {
    log.resize(n + 1, 0);
    for (int i = 2; i <= n; i++) log[i] = log[i / 2] + 1;

    f.resize(n + 1, vector<int>(log[n] + 1));

    for (int i = 1; i <= n; i++) f[i][0] = a[i];

    for (int i = 1; i <= log[n]; i++) {
      for (int j = (1 << i); j <= n; j++) {
        f[j][i] = gcd(f[j][i - 1], f[j - (1 << (i - 1))][i - 1]);
      }
    }
  }
  int query(int l, int r) {
    assert(l <= r && l >= 1);
    int len = r - l + 1;
    if ((1 << log[len]) == len) return f[r][log[len]];
    return gcd(f[l + (1 << log[len]) - 1][log[len]], f[r][log[len]]);
  }
};

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);


  return 0;
}
