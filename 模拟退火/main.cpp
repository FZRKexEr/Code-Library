#include <bits/stdc++.h>
using namespace std;

// 模拟退火 Simulated Annealing
// 经验之谈：
// 1. SA 内部随机种子用固定的就好，方便调参数。
// 2. 尽量使用交题平台的在线测试调参，SA 会受机器影响。
// 3. eps 一般在 1e-12, eps 越大精度一定越高，但是单次退火时间就更长了。
//
// 参数：
// 1. T: 初始温度一般设成 1 - 3000。
// 2. D: 退火率一般是 0.995。注意 D 一定不能是 1
//
// 题目: 最小球覆盖 https://codeforces.com/gym/101981/problem/D

struct SA {
  const double eps = 1e-12;
  double T = 2022, D = 0.995;

  SA(double _T, double _D) {
    T = _T;
    D = _D;
  }

  // 剩下的都交给命运吧
  double fate(vector<array<double, 3>> &a, array<double, 3> ans) {

    auto H = [&] (array<double, 3> p) {
      double res = 0.0;
      auto dis = [&] (array<double, 3> x, array<double, 3> y) {
        return sqrt(
                (x[0] - y[0]) * (x[0] - y[0]) +
                (x[1] - y[1]) * (x[1] - y[1]) +
                (x[2] - y[2]) * (x[2] - y[2]) );
      };

      for (int i = 1; i < (int) a.size(); i++) {
        res = max(res, dis(a[i], p));
      }
      return res;
    };

    mt19937 rng(998244353);
    uniform_int_distribution<long long> R(-RAND_MAX, RAND_MAX);

    double global = H(ans);

    // 不要怀疑，这样移动是正确的。
    while (T > eps) {
      array<double, 3> pos;
      pos[0] = ans[0] + T * R(rng);
      pos[1] = ans[1] + T * R(rng);
      pos[2] = ans[2] + T * R(rng);
      double local = H(pos);
      if (local < global) {
        global = local;
        ans = pos;
      } else if (exp((global - local) / T) * RAND_MAX > abs(R(rng))) {
        ans = pos;
      }
      // 无论是否更优，都退火!
      T *= D;
    }
    return global;
  }
};

int main() {

  const double TL = 0.9;
  int start_time = clock();

  int n; cin >> n;
  vector<array<double, 3>> a(n + 1);

  array<double, 3> ans = {0, 0, 0};
  for (int i = 1; i <= n; i++) {
    cin >> a[i][0] >> a[i][1] >> a[i][2];
    ans[0] += a[i][0];
    ans[1] += a[i][1];
    ans[2] += a[i][2];
  }

  ans[0] /= 3;
  ans[1] /= 3;
  ans[2] /= 3;

  double minn = 1e18;

  mt19937 rng(998244353);

  while (1.0 * (clock() - start_time) < CLOCKS_PER_SEC * TL) {

    // T 的范围一般是 1-3000
    uniform_int_distribution<long long> RT(1, 3000);

    // 注意 RD 不能取到 1000
    uniform_int_distribution<long long> RD(900, 999);

    SA T(RT(rng), 1.0 * RD(rng) / 1000);

    double res = T.fate(a, ans);
    minn = min(minn, res);
  }

  cout << fixed << setprecision(10) <<  minn << endl;

  return 0;
}
