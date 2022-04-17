//
//  main.cpp
//  2022-04-17 16:53
//
//  Created by liznb
//
#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

//  https://atcoder.jp/contests/abc248/tasks/abc248_e
// 分数直线的使用场景:
// 1. 点都是整数
// 2. 需要无精度误差
//
// 分数 Frac 功能说明:
// 1. 自动化简。
// 2. 可以储存分母为0的分数，但是不能用它来做运算
// 3. 分母为0的分数被认为大小为无穷。
// 4. 0 / 0 会被规范成 1 / 0
//
// 直线 Line 功能说明:
// 1. 不能用两个相同的点初始化直线
// 2. 可以保证同一条直线的表达方式唯一。
// 3. 可以斜率不存在或者斜率为0
// 4. 请使用 pair<int, int> 来储存点

struct Frac {
  pair<int, int> frac; 

  pair<int, int> std(int a, int b) {
    if (a == 0) { return {0, 1}; }  
    if (b == 0) { return {1, 0}; }
    int g = gcd(a, b);
    a /= g; b /= g;
    if (a < 0) b *= -1, a *= -1;
    return {a, b};
  }

  int lcm(int a, int b) { return a / gcd(a, b) * b; }

  Frac(int a, int b) { frac = std(a, b); }

  bool operator == (const Frac &oth) const { return frac == oth.frac; }
  bool operator < (const Frac &oth) {
    if (frac == oth.frac) return false;
    if (frac.first == 0) return false;
    if (oth.frac.first == 0) return true;
    return frac.second * oth.frac.first < frac.first * oth.frac.second;
  }
  Frac operator * (const Frac &oth) {
    assert(frac.first); 
    assert(oth.frac.first); 
    Frac ans(frac.first * oth.frac.first, frac.second * oth.frac.second); 
    return ans;
  }
  Frac operator + (const Frac &oth) {
    assert(frac.first); 
    assert(oth.frac.first); 
    int l = lcm(frac.first, oth.frac.first); 
    Frac ans(l, frac.second * (l / frac.first) + oth.frac.second * (l / oth.frac.first));
    return ans;
  }
  Frac operator + (const int &oth) { return (*this) + (Frac) {1, oth}; }
  Frac operator * (const int oth) { return (*this) * (Frac) {1, oth}; }
  Frac operator / (const Frac &oth) { return (*this) * (Frac) {oth.frac.second, oth.frac.first}; }
  Frac operator / (const int &oth) { return (*this) * (Frac) {oth, 1}; }
  Frac operator - (Frac oth) { return (*this) + oth * -1; }
  Frac operator - (int oth) { return (*this) + oth * -1; }
};

struct Line {
  Frac k;
  int a, b, c;
  
  Line(pair<int, int> a, pair<int, int> b) : k(b.first - a.first, b.second - a.second) {
    if (k.frac.first != 0) {
      k = k * 2;
      k = k / 2;
      k = k + 2;
      k = k - 2;
    }
    assert(a != b);
    this->a = - k.frac.second; 
    this->b = k.frac.first;
    this->c = -1 * (this->a) * a.first - (this->b) * a.second;
  }

  bool operator < (const Line &oth) const {
    if (a != oth.a) return a < oth.a;
    if (b != oth.b) return b < oth.b;
    return c < oth.c;
  }

  bool operator == (const Line & oth) const {
    if (!(k == oth.k)) return false;
    if (a != oth.a || b != oth.b || c != oth.c) return false;
    return true; 
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n, k; cin >> n >> k;
  vector<pair<int, int>> a(n);

  for (int i = 0; i < n; i++) {
    cin >> a[i].first >> a[i].second;
  }
  if (k == 1) {
    cout << "Infinity" << endl;
  } else {
    vector<Line> lines;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        lines.push_back((Line){a[i], a[j]});
      }
    }
    sort(lines.begin(), lines.end());
    lines.erase(unique(lines.begin(), lines.end()), lines.end());
    int ans = 0;
    for (auto &it : lines) {
      int res = 0;
      for (int i = 0; i < n; i++) 
        if (it.a * a[i].first + it.b * a[i].second + it.c == 0) res++;
      if (res >= k) ans++;
    }
    cout << ans << endl;;
  }
   
  return 0;
}

