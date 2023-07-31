#include <bits/stdc++.h>
using namespace std;

// 三分法
// 
// 关键是 midl == midr 的情况
// 1. 如果函数不存在平行于 x 轴的一段(不是一点)。
//    那么 midl == midr 时，最大值一定在它们之间。随便更新 l 和 r 中的一个就行。
// 2. 如果存在这样的情况，例如: 下标是整数的三分。
//    首先考虑能不能不用三分做这道题，如果下标是整数，可以直接枚举求出峰值。
//    如果必须三分，就要观察性质了。
//
// 整数三分, 仅供参考，不要复制粘贴

void fen(int limit, auto get_val) {
  int l = 0, r = limit;
  while (r - l + 1 >= 4) {
    int midl = l + (r - l) / 3, midr = l + (r - l) / 3 * 2;
    int res1 = get_val(midl);
    int res2 = get_val(midr);
    if (res1 <= res2) {
      l = midl;
    } else {
      r = midr;
    }
  }
  int ans = 0;
  for (int i = l; i <= r; i++) {
    ans = max(ans, get_val(i));
  }
  cout << ans << endl;
}


// 小数三分
// https://www.luogu.com.cn/problem/P3382

int main() {
  int n;
  double l, r; 
  cin >> n >> l >> r;
  vector<double> a(n + 1);

  auto f = [&](double x) {
    double ans = 0;
    for (int i = 0; i <= n; i++) {
      ans += a[i] * pow(x, i);
    }
    return ans;
  };

  for (int i = n; i >= 0; i--) {
    cin >> a[i];
  }

  const double eps1 = 1e-6;

  // 三分
  while (abs(r - l) > eps1) {
    double midl = l + (r - l) / 3;
    double midr = l + (r - l) / 3 * 2;
    double res1 = f(midl), res2 = f(midr);
    if (res1 <= res2) l = midl;
    else r = midr;
  }

  cout << fixed << setprecision(10) << l << endl;

  return 0;
}
