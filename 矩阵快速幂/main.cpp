//
//  main.cpp
//  2022-12-29 16:37
//
//  Created by liznb
//

#include <bits/stdc++.h>
using namespace std;
#define endl '\n'

// 矩阵快速幂
//
// 1. 注意单位矩阵要放在左边乘

const int MOD = 1e9 + 7;
const int N = 3;

struct Matrix {
  array<array<int, N>, N> m{};

  friend Matrix operator * (const Matrix &x, const Matrix &y) {
    Matrix res;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
          res.m[i][j] = (1ll * res.m[i][j] + 1ll * x.m[i][k] * y.m[k][j] % MOD) % MOD;
        }
      }
    }
    return res;
  }

  void id() {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        m[i][j] = i == j;
      }
    }
  }
};

Matrix power(Matrix base, long long b) {
  Matrix ans; ans.id();
  while (b) {
    if (b & 1) ans = ans * base;
    base = base * base;
    b >>= 1;
  }
  return ans;
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  return 0;
}
