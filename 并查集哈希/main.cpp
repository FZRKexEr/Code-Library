//
//  main.cpp
//  2022-08-15 03:23
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 并查集哈希
// 1. 对并查集的连通状态 hash, hash
//    相同说明连通性相同。
// 2. 请务必 #define int long long (这个代码能跑，建议不要动了)
// 3. 并查集是回滚并查集, 只需要把 rollback 删掉
//    然后路径压缩就可以变成普通并查集
// 
// 4. hash 坑点:
//    a. MOD 很大，所以做幂的时候要用 __int128
//    b. 异或操作不支持 MOD, 所以只能对 global_hash 取 MOD
//    c. hash 策略是对连通块做异或求2次方然后相加, 可以加强(?)成求size次方
//      (如果复杂度可以接受, 注意快速幂要局部__int128)。
//    d. 请使用随机时间种子，不要用固定种子，否则容易冲突。
//    e. 模数用的 1ll << 59 (随便选的)。可以考虑其他模数。
//

#define int long long

const int MOD = 1ll << 59;

struct DSU {
  vector<int> f, size, hash;
  vector<array<int, 3>> ops;
  int global_hash = 0;

  DSU(int n) : f(n + 1), size(n + 1, 1), hash(n + 1) {
    iota(f.begin(), f.end(), 0);

    mt19937 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<long long> R(0, MOD - 1);

    for (int i = 1; i <= n; i++) {
      hash[i] = R(rng);
      (global_hash += hash[i]) %= MOD;
    }
  }

  inline int find(int x) {
    while (x != f[x]) x = f[f[x]];
    return x;
  }

  inline bool merge(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (size[x] > size[y]) swap(x, y);

    ops.push_back({global_hash, 0, 0});
    ops.push_back({x, f[x], 1});
    ops.push_back({y, size[y], 2});
    ops.push_back({y, hash[y], 3});

    f[x] = y;

    global_hash = (global_hash - (__int128) hash[y] * hash[y] % MOD + MOD) % MOD;

    size[y] += size[x];

    global_hash = (global_hash - (__int128) hash[x] * hash[x] % MOD + MOD) % MOD;

    hash[y] ^= hash[x];

    global_hash = (global_hash + (__int128) hash[y] * hash[y] % MOD) % MOD;

    return true;
  }

  void rollback(int t) {
    while ((int) ops.size() > t) {
      auto [i, j, k] = ops.back();
      ops.pop_back();
      if (k == 0) {
        global_hash = i;
      } else if (k == 1) {
        f[i] = j;
      } else if (k == 2) {
        size[i] = j;
      } else if (k == 3) {
        hash[i] = j;
      }
    }
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

