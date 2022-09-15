#include<bits/stdc++.h>
using namespace std;

// 普通 CRT
//
// 说明:
// 1. 返回同余方程组的解
// 2. 需要保证 a(模数) 数组内互质
// 3. 同余方程格式 x mod a = b
//
// 注意:
// 1. 下标从 0 开始。
// 2. 只能用 exgcd 求逆元，因为不保证 a 是质数。
// 3. a 的累乘需要在 long long 范围内。
// 4. 看看会不会爆 long long

void exgcd(long long a, long long b, long long &d, long long &x, long long &y) {
  if (!b) {
    d = a; x = 1; y = 0;
  } else {
    exgcd(b, a % b, d, y, x); y -= x * (a / b);
  }
}

long long inv(long long val, long long p) {
  long long g, x, y;
  exgcd(val, p, g, x, y);
  return (x % p + p) % p;
}

long long crt(vector<long long> &a, vector<long long> &b) {
  long long all = 1, ans = 0;
  for (auto it : a) all *= it;
  for (int i = 0; i < (int) a.size(); i++) {
    long long r = all / a[i], invr = inv(r, a[i]);
    ans = (ans + b[i] * r % all * invr % all) % all;
  }
  return ans;
}

int main() {
  int n; cin >> n;

  vector<long long> a(n), b(n);
  for (int i = 0; i < n; i++) cin >> a[i] >> b[i];

  cout << crt(a, b);

  return 0;
}
