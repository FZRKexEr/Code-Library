//
//  main.cpp
//  2022-07-17 22:56
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 自适应辛普森
// 1. 求函数在 l, r 范围的积分。
// 2. 复杂度..自己试一下。
// 3. 注意边界取值，例如 pow(0, 0) 会出错，所以在示例中使用 T.eps 作为左边界。

struct Simpson {

  const double eps = 1e-9;

  // 待积分函数需要的常数。
  double A;

  Simpson(double _a) : A(_a) {}

  // 修改需要积分对函数
  double f(double x) {
    return pow(x, A / x - x);
  }
   
  // 不要动辛普森公式
  double simpson(double l, double r) {
    return (r - l) * (f(l) + f(r) + 4.0 * f((r + l) / 2.0)) / 6.0;
  }

  double solve(double l, double r) {
    double mid = (l + r) / 2.0;
    double ans = simpson(l, r); 
    double ans_l = simpson(l, mid);
    double ans_r = simpson(mid, r);
    if (fabs(ans_l + ans_r - ans) < eps) {
      return ans_l + ans_r;
    } else {
      return solve(l, mid) + solve(mid, r);
    }
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  double a; cin >> a;

  Simpson T(a);

  if (a < 0.0) cout << "orz" << endl;
  else cout << fixed << setprecision(5) << T.solve(T.eps, 20.0);

  return 0;
}

