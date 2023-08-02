//
//  main.cpp
//  2022-06-05 13:51
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;

// 莫比乌斯反演:
// 注意: 一定打开 #define int long long
//
// 两个公式: 
// 1. f(n) = sigma{u(d) * F(n / d)} (d|n)
// 2. f(n) = sigma{u(d / n) * F(d)} (n|d)
//
// 莫比乌斯反演题特点：
// 1. 一般和 GCD，约数有关。 
// 2. 简单的莫比乌斯反演题可以直接套 gcd 计数, 基本不需要推式子。
// 3. 复杂的莫比乌斯反演需要把GCD计数写进多重和式里，然后推式子构造适合整除分块的式子，化简复杂度。
//
// 经典构造:
// 求 gcd(i, j) = k 的对数，其中 i <= n, j <= m
// 设 f(k) 是答案。F(k) 是 i <= n, j <= m 中 k | gcd(i, j) 的对数。
// 显然 F(k) = (n / k) * (m / k) = sigma{f(d)} (k | d);
// 所以 f(k) = sigma{u(d / k) * (n / d) * (m / d)} (k | d);
// 进一步转换，gcd(i, j) = k 与 gcd(i / k, j / k) = 1 等价。
// 所以只需要每次求 f(1), 但 n / k 和 m / k 不同的情况。用整除分块优化成 sqrt(min(n / k, m / k))
//
// 整除分块:
// i 从小到大, n / i 不相等的下一个 i 是 n / (n / i) + 1

struct Mobius {
  vector<int> is, prinum, miu, sum;

  Mobius(int n) : is(n + 1, 0), miu(n + 1, 1), sum(n + 1, 0) {
    prinum.reserve(n + 1);
    for (int i = 2; i <= n; i++) {
      if (!is[i]) prinum.push_back(i), miu[i] = -1;  
      for (auto &it : prinum) {
        if (it * i > n) break;
        is[it * i] = 1;
        if (i % it == 0) {
          miu[it * i] = 0;
          break;
        } else {
          miu[it * i] = -1 * miu[i];
        }
      }
    }
    for (int i = 1; i <= n; i++) {
      sum[i] = sum[i - 1] + miu[i];
    }
  }

  // gcd(i, j) = k (1 <= i <= n,  1 <= j <= m) 的对数
  // 复杂度 
  // 1. sqrt(min(n / k, m / k)) 单次
  // 2. 如果求 k 从 1 到 t, 复杂度低于 tlogt 
  //
  // 进一步优化策略:
  // 1. 如果 k 从 1 到 t: 外层循环可以再套一个整除分块，因为g函数里开头做了除法。
  // 2. 如果 k 是 质数: 用类似埃筛的方法把每一个质数的倍数处的 miu 函数贡献累计一下(不再使用整除分块)。复杂度 O(n + nloglogn)
  //    提示：这是一个经典变换,公式需要交换求和次序，求出每一个 (n / t) * (m / t) (其中 t = k * d) 的系数. 把t提到最外层,这个系数和 n, m 无关，可以预处理。
  // 3. 如果 n 和 m 始终一样，可以记忆化。
  //    更好的做法是转化成欧拉函数 g(n, n, k) = 2 * sigma{phi(i)} - 1 (i <= n / k)
  
  int g(int n, int m, int k) {
    n /= k, m /= k; // 套用外层整除分块需要删除所有k。
    int limit = min(n, m);
    int ans = 0, nxt;
    for (int i = 1; i <= limit; i = nxt) {
      int nxtn = n / (n / i) + 1;
      int nxtm = m / (m / i) + 1;
      nxt = min({nxtn, nxtm, limit + 1});
      ans += (sum[nxt - 1] - sum[i - 1]) * (n / i) * (m / i);
    }
    return ans;
  }
  
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0); 
  
  return 0;
}
