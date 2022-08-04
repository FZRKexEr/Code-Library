#include <bits/stdc++.h>
using namespace std;

struct Poly {
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
  
    long long power(long long a, long long b, long long p) {
      assert(b >= 0);
      long long base = a, ans = 1ll;
      while (b) {
        if (b & 1ll) ans = ans * base % p;
        base = base * base % p;
        b >>= 1ll;
      }
      return ans;
    }
  
    bool exist(long long m) {
      if (m == 2 || m == 4) return true;
      if (m % 2 == 0) m /= 2;
      if (m % 2 == 0) return false;
      for (int i = 3; 1ll * i * i <= m; i++) {
        if (m % i == 0) {
          while (m % i == 0) m /= i;
          if (m != 1) return false;
          return true;
        }
      }
      if (m != 1) return true; 
      return false;
    }
  
    long long Minimum_root(long long m) {
      if (!exist(m)) return -1;
      long long phi_m = euler_phi(m);
      vector<long long> prime;
      long long val = phi_m;
      for (int i = 2; 1ll * i * i <= val; i++) {
        if (val % i == 0) {
          while (val % i == 0) val /= i;
          prime.push_back(i);
        }
      }
      if (val != 1) prime.push_back(val);
      for (int i = 1; ; i++) {
        if (gcd(i, m) != 1) continue;
        int flag = true;
        for (auto &it : prime) {
          if (power(i, phi_m / it, m) == 1) {
            flag = false;
            break;
          }
        }
        if (flag) return i;
      }
    }
  };

  vector<long long> c;
  vector<int> rev;
  long long MOD;
  int g;

  Poly(vector<long long> &_c, long long _MOD) : c(_c), MOD(_MOD) {
    assert((int) c.size());
    Primitive_Root T;
    g = T.Minimum_root(MOD);
    assert(g != -1);
  }

  void dft(int limit, int o) {
    assert((limit & (-limit)) == limit);
    assert(rev.empty());
    assert(limit >= (int) c.size());

    c.resize(limit, 0);
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

  Poly &operator *= (Poly b) {
    return (*this) = (*this) * b;
  }
};

int main() {

  return 0;
}
