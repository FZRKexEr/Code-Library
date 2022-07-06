#include <bits/stdc++.h>
#define int long long
using namespace std;

// 类欧几里得
// 求 f(n, m, a, b) = sum{ (i * a + b) / m | 0 <= i <= n - 1 }
// 注意是 0 到 n - 1 !!!
// 复杂度和 gcd 一样。
int sum_of_floor(int n, int m, int a, int b) {
  int ans = 0;
  if (a >= m) {
    ans += (n - 1) * n * (a / m) / 2;
    a %= m;
  }
  if (b >= m) {
    ans += n * (b / m);
    b %= m;
  }
  int y_max = (a * n + b) / m, x_max = (y_max * m - b);
  if (y_max == 0) return ans;
  ans += (n - (x_max + a - 1) / a) * y_max;
  ans += sum_of_floor(y_max, a, m, (a - x_max % a) % a);
  return ans;
}

signed main() {
  int z; cin >> z;
  while (z--) {
    int n, m, a, b; cin >> n >> m >> a >> b;
    cout << sum_of_floor(n, m, a, b) << endl;
  }
  return 0;
}
