//
//  main.cpp
//  2022-05-19 11:44
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;

// 用线段辅助理解
// ------------
// --------------------

// gcd 性质：
// 1. gcd(a, b) 是 ax + by 能得到的最小的正整数
// 2. 在 ax + by = c 中，如果 c 不是 gcd(a, b) 的倍数，则无解
// 3. gcd(a, b) = gcd(b, a % b)

// 功能说明：
// ax + by = c
// 1. 求出 x > 0, y > 0 解的数量
// 2. 求出 x > 0, y > 0 解中 x 的最大值，y 的最大值
//
// 注意:
// 1. g = -1 说明无解
// 2. dx, dy 大于 0
// 3. 必须用 long long，值域极限是 max(a, b) * c

struct GCD {
  int a, b, c, g;
  int x, y, dx, dy; // dx 和 dy 是一次移动最小的距离，dx, dy > 0

  void exgcd(int a, int b, int &d, int &x, int &y) {
    if (!b) {
      d = a; x = 1; y = 0;
    } else {
      exgcd(b, a % b, d, y, x); y -= x * (a / b); // 注意括号！
    }
  }

  GCD(int _a, int _b, int _c) : a(_a), b(_b), c(_c) {
    exgcd(a, b, g, x, y);
    if (c % g != 0) {
      g = -1;
      dx = dy = 0;
    } else {
      x *= c / g;
      y *= c / g;
      dx = abs(b / g);
      dy = abs(a / g);
    }
  }
  
  // 从 s 移动到超过 t 每次移动 d，需要的次数
  // s, t, d 可正可负 d 不能为 0
  int cnt(int s, int t, int d) {
    if (d < 0) d *= -1, swap(s, t);
    if (s < t) {
      if ((t - s) % d == 0) return (t - s) / d;
      return (t - s) / d + 1;
    } else {
      if ((t - s) % d == 0) return (t - s) / d;
      return (t - s) / d - 1;
    }
  }
    
  // 把 x 移动到最小正整数
  void move_x() {
    if (x > 0) {
      int cntx = cnt(x, 0, -dx);
      cntx--;
      x -= cntx * dx;
      y += cntx * dy;
    } else {
      int cntx = cnt(x, 1, dx);
      x += cntx * dx;
      y -= cntx * dy; 
    }
  }

  // 把 y 移动到最小正整数
  void move_y() {
    if (y > 0) {
      int cnty = cnt(y, 0, -dy);
      cnty--;
      y -= cnty * dy;
      x += cnty * dx;
    } else {
      int cnty = cnt(y, 1, dy);
      y += cnty * dy;
      x -= cnty * dx;
    }
  }
  
  // x > 0 并且 y > 0 的解的数量
  int count() { 
    move_x();   
    if (y <= 0) return 0; 
    int cnty = cnt(y, 0, -dy); 
    return cnty;
  }
   
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int z; cin >> z;
  while (z--) {
    int a, b, c; cin >> a >> b >> c;
    GCD T(a, b, c);
    if (T.g == -1) cout << -1 << endl;
    else {
      if (T.count() == 0) {
        T.move_x();
        cout << T.x << " ";
        T.move_y();
        cout << T.y << endl;
      } else {
        cout << T.count() << " ";
        T.move_x();
        int xmin = T.x, ymax = T.y;
        T.move_y();
        int ymin = T.y, xmax = T.x;
        cout << xmin << " " << ymin << " " << xmax << " " << ymax << endl;
      }
    }
  }

  return 0;
}

