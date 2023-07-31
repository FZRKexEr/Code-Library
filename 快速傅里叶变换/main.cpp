//
//  main.cpp
//  2022-07-30 15:41
//
//  Created by liznb
//
#include <bits/stdc++.h>
using namespace std;

// FFT 求多项式乘法
// 注意:
//   1. 请使用快读. std::cin 可以读入实部, 快读一般不行。
//   2. 初始化读入一个 vector, 下标从 0 开始，表示系数, 不能传空 vector
//   3. 模板内会自动处理系数长度，不需要传入长度为 2 的幂的 vector。
//   4. 模板速度: 快读 + 单次 1e6 长度的多项式乘法 1.2s
//   5. 多项式 c = a * b, c 的长度为 a.size() + b.size() - 1
//   6. !!! 输出的是得到的复数的实部，为了减少精度问题需要四舍五入到整数, 注意写法。

struct Poly {

  vector<complex<double>> c;
  vector<int> rev;

  Poly(vector<complex<double>> &_c) : c(_c) {
    assert((int) c.size());
  }

  void dft(int limit, int o) {
    assert((limit & (-limit)) == limit);
    assert(rev.empty());
    assert(limit >= (int) c.size());

    c.resize(limit, {0, 0});
    rev.resize(limit, 0);

    for (int i = 0; i < limit; i++) {
      rev[i] = rev[i >> 1] >> 1;
      if (i & 1) rev[i] |= limit >> 1;
    }
    for (int i = 0; i < limit; i++) {
      if (i < rev[i]) {
        swap(c[i], c[rev[i]]);
      }
    }
    const double PI = acos(-1);
    for (int d = 0; (1 << d) < limit; d++) {
      int son = 1 << d, fa = son << 1;
      for (int i = 0; i < limit; i += fa) {
        complex<double> w(1, 0), step(cos(PI / son), sin(o * PI / son));
        for (int j = i; j < i + son; j++) {
          complex<double> x = c[j];
          complex<double> y = c[j + son] * w;
          c[j] = x + y;
          c[j + son] = x - y;
          w *= step;
        }
      }
    }
    if (o == -1) {
      for (int i = 0; i < limit; i++)
        c[i].real(c[i].real() / limit);
    }
  }

  friend Poly operator * (Poly a, Poly b) {
    int limit = 1;
    int len = (int) a.c.size() + (int) b.c.size() - 1;
    while (limit < len) limit <<= 1;

    a.dft(limit, 1); b.dft(limit, 1);

    vector<complex<double>> res(limit);
    for (int i = 0; i < limit; i++) {
      res[i] = a.c[i] * b.c[i];
    }
    Poly ans(res);
    ans.dft(limit, -1);
    ans.c.resize(len);
    return ans;
  }

  Poly &operator *= (const Poly &b) {
    return (*this) = (*this) * b;
  }
};


int main() {
  int n, m; cin >> n >> m;
  vector<complex<double>> a(n + 1), b(m + 1);

  for (int i = 0; i <= n; i++) cin >> a[i];
  for (int i = 0; i <= m; i++) cin >> b[i];

  Poly c(a), d(b);
  c *= d;

  for (auto &it : c.c) cout << (int) floor(it.real() + 0.5) << " ";

  return 0;
}
