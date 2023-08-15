//
//  main.cpp
//  2022-06-05 13:53
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 说明:
// 1. is = 0 时是质数
// 2. 线性筛：利用莫比乌斯函数的积性
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
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  Mobius T(100);
  for (int i = 1; i <= 100; i++) {
    cout << T.miu[i] << " ";  
  }

  return 0;
}

