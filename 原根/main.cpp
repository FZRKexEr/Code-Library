//
//  main.cpp
//  2022-07-07 01:02
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

struct Primitive_Root {

  long long euler_phi(long long n) {
    long long ans = n;
    for (int i = 2; i * i <= n; i++) {
      if (n % i == 0) {
        ans = ans / i * (i - 1);
        while (n % i == 0) n /= i;
      }
    }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
  }

  int power(int a, int b, int p) {
    assert(b >= 0);
    int base = a, ans = 1;
    while (b) {
      if (b & 1) ans = ans * base % p;
      base = base * base % p;
      b >>= 1;
    }
    return ans;
  }

  // 判断是否存在原根 O(m ^ 0.5)
  // 即 m 是否属于 2, 4, p^n, 2 * p^n
  bool exist(long long m) {
    if (m == 2 || m == 4) return true;
    if (m % 2 == 0) m /= 2;
    for (int i = 2; i * i <= m; i++) {
      if (m % i == 0) {
        while (m % i == 0) m /= i;
        if (m != 1) return false;
        return true;
      }
    }
    if (m != 1) return true; 
    return false;
  }
  
  // 求最小原根, 如果不存在原根，返回-1
  // 粗略估计复杂度上界 O(m ^ 0.75)
  
  int Minimum_root(long long m) {
    if (!exist(m)) return -1;

    int phi_m = euler_phi(m);  
    vector<int> factor;
    
    for (int i = 1; i * i <= phi_m; i++) {
      if (phi_m % i) continue;
      if (i != phi_m) factor.push_back(i);
      if (phi_m / i != i) {
        if (phi_m / i != phi_m) factor.push_back(phi_m / i);
      }
    }
  
    for (int i = 1; ; i++) {
      if (gcd(i, m) != 1) continue;
      int flag = true;
      for (auto &it : factor) {
        if (power(i, it, m) == 1) {
          flag = false;
          break;
        }
      }
      if (flag) return i;
    }
  }

  // 求所有原根
  // 复杂度上界 O(nlogn)
  vector<int> all_root(long long m) {
    if (!exist(m)) return {};

    int g = Minimum_root(m);
    vector<int> root;
    long long phi_m = euler_phi(m); 
    long long res = g;
    
    for (int i = 1; i <= phi_m; i++) {
      if (gcd(i, phi_m) == 1) root.push_back(res);
      res = res * g % m;
    }

    sort(root.begin(), root.end());
    return root;
  }
};

int main() {
  freopen("in.txt", "r", stdin);
//  freopen("out.txt", "w", stdout);
  ios::sync_with_stdio(false); 
  cin.tie(0);


  Primitive_Root T; 
  cout << T.Minimum_root(319981) << endl;
  return 0;

  int z; cin >> z;
  while (z--) {
    int n, d; cin >> n >> d;
    auto ans = T.all_root(n);
    cout << ans.size() << endl;
    for (int i = d - 1; i < (int) ans.size(); i += d) {
      cout << ans[i] << " ";
    }
    cout << endl;
  }

  return 0;
}

