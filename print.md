# 1. 2-SAT(搜索)

```cpp
//
//  main.cpp
//  2022-04-21 21:00
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

// 标准 2-SAT
// 暴搜 + 随机化
// 期望复杂度 O(nlogn ?), 实测效率优秀。
// 下标从1开始。
// 注意：2-sat 很简单，如果不赶时间建议理解代码再粘板子

struct TwoSAT{
  vector<vector<int>> G;
  vector<bool> vis;
  stack<int> stk;
  int n;

  TwoSAT(int _n) : G(_n * 2 + 2), vis(_n * 2 + 2, 0) { n = _n; }
 
  // a = at, b = bt 至少一个成立
  void addor(int a, int at, int b, int bt) {
    a += a + at;
    b += b + bt;
    G[a ^ 1].push_back(b); 
    G[b ^ 1].push_back(a); 
  }

  // a = at, b = bt 不能同时成立
  void addnand(int a, int at, int b, int bt) {
    a += a + at;
    b += b + bt;
    G[a].push_back(b ^ 1);
    G[b].push_back(a ^ 1);
  }

  bool dfs(int pos) {
    if (vis[pos ^ 1]) return false;
    if (vis[pos]) return true;
    vis[pos] = 1;
    stk.push(pos);
    for (auto &v : G[pos]) {
      if (!dfs(v)) return false;
    }
    return true;
  }

  bool sol() {
    random_device rd;
    mt19937 seed(rd());
    vector<int> ord(n + 1);
    iota(ord.begin(), ord.end(), 0);
    shuffle(ord.begin() + 1, ord.end(), seed); // 随机选点 dfs
    for (int i = 1; i <= n; i++) {
      int it = ord[i];
      if (!vis[it * 2 + 1] && !vis[it * 2]) {
        while (!stk.empty()) stk.pop();
        int c = (it ^ i) & 1; // 随机选真假
        if (!dfs(it * 2 + c)) {
          while (!stk.empty()) {
            vis[stk.top()] = 0;
            stk.pop();
          }
          if (!dfs(it * 2 + (c ^ 1))) return false;
        }
      }
    }
    return true;
  }
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n, m; cin >> n >> m;

  TwoSAT T(n);
  for (int i = 1; i <= m; i++) {
    int u, x, v, y; cin >> u >> x >> v >> y;
    T.addor(u, x, v, y);
  }

  if (T.sol()) {
    cout << "POSSIBLE" << endl;
    for (int i = 1; i <= n; i++) {
      if (T.vis[i * 2]) cout << 0 << " ";
      else cout << 1 << " ";
    }
  } else {
    cout << "IMPOSSIBLE" << endl;
  }
   
  return 0;
}
```

# 2. BSGS

```cpp
//
//  main.cpp
//  2022-08-05 13:59
//
//  Created by liznb
//
#include <bits/stdc++.h>
using namespace std;

// 朴素 BSGS
// 1. 求解离散对数 a ^ x = b (mod p)
// 2. MOD 必须是质数
// 3. 复杂度 O(p ^ 0.5)
// 常见用法：
// 1. 求 x ^ a = b (mod p), p 是质数
//   a. 把 x 用原根表示 x = g ^ c
//   b. 原式变成 (g ^ a) ^ c = b (mod p), 求离散对数即可

struct BSGS {
  long long MOD, a, b;
  vector<long long> x;

  long long power(long long a, long long b) {
    assert(b >= 0);
    long long base = a, ans = 1ll;
    while (b) {
      if (b & 1) ans = 1ll * ans * base % MOD;
      base = 1ll * base * base % MOD;
      b >>= 1;
    }
    return ans;
  }

  BSGS(long long _MOD, long long _a, long long _b) : MOD(_MOD), a(_a), b(_b) {
    long long limit = ceil(sqrt(MOD));
    b %= MOD;
    map<long long, long long> hash;
    for (long long i = 0ll, val = b; i <= limit; i++) {
      hash[val] = i;
      (val *= a) %= MOD;
    }

    long long step = power(a, limit);

    for (long long i = 0ll, val = 1; i <= limit; i++) {
      if (hash.find(val) != hash.end()) {
        long long res = i * limit - hash[val];
        if (res >= 0) x.push_back(res);
      }
      (val *= step) %= MOD;
    }
    sort(x.begin(), x.end());
  }
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int p, a, b; cin >> p >> a >> b;
  BSGS T(p, a, b);

  if (T.x.size() == 0) cout << "no solution";
  else cout << T.x[0];

  return 0;
}

```

# 3. CDQ分治

```cpp
//
//  main.cpp
//  2022-07-14 00:21
//
//  Created by liznb
//  

#include <bits/stdc++.h>
using namespace std;

#define int long long

// CDQ分治
// 1. 需要先把问题转化成三维偏序问题。
// 2. 第一维排序，第二维双指针，第三维 线段树/树状数组
// 3. 注意下标！
//
// 例题: 二维最长不下降序列

struct BIT {
  vector<int> tree;
  int limit;
  BIT (int n) : tree(n + 1, 0), limit(n) {}

  inline int lowbit(int x) { return x & (-x); }

  void modify(int x, int val) { // 单点修改
    assert(x >= 1);
    while (x <= limit) {
      tree[x] = max(val, tree[x]);     
      x += lowbit(x);
    }
  }

  void init(int x) {
    assert(x >= 1);
    while (x <= limit) {
      tree[x] = 0;
      x += lowbit(x);
    }
  }

  int query(int x) {
    assert(x >= 0);
    int ans = 0;
    while (x) {
      ans = max(tree[x], ans);
      x -= lowbit(x);
    }
    return ans;
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
   
  int z; cin >> z; 
  while (z--) {
    int n; cin >> n;
    vector<array<int, 3>> a(n + 1); 
    vector<int> dis;
    for (int i = 1; i <= n; i++) {

      // 注意下标，CDQ分治排序后下标会乱, 一定需要保存下标。
      a[i][0] = i;

      cin >> a[i][1];
      dis.push_back(a[i][1]);
    }
    for (int i = 1; i <= n; i++) {
      cin >> a[i][2];
      dis.push_back(a[i][2]);
    }

    sort(dis.begin(), dis.end()); 
    dis.erase(unique(dis.begin(), dis.end()), dis.end());

    for (int i = 1; i <= n; i++) {
      a[i][1] = lower_bound(dis.begin(), dis.end(), a[i][1]) - dis.begin() + 1;
      a[i][2] = lower_bound(dis.begin(), dis.end(), a[i][2]) - dis.begin() + 1;
    }
    
    vector<int> dp(n + 1, 0);

    // 树状数组重复使用可以快不少。不重复使用也行。
    BIT T(dis.size());

    function<void(int, int)> CDQ = [&] (int l, int r) {
      if (l == r) {
        dp[l]++;
        return;
      }
      int mid = (l + r) >> 1;
      CDQ(l, mid);

      sort(a.begin() + l, a.begin() + mid + 1, [&](auto i, auto j) { return i[1] < j[1]; });
      sort(a.begin() + mid + 1, a.begin() + r + 1, [&](auto i, auto j) { return i[1] < j[1]; });

      int p = l;
      for (int i = mid + 1; i <= r; i++) {
        while (p <= mid && a[p][1] < a[i][1]) {

          // 注意下标！！！
          T.modify(a[p][2], dp[a[p][0]]);

          p++;
        }
        if (a[i][2] != 1) {
          dp[a[i][0]] = max(dp[a[i][0]], T.query(a[i][2] - 1));
        }
      }

      for (int i = l; i < p; i++) {
        T.init(a[i][2]);
      }

      sort(a.begin() + l, a.begin() + mid + 1);
      sort(a.begin() + mid + 1, a.begin() + r + 1);
      CDQ(mid + 1, r);
    };
    
    CDQ(1, n);

    int ans = 0;
    for (int i = 1; i <= n; i++) {
      ans = max(ans, dp[i]);
    }
    cout << ans << endl;
  }
  return 0;
}
```

# 4. KMP

```cpp
//
//  main.cpp
//  2022-08-29 11:25
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// KMP
// S: 文本串
// T: 模式串
// 在 S 中寻找子串 T
// 前缀函数是用在 T 上

// 返回前缀函数
vector<int> prefix_function(const string &s) {
  int n = (int) s.length();
  vector<int> pi(n, 0);
  for (int i = 1; i < n; i++) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) j++;
    pi[i] = j;
  }
  return pi;
}

// 返回 t 在 s 中出现的每个位置的结束位置。
// 例如: abcabc abc 返回 2 5
vector<int> kmp(const string &s, const string &t) {
  vector<int> pi = prefix_function(t), ans;
  int p = -1;
  for (int i = 0; i < (int) s.length(); i++) {
    while (t[p + 1] != s[i] && p != -1) p = pi[p] - 1;
    if (t[p + 1] == s[i]) p++;
    if (p + 1 == (int) t.length()) {
      ans.push_back(i);
      p = pi[p] - 1;
    }
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  string s, t; cin >> s >> t;
  auto ans = kmp(s, t);
  for (auto &it : ans) {
    cout << it - t.length() + 2 << endl;
  }
  auto res = prefix_function(t);
  for (auto &it : res) cout << it << " ";

  return 0;
}

```

# 5. LCT

```cpp
//
//  main.cpp
//  2022-08-19 17:51
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// Link-Cut Tree

template<class T>
struct LCT {
  struct Node {
    array<int, 2> son;
    int fa, lazy, size;
    T val, sum;
    Node() : son({0, 0}) { fa = lazy = 0; size = 1; }
  };

  vector<Node> tree;

  LCT(int n) {
    tree.resize(n + 1);
  }

  // 输出 splay 中序遍历, 调试用
  void output(int pos) {
    push_down(pos);
    if (tree[pos].son[0])
      output(tree[pos].son[0]);
    cout << pos << " ";
    if (tree[pos].son[1])
      output(tree[pos].son[1]);
  }

  void update(int pos) {
    int l = tree[pos].son[0];
    int r = tree[pos].son[1];
    tree[pos].size = (l ? tree[l].size : 0) + (r ? tree[r].size : 0) + 1;
    tree[pos].sum = (l ? tree[l].sum : 0) ^ (r ? tree[r].sum : 0) ^ tree[pos].val;
  }

  void push_down(int pos) {
    if (tree[pos].lazy) {
      int l = tree[pos].son[0], r = tree[pos].son[1];
      tree[l].lazy ^= 1;
      tree[r].lazy ^= 1;
      swap(tree[pos].son[0], tree[pos].son[1]);
      tree[pos].lazy = 0;
    }
  }

  bool is_root(int pos) {
    int fa = tree[pos].fa;
    return (tree[fa].son[0] != pos && tree[fa].son[1] != pos);
  }

  int get_o(int pos) {
    return pos == tree[tree[pos].fa].son[1];
  }

  void rotate(int pos) {
    int y = tree[pos].fa, z = tree[y].fa, o = get_o(pos);
    if (!is_root(y)) tree[z].son[y == tree[z].son[1]] = pos;

    tree[y].son[o] = tree[pos].son[o ^ 1];
    if (tree[pos].son[o ^ 1]) tree[tree[pos].son[o ^ 1]].fa = y;

    tree[pos].son[o ^ 1] = y;
    tree[y].fa = pos;
    tree[pos].fa = z;

    update(y), update(pos);
  }

  // 清理 splay 操作路径上的 lazy 标记
  void clear_tag(int pos) {
    if (!is_root(pos)) clear_tag(tree[pos].fa);
    push_down(pos);
  }

  void splay(int pos) {
    clear_tag(pos);
    while (!is_root(pos)) {
      int fa = tree[pos].fa;
      if (!is_root(fa))
        rotate(get_o(pos) == get_o(fa) ? fa : pos);
      rotate(pos);
    }
    update(pos);
  }

  // 把从 pos 到根的路径边成实边
  // 返回只由路径上的点组成的 Splay 的根
  int access(int pos) {
    int last = 0;
    while (pos) {
      splay(pos);
      tree[pos].son[1] = last;
      update(pos);
      last = pos;
      pos = tree[pos].fa;
    }
    return last;
  }

  // 设置 pos 为原树的根
  void make_root(int pos) {
    pos = access(pos);
    tree[pos].lazy ^= 1;
    push_down(pos);
  }

  // 查找 pos 所在的原树的树根
  int find(int pos) {
    access(pos);
    splay(pos);
    push_down(pos);
    while (tree[pos].son[0]) {
      pos = tree[pos].son[0];
      push_down(pos);
    }
    splay(pos);
    return pos;
  }

  // 在 u 和 v 之间建立一条(虚)边
  void link(int u, int v) {
    if (find(u) == find(v)) return;
    make_root(u);
    splay(u);
    tree[u].fa = v;
  }

  // 把 u 到 v 的路径边成实边
  // split 后 v 是 Splay 的根, u 是原树的根
  void split(int u, int v) {
    make_root(u), access(v), splay(v);
  }

  // 删除边 (u, v) , 不需要保证边存在。
  void cut(int u, int v) {
    split(u, v);
    if (tree[u].fa == v) {
      tree[v].son[0] = 0;
      tree[u].fa = 0;
    }
  }

  void set(int pos, int val) {
    splay(pos);
    tree[pos].val = val;
    update(pos);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m; cin >> n >> m;

  LCT<int> T(n);
  
  for (int i = 1; i <= n; i++) {
    int val; cin >> val;
    T.set(i, val);
  }

  for (int i = 1; i <= m; i++) {
    int o, x, y; cin >> o >> x >> y;
    if (o == 0) {
      T.split(x, y);
      cout << T.tree[y].sum << endl;
    } else if (o == 1) {
      T.link(x, y);
    } else if (o == 2) {
      T.cut(x, y);
    } else if (o == 3) {
      T.set(x, y);
    }
  }
  return 0;
}

```

# 6. MTT

```cpp
#include <bits/stdc++.h>
using namespace std;

// MTT (任意模数NTT)
// 注意:
// 1. 模数 998244353, 1004535809, 469762049 的原根是 3
// 2. 998244353 - 1 能整除最大 2 ^ 23
// 3. 1004535809 - 1 能整除最大 2 ^ 21
// 4. 469762049 - 1 能整除最大 2 ^ 26
// 5. 998244353 * 1004535809 * 469762049 大约 4e26
// 6. 答案必须小于 4e26, 即 n * A * A < 4e26
// 7. MOD, g, ig 定义成全局 const 会在编译阶段优化取模运算
// 8. Mint 必须用构造函数初始化，因为 a, b, c 必须是规范的。
//
// 关于速度:
// 1. 因为做了三次NTT并且最后涉及__int128, 所以速度不快。
// 2. n = 1e5 A = 1e9 时耗时 2s
//
// https://www.luogu.com.cn/problem/P4245

long long power(long long a, long long b, long long MOD) {
  a %= MOD;
  long long ans = 1;
  while (b) {
    if (b & 1) ans = ans * a % MOD;
    a = a * a % MOD;
    b >>= 1;
  }
  return ans;
}

const long long g = 3;
const long long MOD1 = 998244353, MOD2 = 1004535809, MOD3 = 469762049;
const __int128 ALL = (__int128) MOD1 * MOD2 * MOD3;
const long long ig1 = power(g, MOD1 - 2, MOD1), ig2 = power(g, MOD2 - 2, MOD2), ig3 = power(g, MOD3 - 2, MOD3);
const long long P1 = power(MOD2 * MOD3, MOD1 - 2, MOD1);
const long long P2 = power(MOD1 * MOD3, MOD2 - 2, MOD2);
const long long P3 = power(MOD1 * MOD2, MOD3 - 2, MOD3);

struct Mint {
  long long a, b, c;

  Mint() {}
  Mint(long long _a, long long _b, long long _c) {
    a = (_a % MOD1 + MOD1) % MOD1;
    b = (_b % MOD2 + MOD2) % MOD2;
    c = (_c % MOD3 + MOD3) % MOD3;
  }

  long long crt(long long p) {
    __int128 ans = 0;
    ans = (ans + (__int128) MOD2 * MOD3 * a % ALL * P1 % ALL) % ALL;
    ans = (ans + (__int128) MOD1 * MOD3 * b % ALL * P2 % ALL) % ALL;
    ans = (ans + (__int128) MOD1 * MOD2 * c % ALL * P3 % ALL) % ALL;
    return ans % p;
  }

  friend Mint operator + (const Mint x, const Mint y) {
    return {(x.a + y.a) % MOD1, (x.b + y.b) % MOD2, (x.c + y.c) % MOD3};
  }
  friend Mint operator - (const Mint x, const Mint y) {
    return {(x.a - y.a + MOD1) % MOD1, (x.b - y.b + MOD2) % MOD2, (x.c - y.c + MOD3) % MOD3};
  }
  friend Mint operator * (const Mint x, const Mint y) {
    return {x.a * y.a % MOD1, x.b * y.b % MOD2, x.c * y.c % MOD3};
  }
  Mint &operator += (const Mint y) {
    return (*this) = (*this) + y;
  }
  Mint &operator *= (const Mint y) {
    return (*this) = (*this) * y;
  }
};

struct Poly {
  vector<Mint> c;
  vector<int> rev;

  Poly(vector<Mint> &_c) : c(_c) {
    assert((int) c.size());
  }

  Mint power(Mint a, Mint b) {
    Mint ans = {1, 1, 1};
    while (b.a || b.b || b.c) {
      if (b.a && b.a & 1) ans.a = ans.a * a.a % MOD1;
      if (b.b && b.b & 1) ans.b = ans.b * a.b % MOD2;
      if (b.c && b.c & 1) ans.c = ans.c * a.c % MOD3;
      b.a >>= 1, b.b >>= 1, b.c >>= 1;
      a *= a;
    }
    return ans;
  }

  void dft(int limit, int o) {
    c.resize(limit, {0, 0, 0});
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

      Mint b = {(MOD1 - 1) / fa, (MOD2 - 1) / fa, (MOD3 - 1) / fa};
      Mint step = power(o == 1 ? (Mint) {g, g, g} : (Mint) {ig1, ig2, ig3}, b);

      for (int i = 0; i < limit; i += fa) {
        Mint w = {1, 1, 1};
        for (int j = i; j < i + son; j++) {
          Mint x = c[j], y = c[j + son] * w;
          c[j] = x + y;
          c[j + son] = x - y;
          w *= step;
        }
      }
    }
    if (o == -1) {
      Mint ilimit = power({limit, limit, limit}, {MOD1 - 2, MOD2 - 2, MOD3 - 2});
      for (int i = 0; i < limit; i++)
        c[i] = c[i] * ilimit;
    }
  }

  friend Poly operator * (Poly a, Poly b) {
    int limit = 1;
    int len = (int) a.c.size() + (int) b.c.size() - 1;
    while (limit < len) limit <<= 1;

    a.dft(limit, 1); b.dft(limit, 1);

    vector<Mint> res(limit);
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
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m, p; cin >> n >> m >> p;
  vector<Mint> a(n + 1), b(m + 1);

  for (auto &it : a) {
    long long val; cin >> val;
    it = {val, val, val};
  }
  for (auto &it : b) {
    long long val; cin >> val;
    it = {val, val, val};
  }

  Poly A(a), B(b);
  A *= B;

  for (int i = 0; i < (int) A.c.size(); i++) {
    cout << A.c[i].crt(p) << " ";
  }

  return 0;
}
```

# 7. SOSDP

```cpp
//
//  main.cpp
//  2022-05-18 16:59
//
//  Created by liznb
//  
//  https://codeforces.com/contest/165/problem/E


#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n; cin >> n;

  int limit = (1ll << 22) - 1;
  vector<int> a(limit + 1, -1);
  vector<int> b(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> b[i];
    a[b[i]] = b[i];
  }
  
  // SOSDP
  // 适用情况：
  // 1. 求子集的和/最大值/最小值/积
  // 2. 一般不能直接看出来用 SOSDP 需要转化
  //
  // 原理:
  // f[mask][j] 表示与 mask 前 j 位(从0位开始) 不同的子集(也可以完全相同)的答案统计 。
  // 两种转移:
  // 1. f[mask][j] <= f[mask][j - 1] 无论第 j 位是 0/1 第 j 位不变。
  // 2. f[mask][j] <= f[mask ^ (1 << j)][j - 1] (条件 mask >> j & 1) 让第 j 位变成 0.
  //
  // 理解：
  // SOSDP 也可以父集枚举
  // SOSDP 实质上是 FWT 或的正变换
  //
  // 下面是是简易写法
  // 注意: 
  // 1. 22 和 limit 一定要跑满。即使不是简易写法也要跑满。
  // 2. 一定从 0 开始
  // 3. 第二层循环顺序并不重要，这不是背包。只存在 0->1
  //
  for (int i = 0; i <= 22; i++) {
    for (int j = 0; j <= limit; j++) {
      // 这里 max 也可以是 min, +=, *= 之类的。
      a[j] = max((j >> i & 1) ? a[j ^ (1 << i)] : -1, a[j]);
    }
  }

  for (int i = 1; i <= n; i++) {
    int val = (~b[i]) & ((1ll << 22) - 1);
    assert(val < limit);
    cout << a[val] << " ";
  }
  
  return 0;
}

```

# 8. Splay

```cpp
#include <bits/stdc++.h>

using namespace std;

// Splay 伸展树
// 说明：
// 1. 能做常规平衡树能做的事情。
// 2. 如果要 合并平衡树/可持久化，请用 Treap
// 3. 速度没有 Treap 快。Treap 没 set 快。。
// 4. 可以自定义数据类型。
// 5. 空结点指向 0，如果要修改，请注意 val 的初始值。

template<class T>
struct Splay {
  struct Node {
    array<int, 2> son;
    int fa, size, cnt;
    T val;
    Node() : son({0, 0}) { fa = size = cnt = 0; }
    Node(T _val) : son({0, 0}), val(_val) { size = cnt = 1; fa = 0; }
  };

  vector<Node> tree;

  int root;

  Splay() {
    tree.emplace_back(), root = 0;
  }
  
  // 判断 pos 是它父亲的 左儿子 还是 右儿子
  int get_o(int pos) {
    return pos == tree[tree[pos].fa].son[1];
  }

  void insert(T val) {
    if (!root) {
      tree.emplace_back(val);
      root = (int) tree.size() - 1;
      update(root);
      return;
    }
    int pos = root, fa = 0;
    while (true) {
      if (tree[pos].val == val) {
        tree[pos].cnt++;
        update(pos), update(fa);
        splay(pos);
        break;
      }
      fa = pos;
      pos = tree[pos].son[tree[pos].val < val];
      if (!pos) {
        tree.emplace_back(val);
        tree.back().fa = fa;
        tree[fa].son[tree[fa].val < val] = (int) tree.size() - 1;
        update((int) tree.size() - 1), update(fa);
        splay((int) tree.size() - 1);
        break;
      }
    }
  }

  // 把 pos 设为废点，事实上即使不 clear，这个点也永远不会再访问。
  void clear(int pos) {
    tree[pos].son = {0, 0};
    tree[pos].fa = tree[pos].size = tree[pos].cnt = 0;
  }

  // 查询比 val 小的数的个数 + 1, val 可以不存在。
  int rank(T val) {
    if (!root) return 1;
    int res = 0, pos = root;
    while (true) {
      if (val < tree[pos].val) {
        if (tree[pos].son[0]) {
          pos = tree[pos].son[0];
        } else {
          splay(pos);
          return res + 1;
        }
      } else {
        res += tree[tree[pos].son[0]].size;
        if (val == tree[pos].val) {
          splay(pos);
          return res + 1;
        }
        res += tree[pos].cnt;
        if (tree[pos].son[1]) {
          pos = tree[pos].son[1];
        } else {
          splay(pos);
          return res + 1;
        }
      }
    }
  }

  // 查询第 k 小的数，如果 k 大于元素总数，返回最大的数
  T kth(int k) {
    assert(k >= 1);

    int pos = root;
    k = min(k, tree[root].size);

    while (true) {
      int l = tree[pos].son[0], r = tree[pos].son[1];
      if (l && k <= tree[l].size) {
        pos = l;
      } else {
        k -= tree[pos].cnt + tree[l].size;
        if (k <= 0) {
          splay(pos);
          return tree[pos].val;
        }
        pos = r;
      }
    }
  }

  // 返回比 k 小的最大的数。如果不存在 assert(0);
  T pre(T k) {
    insert(k);
    int pos = tree[root].son[0];
    if (!pos) assert(0);
    while (tree[pos].son[1]) pos = tree[pos].son[1];
    splay(pos);
    T res = tree[pos].val;
    erase(k);
    return res;
  }

  // 返回比 k 小的最大的数。如果不存在 assert(0);
  T nxt(T k) {
    insert(k);
    int pos = tree[root].son[1];
    if (!pos) assert(0);
    while (tree[pos].son[0]) pos = tree[pos].son[0];
    splay(pos);
    T res = tree[pos].val;
    erase(k);
    return res;
  }

  void erase(T k) {
    rank(k);
    if (tree[root].cnt > 1) {
      tree[root].cnt--;
      update(root);
      return;
    }
    int l = tree[root].son[0], r = tree[root].son[1];
    if (!l && !r) {
      clear(root);
      root = 0;
      return;
    } else if (!l) {
      int pos = root;
      root = r;
      tree[root].fa = 0;
      clear(pos);
      return;
    } else if (!r) {
      int pos = root;
      root = l;
      tree[root].fa = 0;
      clear(pos);
      return;
    } else {
      int pos = root;
      int x = l;
      while (tree[x].son[1]) x = tree[x].son[1];
      splay(x);

      tree[tree[pos].son[1]].fa = x;
      tree[x].son[1] = tree[pos].son[1];

      clear(pos);
      update(root);
    }
  }

  void update(int pos) {
    int l = tree[pos].son[0];
    int r = tree[pos].son[1];
    tree[pos].size = tree[pos].cnt + (l ? tree[l].size : 0) + (r ? tree[r].size : 0);
  }

  void rotate(int pos) {
    int y = tree[pos].fa, z = tree[y].fa, o = get_o(pos);
    assert(y);

    tree[y].son[o] = tree[pos].son[o ^ 1];
    if (tree[pos].son[o ^ 1]) tree[tree[pos].son[o ^ 1]].fa = y;

    tree[pos].son[o ^ 1] = y;
    tree[y].fa = pos;

    tree[pos].fa = z;
    if (z) tree[z].son[y == tree[z].son[1]] = pos;

    update(y), update(pos);
  }

  void splay(int pos) {
    while (tree[pos].fa) {
      int fa = tree[pos].fa;
      if (tree[fa].fa)
        rotate(get_o(pos) == get_o(fa) ? fa : pos);
      rotate(pos);
    }
    root = pos;
  }
};

int main() {
  int n, m; scanf("%d %d", &n, &m);
  Splay<int> T;
  
  for (int i = 1; i <= n; i++) {
    int val; scanf("%d", &val);
    T.insert(val);
  }
  int last = 0, ans = 0;
  for (int i = 1; i <= m; i++) {
    int opt, x; 
    scanf("%d %d", &opt, &x);
    x ^= last;
    if (opt == 1) {
      T.insert(x);
    }
    if (opt == 2) {
      T.erase(x);
    }
    if (opt == 3) {
      last = T.rank(x);
      ans ^= last;
    } 
    if (opt == 4) {
      last = T.kth(x);
      ans ^= last;
    } 
    if (opt == 5) {
      last = T.pre(x);
      ans ^= last;
    }
    if (opt == 6) {
      last = T.nxt(x);
      ans ^= last;
    }
  }
  printf("%d", ans);
  return 0;
}
```

# 9. Treap

```cpp
//
//  main.cpp
//  2022-05-06 22:18
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 无旋 Treap
// 特性说明:
// 1. kth 可以求出 Treap 中第 k 小的数 1-indexed
// 2. merge_treap 可以合并两个 treap，并且不需要保证它们之间的大小关系, 例题: CF911G。
//    但是效率成迷, 感性理解大概是大常数 O(logn),  实测:
//    a. 在洛谷快速排序中 merge 可以 AC, merge_treap T 3组。
//    b. Treap 板题merge 和 merge_treap 都可以AC。(因为数据水)
//    c. Treap 板题加强版中 merge_treap 用时是 merge 的两倍, 但还是 AC 了
// 3. erase 可以删除一个 key, 如果要删光 key, 把 res2[1] 赋值成 -1 即可
// 4. 这个 Treap 是模拟的 multiset
// 5. 没有 lower_bound 和 upper_bound, nxt() pre() count() 够用了，不要尝试实现 lower_bound,
//    返回 pos(下标) 对 Treap 没用，因为不能保证是根结点。
// 
// 注意:
// 1. 不是根结点的点是完全没用的，不能提供任何有用性质。所以一定要用 split 来得到根结点。
// 2. 不要随意在外面修改 size。修改后要想办法 update
// 3. 时刻留意是不是数组访问了-1
// 4. create 一定不要用 mt19937, 花费时间是函数随机的两倍!
// 5. nxt pre 可以在找不到的时候返回-1，kth 必须找得到。 
// 6. 判断子树是否为空不能用 size = 0 判断，子树为空的时候下标是 -1 !
// 
// 区间操作:
// 1. 区间操作修改 push_down
// 2. 使用 split_sz
// 
// 模板题: https://www.luogu.com.cn/problem/P6136

struct Treap {
  struct Node {
    int l, r; 
    int key, pri, size;
    int lazy;
    Node (int a, int b) : key(a), pri(b) { 
      l = r = -1, size = 1, lazy = 0; 
    }
  };

  vector<Node> tree; 
  vector<int> root; 
  int seed = 1;
  
  Treap(int n) : root(n + 1, -1) {} 

  void push_down(int pos) {
    
  }
  
  array<int, 2> split(int pos, int key) {
    if (pos == -1) return {-1, -1};
    push_down(pos);
    if (tree[pos].key <= key) {
      array<int, 2> res = split(tree[pos].r, key);
      tree[pos].r = res[0];
      update(pos);
      return {pos, res[1]};
    } else {
      array<int, 2> res = split(tree[pos].l, key);
      tree[pos].l = res[1]; 
      update(pos);
      return {res[0], pos};
    }
  }

  array<int, 2> split_sz(int pos, int sz) {
    if (pos == -1) return {-1, -1};
    push_down(pos);
    int lsize = tree[pos].l == -1 ? 1 : tree[tree[pos].l].size + 1;
    if (lsize <= sz) {
      array<int, 2> res = split_sz(tree[pos].r, sz - lsize);
      tree[pos].r = res[0];
      update(pos);
      return {pos, res[1]};
    } else {
      array<int, 2> res = split_sz(tree[pos].l, sz); 
      tree[pos].l = res[1];
      update(pos);
      return {res[0], pos};
    }
  }

  int merge(int x, int y) {
    if (x == -1) return y;
    if (y == -1) return x;
    if (tree[x].pri < tree[y].pri) {
      push_down(x);
      tree[x].r = merge(tree[x].r, y);  
      update(x);
      return x;
    } else {
      push_down(y);
      tree[y].l = merge(x, tree[y].l); 
      update(y);
      return y;
    }
  }

  int merge_treap(int x, int y) {
    if (x == -1) return y;
    if (y == -1) return x;
    push_down(x), push_down(y);
    if (tree[x].pri > tree[y].pri) swap(x, y);
    array<int, 2> res = split(y, tree[x].key);
    tree[x].l = merge_treap(tree[x].l, res[0]);
    tree[x].r = merge_treap(tree[x].r, res[1]);
    update(x);
    return x;
  }

  vector<int> output(int pos) {
    if (pos == -1) return {};
    push_down(pos);
    vector<int> res;
    vector<int> l = output(tree[pos].l), r = output(tree[pos].r);
    res.insert(res.end(), l.begin(), l.end());
    res.push_back(tree[pos].key);
    res.insert(res.end(), r.begin(), r.end());
    return res;
  }

  void update(int pos) {
    if (pos == -1) return;
    tree[pos].size = 1;
    if (tree[pos].l != -1) tree[pos].size += tree[tree[pos].l].size;
    if (tree[pos].r != -1) tree[pos].size += tree[tree[pos].r].size;
  }

  int func_rand() {
    return seed *= 19260817;
  }

  int create(int key) {
    tree.emplace_back(key, func_rand());
    return (int) tree.size() - 1;
  }
  
  void insert(int &pos, int key) {
    int o = create(key);
    array<int, 2> res = split(pos, key);
    pos = merge(merge(res[0], o), res[1]); 
  }

  int rank(int &pos, int key) {
    array<int, 2> res = split(pos, key - 1);
    int rk = (res[0] == -1) ? 1 : tree[res[0]].size + 1;
    pos = merge(res[0], res[1]);
    return rk;
  }

  int kth(int pos, int k) {
    while (pos != -1) {
      int lsize = tree[pos].l == -1 ? 1 : tree[tree[pos].l].size + 1;
      if (lsize == k) return tree[pos].key; 
      if (k < lsize) pos = tree[pos].l;
      if (k > lsize) pos = tree[pos].r, k -= lsize;
    }
    assert(0);
  }

  void erase(int &pos, int key) {
    array<int, 2> res1 = split(pos, key); 
    array<int, 2> res2 = split(res1[0], key - 1); 
    if (res2[1] != -1) {
      res2[1] = merge(tree[res2[1]].l, tree[res2[1]].r);
    }
    pos = merge(merge(res2[0], res2[1]), res1[1]);
  }

  int pre(int &pos, int key) {
    array<int, 2> res = split(pos, key - 1); 
    if (res[0] == -1) return -1;
    int ans = kth(res[0], tree[res[0]].size);
    pos = merge(res[0], res[1]);
    return ans;
  }

  int nxt(int &pos, int key) {
    array<int, 2> res = split(pos, key);
    if (res[1] == -1) return -1;
    int ans = kth(res[1], 1);
    pos = merge(res[0], res[1]);
    return ans;
  }

  int count(int &pos, int key) {
    array<int, 2> res1 = split(pos, key);
    array<int, 2> res2 = split(res1[0], key - 1);
    int ans = res2[1] == -1 ? 0 : tree[res2[1]].size;
    pos = merge(merge(res2[0], res2[1]), res1[1]);
    return ans;
  }
};

int main() {
  int n, m; scanf("%d %d", &n, &m);
  Treap T(1);
  
  for (int i = 1; i <= n; i++) {
    int val; scanf("%d", &val);
    T.insert(T.root[1], val);
  }
  int last = 0, ans = 0;
  for (int i = 1; i <= m; i++) {
    int opt, x; 
    scanf("%d %d", &opt, &x);
    x ^= last;
    if (opt == 1) {
      T.insert(T.root[1], x);
    }
    if (opt == 2) {
      T.erase(T.root[1], x);
    }
    if (opt == 3) {
      last = T.rank(T.root[1], x);
      ans ^= last;
    } 
    if (opt == 4) {
      last = T.kth(T.root[1], x);
      ans ^= last;
    } 
    if (opt == 5) {
      last = T.pre(T.root[1], x);
      ans ^= last;
    }
    if (opt == 6) {
      last = T.nxt(T.root[1], x);
      ans ^= last;
    }
  }
  printf("%d", ans);
  return 0;
}

```

# 10. 割点

```cpp
//
//  main.cpp
//  2022-07-25 20:11
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// Tarjan 求割点
// 1. 重边不影响
// 2. 核心思想：
//   a. 非根节点 u 是 G 的割顶当且仅当 u 存在一个子结点 v, 
//      使得 v 及其所有后代都没有反相边连回 u 的祖先(连回 u 不算)。
//   b. 根节点只在儿子个数大于等于 2 的时候是割点。
// 3. 一个例子: 两点一线图中两个点都不是割点。
// 4. g 中的点可以从1开始也可以从0开始编号，因为单独一个点不是割点。
// 4. 复杂度 O(n + m)

struct Tarjan {
 
  vector<int> dfn, low, iscut;  
  vector<vector<int>> g;
  int dfs_clock;

  Tarjan(int n, vector<vector<int>> &_g) : g(_g) {
    dfn.resize(n + 1, 0);
    low.resize(n + 1, 0);
    iscut.resize(n + 1, 0);
    dfs_clock = 0;
    for (int i = 0; i <= n; i++) {
      if (!dfn[i]) dfs(i, -1); 
    }
  }
  
  // 根节点的父亲必须是负数
  int dfs(int u, int fa) {
    int lowu = dfn[u] = ++dfs_clock;
    int child = 0; 
    for (auto &v : g[u]) {
      if (!dfn[v]) {
        child++;
        int lowv = dfs(v, u);
        lowu = min(lowu, lowv);
        if (lowv >= dfn[u]) {
          iscut[u] = true;
        }
      } else if (dfn[v] < dfn[u] && v != fa) {
        lowu = min(lowu, dfn[v]);
      }
    } 
    if (fa < 0 && child == 1) iscut[u] = 0;
    low[u] = lowu;
    return lowu;
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n, m; cin >> n >> m;
  vector<vector<int>> g(n + 1);
  for (int i = 1; i <= m; i++) {
    int u, v; cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u); 
  }

  Tarjan T(n, g);
  vector<int> ans;
  for (int i = 1; i <= n; i++) {
    if (T.iscut[i]) ans.push_back(i);
  }

  cout << ans.size() << endl;
  for (auto &it : ans) cout << it << " ";

  return 0;
}

```

# 11. 原根

```cpp
//
//  main.cpp
//  2022-07-07 01:02
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 原根
// 功能：
// 1. 求最小原根 O(n ^ 0.5) 跑不满
// 2. 求所有原根 O(nlogn) 跑满
// 注意:
// 1. 求最小原根，如果不存在，返回 -1
// 2. 求所有原根，如果不存在，返回空 vector
// 优化:
// 1. 如果 n 很小(1e6 左右):
//   a. 可以用线性筛预处理欧拉函数和原根是否存在
//   b. 线性筛预处理后可以 logn 枚举一个数的质因子/ 也可以直接埃筛
//   c. 求最小原根复杂度可以降到 O(n ^ 0.25 logn)


struct Primitive_Root {

  // 单次朴素求欧拉函数 O((n ^ 0.5) / logn)
  long long euler_phi(long long n) {
    long long ans = n;
    for (int i = 2; 1ll * i * i <= n; i++) {
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
      if (b & 1ll) ans = 1ll * ans * base % p;
      base = 1ll * base * base % p;
      b >>= 1ll;
    }
    return ans;
  }

  // 判断是否存在原根 O(m ^ 0.5)
  // 即 m 是否属于 2, 4, p^n, 2 * p^n
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

  // 求最小原根, 如果不存在原根，返回-1
  // 粗略估计复杂度 O(m ^ 0.5) , 但跑不满，实测很快。
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

  // 求所有原根
  // 复杂度上界 O(nlogn)
  vector<long long> all_root(long long m) {
    if (!exist(m)) return {};

    long long g = Minimum_root(m);
    vector<long long> root;
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

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  Primitive_Root T;

  const int MOD = 998244353;
  int g = T.Minimum_root(MOD);
  cout << g << endl;

  return 0;
}

```

# 12. 虚树

```cpp
//
//  main.cpp
//  2022-05-27 14:21
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;

// 虚树
//
// 注意: 有些虚树题也可以用回滚并查集+分治做。这里是标准虚树。
//
// 原理说明:
// 1. 把一棵树里的关键点构造成一个虚树。
// 2. 虚树里只有关键点和关键点的 lca
// 3. 找到所有关键点的 lca 的方法是对 dfs 序相邻的两个点找 lca，然后去重。
//
// 细节:
// 1. 虚树保存在 tree 里，注意，只有从 root (s) 能访问到的点才是虚树内的点。
// 2. 传入build 的数组需要 0 indexed
// 2. 做虚树题的时候记得及时清空数组，且不要 memset(模版内已经及时清空了)。
// 3. 这个模板只会建树，不会在建树的时候做额外操作。
//
// 使用流程:
// 1. Virtual_Tree T(n, root);
// 2. T.add(u, v); 自动双向
// 3. T.init();
// 4. T.build(key_point);

struct Virtual_Tree {
  vector<vector<int>> G, tree, p;
  vector<int> d, ord;
  int n, s, tot; 

  Virtual_Tree(int _n, int _s) : G(_n + 1), tree(_n + 1), d(_n + 1, 1), ord(_n + 1), n(_n), s(_s) {
    p.resize(_n + 1);
    int level = 0;
    while ((1 << level) <= _n) level++;
    for (int i = 0; i <= _n; i++) {
      p[i].resize(level + 1);
    }
  }

  int flag = false;

  void add(int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }
  
  void dfs(int pos, int fa) {
    ord[pos] = ++tot;
    p[pos][0] = fa;
    for (auto &v : G[pos]) {
      if (v == fa) continue;
      d[v] = d[pos] + 1;
      dfs(v, pos);
    }
  }

  void init() {
    flag = true; 
    tot = 0;
    dfs(s, s); 
    for (int level = 1; (1 << level) <= n; level++) {
      for (int i = 1; i <= n; i++) {
        p[i][level] = p[p[i][level - 1]][level - 1];
      }
    }
  }

  int lca(int a, int b) {
    assert(flag);
    if (d[a] < d[b]) swap(a, b); 
    int i, j;
    for (i = 0; (1 << i) <= d[a]; i++);  
    i--; 
    for (j = i; j >= 0; j--) {
      if (d[a] - (1 << j) >= d[b]) {
        a = p[a][j];
      }
    }
    if (a == b) return b; 
    for (j = i; j >= 0; j--) {
      if (p[a][j] != p[b][j]) {
        a = p[a][j];
        b = p[b][j];
      }
    }
    return p[a][0];
  }
  
  // key 0-indexed
  void build(vector<int> key) {
    sort(key.begin(), key.end(), [&](auto a, auto b) { return ord[a] < ord[b]; });
    vector<int> que;
    for (int i = 0; i < (int) key.size() - 1; i++) {
      que.push_back(lca(key[i], key[i + 1]));
    }
    que.push_back(s);
    key.insert(key.end(), que.begin(), que.end());
    sort(key.begin(), key.end()); 
    key.erase(unique(key.begin(), key.end()), key.end());
    sort(key.begin(), key.end(), [&](auto a, auto b) { return ord[a] < ord[b]; });
    assert(key[0] == s);

    for (auto &it : key) {
      tree[it].clear();
    }

    vector<int> stk; stk.push_back(s);
    for (int i = 1; i < (int) key.size(); i++) {
      while (lca(stk.back(), key[i]) != stk.back()) stk.pop_back();
      tree[stk.back()].push_back(key[i]);
      tree[key[i]].push_back(stk.back());
      stk.push_back(key[i]);
    }
  }
};

signed main() {
  // https://www.luogu.com.cn/problem/P2495
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
  int n; cin >> n; 
  Virtual_Tree T(n, 1);
  vector<vector<array<int, 2>>> G(n + 1);
  for (int i = 1; i <= n - 1; i++) {
    int u, v, w; cin >> u >> v >> w;
    G[u].push_back({w, v});
    G[v].push_back({w, u});
    T.add(u, v);
  }

  T.init();

  vector<int> dp(n + 1, 0), is(n + 1, 0), w(n + 1);

  function<void(int, int, int)> init = [&] (int pos, int fa, int minn) {
    w[pos] = minn;
    for (auto &[w, v] : G[pos]) {
      if (v == fa) continue;
      init(v, pos, min(minn, w)); 
    }
  };

  init(1, -1, 0x3f3f3f3f3f3f3f3f);

  int m; cin >> m;
  for (int i = 1; i <= m; i++) {
    int k; cin >> k; 
    vector<int> h(k);
    for (int j = 0; j < k; j++) {
      cin >> h[j];  
      is[h[j]] = 1;
    }
    T.build(h); 
    function<int(int, int)> dfs = [&] (int pos, int fa) {
      dp[pos] = 0;
      for (auto &v : T.tree[pos]) {
        if (v == fa) continue;
        int res = dfs(v, pos);
        if (is[v]) dp[pos] += w[v];
        else dp[pos] += min(w[v], res); 
      }
      return dp[pos];
    };
    dfs(1, -1);
    cout << dp[1] << endl;
    for (auto &it : h) {
      dp[it] = 0;
      is[it] = 0;
    }
  }

  return 0;
}
```

# 13. 直线(分数表示)

```cpp
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
// 加速手段：
// 1. 删掉 gcd
//
// 分数 Frac 功能说明:
// 0. (Frac) {a, b} 表示 b / a
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
  bool operator < (const Frac &oth) const {
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

```

# 14. 三分法

```cpp
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
```

# 15. 并查集

```cpp
//
//  main.cpp
//  2022-04-05 20:06
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

struct DSU {
  vector<int> f, dep, size;   
  DSU(int n) : f(n + 1), dep(n + 1, 0), size(n + 1, 1) { iota(f.begin(), f.end(), 0); }

  inline int find(int x) {
    while (x != f[x]) x = f[x] = f[f[x]];
    return x;
  }

  inline bool merge(int x, int y) {
    x = find(x), y = find(y); 

    if (x == y) return false;
    if (dep[x] > dep[y]) swap(x, y);
    if (dep[x] == dep[y]) dep[y]++;

    f[x] = y;    
    size[y] += size[x];
    return true;
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

```

# 16. 快速幂

```cpp
//
//  main.cpp
//  2022-07-08 20:30
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

long long power(long long a, long long b) {
  assert(b >= 0);
  long long base = a % MOD, ans = 1ll;
  while (b) {
    if (b & 1) ans = 1ll * ans * base % MOD;
    base = 1ll * base * base % MOD;
    b >>= 1;
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  cout << power(3, MOD - 2);

  return 0;
}

```

# 17. 格雷码

```cpp
//
//  main.cpp
//  2022-04-11 15:08
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

// 格雷码 Gray Code
// 功能说明：
// 1. 构造 K 位格雷码(一个 1<<k 长的序列, 相邻两个数二进制相差一位) 
// 特性：
// 1. k >=0 

struct Gray {
  vector<int> a; 
  Gray(int k) {
    a.resize(1ll << k, 0);
    for (int i = 1; i < (1ll << k); i++) {
      if (i & 1) a[i] = a[i - 1] ^ 1;
      else a[i] = a[i - 1] ^ (1 << (__builtin_ctzll(a[i - 1]) + 1));
    }
  }
};

signed main() {
  //file();
  ios::sync_with_stdio(false); 
  cin.tie(0);
    
  return 0;
}
```

# 18. 线性基

```cpp
//
//  线性基.cpp
//  2022-04-10 21:53
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

// Basis Vector
// 功能：
// 1. 求一组数能异或得到的最小值/最大值，kth小值。  
// 2. 判断一个数能不能被一组数异或得到
// 特性：
// 1. k 小值中，最小值是 0

struct Basis {
  vector<int> a; 
  void insert(int x) {
    for (auto &it : a) x = min(x, x ^ it);
    if (!x) return;
    for (auto &it : a) it = min(it, it ^ x);
    a.push_back(x);
    sort(a.begin(), a.end());
  }

  bool can(int x) {
    for (auto &it : a) x = min(x, x ^ it); 
    return !x;
  }

  int maxn(int x = 0) {
    for (auto &it : a) x = max(x, x ^ it);
    return x;
  }

  int kth(int k) {
    if (k > (1ll << (int) a.size())) return -1; 
    k--; int ans = 0;
    for (int i = 0; i < (int) a.size(); i++) {
      if (k >> i & 1) ans ^= a[i];
    }
    return ans;
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
   
   
  return 0;
}
```

# 19. 线段树

```cpp
//
//  main.cpp
//  2022-05-09 17:00
//
//  Created by liznb
//  
#include <bits/stdc++.h>

using namespace std;

// 普通线段树
// 1. 同时支持区间和，区间最大值，区间最小值, 区间覆盖。
// 2. 提速方法：
//    a. 删掉不需要维护的东西。
//    b. 不要用 modify 初始化，用 build
//    c. 如果只用 sum 和 add, 请使用树状数组。
//    d. 关闭 define int long long
//    e. 使用区间覆盖来整体初始化
//
// 3. 使用步骤:
//    a. 初始化传入点数, 默认 1 - n 可维护
//    b. query(l, r).maxn or sum or minn
//    c. modify(l, r, val); 区间加
//    d. cover(l, r, val) 区间覆盖

struct Segment_Tree {
  struct Info {
    long long sum, maxn, minn; 
  };

  struct Tree {
    int l, r; 
    Info val;
  };  

  vector<Tree> tree;
  vector<long long> lazy;
  vector<pair<bool, long long>> tag;

  Segment_Tree(int n) {
    tree.resize(n * 4 + 10);
    lazy.resize(n * 4 + 10);
    tag.resize(n * 4 + 10);
    vector<int> arr(n + 1, 0);
    build(1, n, arr);
  }

  Segment_Tree(int n, vector<int> &arr) {
    tree.resize(n * 4 + 10);
    lazy.resize(n * 4 + 10);
    tag.resize(n * 4 + 10);
    build(1, n, arr);
  }

  Info merge(Info a, Info b) {
    Info res;
    res.sum = a.sum + b.sum;
    res.maxn = max(a.maxn, b.maxn);
    res.minn = min(a.minn, b.minn);
    return res;
  }

  void build(int pos, int l, int r, vector<int> &arr) {
    assert(l <= r);
    tree[pos].l = l; tree[pos].r = r;
    lazy[pos] = 0ll;
    tag[pos] = make_pair(false, 0);

    int m = l + (r - l) / 2;
    if (l == r) {
      tree[pos].val.sum = arr[l]; 
      tree[pos].val.maxn = arr[l];
      tree[pos].val.minn = arr[l];
      return;
    }
    build(pos << 1, l, m, arr);
    build(pos << 1 | 1, m + 1, r, arr);
    tree[pos].val = merge(tree[pos << 1].val, tree[pos << 1 | 1].val);
  }


  // 对树上一个位置进行修改, o = 1 表示普通修改，o = 0 表示覆盖 
  void add(int pos, long long val, int o) {
    if (o) {
      lazy[pos] += val;
      tree[pos].val.sum += (tree[pos].r - tree[pos].l + 1) * val;
      tree[pos].val.maxn += val;
      tree[pos].val.minn += val;
    } else {
      lazy[pos] = 0;
      tag[pos] = make_pair(true, val);
      tree[pos].val.sum = (tree[pos].r - tree[pos].l + 1) * val;
      tree[pos].val.maxn = val;
      tree[pos].val.minn = val;
    }
  }

  // 传递lazy 和 tag
  void push_down(int pos) {
    if (lazy[pos]) {
      if (tag[pos].first) {
        add(pos << 1, tag[pos].second, 0);
        add(pos << 1 | 1, tag[pos].second, 0);
        tag[pos].first = false;
      }
      add(pos << 1, lazy[pos], 1); 
      add(pos << 1 | 1, lazy[pos], 1);
      lazy[pos] = 0;
    }
    if (tag[pos].first) {
      add(pos << 1, tag[pos].second, 0);
      add(pos << 1 | 1, tag[pos].second, 0);
      tag[pos].first = false;
    }
  } 

  void modify(int pos, int l, int r, long long val) {
    assert(l <= r);
    if (tree[pos].l >= l && tree[pos].r <= r) {
      add(pos, val, 1);
      return;
    }  
    push_down(pos);
    int m = tree[pos].l + (tree[pos].r - tree[pos].l) / 2;
    if (l <= m) modify(pos << 1, l, r, val);
    if (r > m) modify(pos << 1 | 1, l, r, val);
    tree[pos].val = merge(tree[pos << 1].val, tree[pos << 1 | 1].val);
  } 

  void cover(int pos, int l, int r, long long val) {
    assert(l <= r);
    if (tree[pos].l >= l && tree[pos].r <= r) {
      add(pos, val, 0);
      return;
    }  
    push_down(pos);
    int m = tree[pos].l + (tree[pos].r - tree[pos].l) / 2;
    if (l <= m) cover(pos << 1, l, r, val);
    if (r > m) cover(pos << 1 | 1, l, r, val);
    tree[pos].val = merge(tree[pos << 1].val, tree[pos << 1 | 1].val);
  } 
  
  Info query(int pos, int l, int r) {
    assert(l <= r);
    if (tree[pos].l >= l && tree[pos].r <= r) {
      return tree[pos].val;
    }   
    push_down(pos);
    int m = tree[pos].l + (tree[pos].r - tree[pos].l) / 2;
    Info res;

    if (l <= m && r > m) {
      res = merge(query(pos << 1, l, r), query(pos << 1 | 1, l , r));
    } else if (l > m) {
      res = query(pos << 1 | 1, l, r);
    } else if (r <= m) {
      res = query(pos << 1, l, r);
    }

    tree[pos].val = merge(tree[pos << 1].val, tree[pos << 1 | 1].val);
    return res;
  }

  void build(int l, int r, vector<int> &arr) { build(1, l, r, arr); }
  void modify(int l, int r, long long val) { modify(1, l, r, val); }
  void cover(int l, int r, long long val) { cover(1, l, r, val); }
  Info query(int l, int r) { return query(1, l, r); };
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
   
  int n, q; cin >> n >> q;   

  vector<int> arr(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> arr[i];
  }

  Segment_Tree T(n, arr);

  for (int i = 1; i <= q; i++) {
    int o; cin >> o;
    if (o == 1) {
      int l, r, x; cin >> l >> r >> x;
      T.modify(l, r, x);
    } else {
      int l, r; cin >> l >> r;
      cout << T.query(l, r).sum << '\n';
    }
  }

  return 0;
}
```

# 20. 组合数

```cpp
//
//  组合数.cpp
//  2022-03-26 00:52
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 组合数
// 1. 务必 #define int long long
// 2. 同时适用于小模数和大模数, Lucas 的上位替代
// 3. 初始化复杂度 O(n)
// 4. 模数必须是质数
//
// 卡常优化：
// 1. 如果题目 MOD 固定，把 MOD 定义成全局 const, 速度可以提升数倍。

#define int long long
struct Combination {

  vector<int> fac, cnt;
  int MOD;

  Combination(int n, int p) : fac(n + 1), cnt(n + 1), MOD(p) {
    fac[0] = fac[1] = 1; cnt[0] = cnt[1] = 0;
    for (int i = 2; i <= n; i++) {
      fac[i] = fac[i - 1] * i;
      cnt[i] = cnt[i - 1];
      while (fac[i] % MOD == 0) {
        cnt[i]++;
        fac[i] /= MOD;
      }
      fac[i] %= MOD;
    }
  } 

  int power(int a, int b) {
    assert(b >= 0);
    int base = a % MOD, ans = 1ll;
    while (b) {
      if (b & 1) ans = ans * base % MOD;
      base = base * base % MOD;
      b >>= 1;
    }
    return ans;
  }

  int inv(int x) {
    return power(x, MOD - 2);
  }

  int C(int a, int b) { // C_a^b
    if (b > a) return 0;
    if (cnt[a] != cnt[b] + cnt[a - b]) return 0;
    return fac[a] * inv(fac[b] * fac[a - b] % MOD) % MOD;
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
  return 0;
}

```

# 21. 最短路(SPFA)

```cpp
//
//  main.cpp
//  2022-04-14 00:44
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;

// 特性说明：
// 1. 放在循环里，一次性使用。
// 2. 自环会忽略。重边不影响。
// 2. SLF + swap 优化，如果队尾小于队首，就 swap 它们。
// 3. 没有负环返回 1，有负环返回0
// 4. 注意 INF 只能最多允许两个加数不爆 long long
// 5. 必须 define int long long

struct SPFA {
  const int INF = 0x3f3f3f3f3f3f3f3f;
  vector<vector<array<int, 2>>> G;
  vector<int> d, inq, cnt;

  SPFA(int n) : G(n + 1), d(n + 1, INF), inq(n + 1, 0), cnt(n + 1, 0) {}
  
  void add(int u, int v, int w) {
    G[u].push_back({w, v});
  }

  int BFS(int n, int s) {
    deque<int> que; que.push_back(s);
    inq[s] = 1; d[s] = 0;
    while (!que.empty()) {
      int u = que.front(); que.pop_front();
      inq[u] = false;
      for (auto &[w, v] : G[u]) {
        if (v != u && d[v] > d[u] + w) {
          d[v] = d[u] + w;
          if (!inq[v]) {
            que.push_back(v);
            if (que.front() > que.back()) {
              swap(que.front(), que.back());
            }
            inq[v] = 1;
            if (++cnt[v] > n) return 0;
          }
        }
      }
    }
    return 1;
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n, m, s; cin >> n >> m >> s; 

  SPFA T(n);
  for (int i = 1; i <= m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    T.add(u, v, w);
  }
  T.BFS(n, s);
  for (int i = 1; i <= n; i++) {
    cout << T.d[i] << " ";
  }
  return 0;
}

```

# 22. 最短路(priority_queue)

```cpp
#include <bits/stdc++.h>
using namespace std;

// Dijkstra  K 短路 + K 短路计数 (严格)
// 能处理 0 - n , 重边和自环不影响
// 注意根据数据范围替换 INF MOD
// 复杂度大概是 O(K(M + N)logN)

const int INF = 0x3f3f3f3f;
const int MOD = 100003;

struct Dijkstra {
  struct Edge {
    int w, v;

    Edge(int a, int b) : w(a), v(b) {}
    bool operator < (const Edge &oth) const {
      if (w != oth.w) return w < oth.w;
      return v < oth.v;
    }
    bool operator > (const Edge &oth) const {
      if (w != oth.w) return w > oth.w;
      return v > oth.v;
    }
  };
  vector<vector<Edge>> G; 
  vector<vector<int>> d, vis, cnt;
  int K; // 求 1-K 短路
  
  Dijkstra(int n, int k) : K(k) {
    G.resize(n + 1); 
    d.resize(n + 1, vector<int> (k, INF)); 
    vis.resize(n + 1, vector<int> (k, 0)); 
    cnt.resize(n + 1, vector<int> (k, 0));
  }

  void add(int u, int v, int w) {  // 有向边
    G[u].push_back({w, v}); // (距离, 序号) w 在前!
  }
  
  void solve(int s) {
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> que;           
    // 距离, 序号, 第K短
    que.push({0, s, 0});
    d[s][0] = 0; 
    cnt[s][0] = 1;

    while (!que.empty()) {
      auto x = que.top(); que.pop();
      int xw = x[0], xv = x[1], xo = x[2]; 
      
      if (vis[xv][xo]) continue;
      vis[xv][xo] = 1;

      for (auto &it : G[xv]) {
        int dis = it.w + xw;
        int xcnt = cnt[xv][xo];

        for (int i = 0; i < K; i++) {
          if (d[it.v][i] > dis) {
            swap(d[it.v][i], dis);
            swap(cnt[it.v][i], xcnt);
            que.push({d[it.v][i], it.v, i});
          } else if (d[it.v][i] == dis) {
            (cnt[it.v][i] += xcnt) %= MOD;
            break;
          }
        }
      }
    }
  }
};
int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  return 0;
}

```

# 23. 最短路(set)

```cpp
//
//  main.cpp
//  2022-03-26 00:49
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// Dijkstra 最短路，最短路计数
// 能处理 0 - n , 重边和自环不影响
// 注意根据数据范围替换 INF MOD
// 直接在 main 里面声明

const int INF = 0x3f3f3f3f;
const int MOD = 1e9 + 7;

struct Dijkstra {
  struct Edge {
    int w, v;

    Edge(int a, int b) : w(a), v(b) {}
    bool operator < (const Edge &oth) const {
      if (w != oth.w) return w < oth.w;
      return v < oth.v;
    }
  };
  
  vector<vector<Edge>> G; 
  vector<int> d, cnt;
  
  Dijkstra(int n) : G(n + 1), d(n + 1, INF), cnt(n + 1, 0) {}

  void add(int u, int v, int w) {  // 有向边
    G[u].push_back({w, v}); // (距离, 序号) w 在前!
  }
  
  void solve(int s) {
    d[s] = 0; cnt[s] = 1;
    set<Edge> que; // (距离, 序号) 

    for (int i = 0; i < (int)d.size(); i++) {
      que.insert({d[i], i}); // (距离, 序号) 
    }
    
    while (que.empty() == false) {
      int x = que.begin()->v;
      que.erase(que.begin());

      for (auto &it : G[x]) {
        int v = it.v, w = it.w;

        if (d[v] > d[x] + w) {
          que.erase({d[v], v}); // (距离, 序号) 
          d[v] = d[x] + w;
          que.insert({d[v], v});
          cnt[v] = cnt[x];
        } else if (d[v] == d[x] + w) {
          (cnt[v] += cnt[x]) %= MOD;
        }
      }
    }
  }
};


int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

```

# 24. 普通莫队

```cpp
#include<bits/stdc++.h>
using namespace std;

// 普通莫队
// From YouKn0wWho

const int N = 1e6 + 9, B = 440;

struct query {
  int l, r, id;
  bool operator < (const query &x) const {
    if(l / B == x.l / B) return ((l / B) & 1) ? r > x.r : r < x.r;
    return l / B < x.l / B;
  }
} Q[N];
int cnt[N], a[N];
long long sum;
inline void add_left(int i) {
  int x = a[i];
  sum += 1LL * (cnt[x] + cnt[x] + 1) * x;
  ++cnt[x];
}
inline void add_right(int i) {
  int x = a[i];
  sum += 1LL * (cnt[x] + cnt[x] + 1) * x;
  ++cnt[x];
}
inline void rem_left(int i) {
  int x = a[i];
  sum -= 1LL * (cnt[x] + cnt[x] - 1) * x;
  --cnt[x];
}
inline void rem_right(int i) {
  int x = a[i];
  sum -= 1LL * (cnt[x] + cnt[x] - 1) * x;
  --cnt[x];
}
long long ans[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n, q;
  cin >> n >> q;
  for(int i = 1; i <= n; i++) cin >> a[i];
  for(int i = 1; i <= q; i++) {
    cin >> Q[i].l >> Q[i].r;
    Q[i].id = i;
  }
  sort(Q + 1, Q + q + 1);
  int l = 1, r = 0;
  for(int i = 1; i <= q; i++) {
    int L = Q[i].l, R = Q[i].r;
    if(R < l) {
      while (l > L) add_left(--l);
      while (l < L) rem_left(l++);
      while (r < R) add_right(++r);
      while (r > R) rem_right(r--);
    } else {
      while (r < R) add_right(++r);
      while (r > R) rem_right(r--);
      while (l > L) add_left(--l);
      while (l < L) rem_left(l++);
    }
    ans[Q[i].id] = sum;
  }
  for(int i = 1; i <= q; i++) cout << ans[i] << '\n';
  return 0;
}
```

# 25. 树状数组

```cpp
//
//  main.cpp
//  2022-04-06 17:22
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
#define LOCAL
using namespace std;

// 二元索引树, Binary Indexed Tree, Fenwick树, 树状数组
// 特性:
// 1. 只能处理具有前缀和性质数组。
// 2. 下标绝对不能为 0
struct BIT {
  vector<int> tree;
  int limit;
  BIT (int n) : tree(n + 1, 0), limit(n) {}

  inline int lowbit(int x) { return x & (-x); }

  void modify(int x, int val) { // 单点修改
    assert(x >= 1);
    while (x <= limit) {
      tree[x] += val;     
      x += lowbit(x);
    }
  }

  int query(int x) {
    assert(x >= 0);
    int ans = 0;
    while (x) {
      ans += tree[x];
      x -= lowbit(x);
    }
    return ans;
  }

  int sum(int l, int r) {
    if (r < l) return 0;
    assert(l >= 1 && r >= 1);
    return query(r) - query(l - 1); 
  }
};

signed main() {
  //file();
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
   
  return 0;
}
```

# 26. 模拟退火

```cpp
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
```

# 27. 欧拉函数

```cpp
//
//  main.cpp
//  2022-07-07 19:38
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 1. 筛法求欧拉函数 O(n)
struct Euler {
  vector<int> phi, is, prinum;
  
  Euler(int n) : phi(n + 1), is(n + 1, 0) {
    prinum.reserve(n + 1); 
    phi[1] = 1;
    for (int i = 2; i <= n; i++) {
      if (!is[i]) prinum.push_back(i), phi[i] = i - 1;
      for (auto &it : prinum) {
        if (it * i > n) break;
        is[i * it] = 1;
        if (i % it == 0) {
          phi[i * it] = phi[i] * it;
          break;
        } else {
          phi[i * it] = phi[i] * (it - 1);
        }
      }
    }
  }
};

// 2. 单点欧拉函数 O(sqrt(n)) (朴素)
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

// 3. 单点欧拉函数 O(n ^ 0.25) (PR 优化)
namespace NT {
  long long gcd(long long a, long long b) {
    return b ? gcd(b, a % b) : a;
  }
  long long mul(long long a, long long b, long long m) {
    long long s = a * b - (long long)((long double)a * b / m + 0.5) * m;
    return s < 0 ? s + m : s;
  }
  long long fpow(long long x, long long a, long long m) {
    long long ans = 1;
    while(a) {
      if(a & 1) ans = mul(ans, x, m);
      x = mul(x, x, m), a >>= 1;
    }
    return ans;
  }
}

namespace Miller_Rabin {
  using namespace NT;
  long long p[15] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  int detect(long n, long long a) {
    if(n == a) return 1;
    if(a % n == 0) return 1;
    long long now = n - 1, lst = 1;
    if(fpow(a, now, n) != 1) 
      return 0;
    while(!(now & 1)) {
      now /= 2;
      long long p = fpow(a, now, n);
      if(lst == 1 && p != 1 && p != n - 1)
        return 0;
      lst = p;
    }
    return 1;
  }
  
  long long MR(long long n) {
    if(n < 2) return 0;
    for(int i = 0; i < 7; ++i) {
      if(!detect(n, p[i])) 
        return 0;
    }
    return 1;
  }
}

namespace Pollard_Rho {
  using namespace NT;
  using namespace Miller_Rabin;
  long long f(long long x, long long C, long long p) {
    return (mul(x, x, p) + C) % p;
  }
  long long Rand() {return (rand() << 15) + rand();}
  long long Randll() {return (Rand() << 31) + Rand();}
  
  long long PR(long long n) {
    if(n == 4) return 2;
    if(MR(n)) return n;
    while(1) {
      long long C = Randll() % (n - 1) + 1;
      long long s = 0, t = 0;
      long long acc = 1;
      do {
        for(int i = 1; i <= 128; ++i) {
          t = f(f(t, C, n), C, n);
          s = f(s, C, n);
          long long tmp = mul(acc, abs(t - s), n);
          if(s == t || tmp == 0)
            break;
          acc = tmp;
        }
        long long d = gcd(n, acc);
        if(d > 1) return d;
      } while(s != t);
    }
  }
  
  // 返回的 first 是质因子, second 是质因子个数, 从小到大排序。
  typedef pair<long long, int > pii;
  vector<pii> DCOMP(long long n) {
    vector<pii> ret;
    while(n != 1) {
      long long p = PR(n);
      while(!MR(p)) 
        p = PR(p);
      int c = 0;
      while(n % p == 0) n /= p, ++c;
      ret.push_back(make_pair(p, c));
    }
    sort(ret.begin(), ret.end());
    return ret;
  }
}

long long euler_phi_pr(long long n) {
  auto prime = Pollard_Rho::DCOMP(n);
  long long ans = n;
  for (auto &it : prime) {
    ans = ans / it.first * (it.first - 1);  
  }
  return ans;
}


int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

```

# 28. 珂朵莉树

```cpp
//
//  main.cpp
//  2022-06-08 11:28
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 珂朵莉树
// 1. 记住 split 返回包含x的右半区间
// 2. split 的 x 只能在初始化的范围内

struct ODT {
  struct Node {
    int l, r;
    mutable int val;
    Node(const int &a, const int &b, const int &c) : l(a), r(b), val(c) {}
    bool operator < (const Node & oth) const { return l < oth.l; }
  };

  set<Node> odt;

  ODT(int l, int r, int val) {
    odt.insert({l, r, val}); 
  }

  // 把包含 x 的区间 split 成[l, x) [x, r], 返回 [x, r]
  auto split(int x) { 
    auto it = --odt.upper_bound((Node) {x, 0, 0});
    if (it->l == x) return it;
    int l = it->l, r = it -> r, val = it->val;
    odt.erase(it);
    odt.insert((Node) {l, x - 1, val});
    return odt.insert((Node) {x, r, val}).first;
  }

  // split 必须先右后左, 因为 split 右端点可能会让已经 split 的左端点失效
  void assign(int l, int r, int val) {
    auto itr = split(r + 1);  
    auto itl = split(l);
    odt.erase(itl, itr);
    odt.insert((Node) {l, r, val});
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
  return 0;
}

```

# 29. 树状数组(区间操作)

```cpp
//
//  main.cpp
//  2022-07-16 02:37
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 树状数组
// 1. 支持 区间add，区间查询 sum
// 2. 不要用 modify 和 query (避免误用)
// 3. 用 add 函数和 get_sum 函数 修改和查询
// 4. 下标必须从1开始
// 5. 复杂度 O(nlogn)
// 6. 常数很小，比线段树快6倍！

struct BIT {
  vector<array<long long, 2>> tree; 
  int limit;

  BIT(int n) : tree(n + 1, {0ll, 0ll}), limit(n) {}

  inline int lowbit(int x) {
    return x & (-x);
  }

  void modify(int x, long long v1) {
    assert(x >= 1);
    long long v2 = v1 * x;
    while (x <= limit) {
      tree[x][0] += v1;
      tree[x][1] += v2;
      x += lowbit(x);
    }
  }

  long long query(int x) {
    assert(x >= 0);
    long long ans = 0ll;
    int backup = x;
    while (x) {
      ans -= tree[x][1];
      ans += 1ll * (backup + 1) * tree[x][0];
      x -= lowbit(x);
    }
    return ans;
  }

  void add(int l, int r, long long val) {
    assert(l >= 1 && r <= limit);
    modify(l, val);
    modify(r + 1, -val);
  }

  long long get_sum(int l, int r) {
    assert(l >= 1 && r <= limit);
    return query(r) - query(l - 1);
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

```

# 30. 合并线段树

```cpp
//
//  main.cpp
//  2022-05-21 16:43
//
//  Created by liznb
//  

#include <bits/stdc++.h>
//#define int long long
#define endl '\n'
#define LOCAL
using namespace std;

// 合并线段树
// 特性说明：
// 1. 在 vector<Tree> tree; 中初始有 n 个权值线段树(动态开点), 用于合并
// 2. 使用 merge_tree , 合并两个线段树(可以是子树), 需要保证传入的两个下标对应的 l, r 相同
// 3. 使用 merge_tree_lossless 后，原有的两个线段树依然可以正常访问。
// 4. 访问初始的 n 个root，一定！要用 root[i], 因为 root[i] 会因为 merge_tree 变动，类比 Treap
// 5. root[pos] = merge_tree(root[pos], root[v]); 才是 merge_tree 的正确用法，一定要让 merge_tree 的返回值被用到！
//
// 针对不同的题需要重写模板的一部分，修改顺序如下:
// 1. Info 结构体(注意初始值)
// 2. merge 函数
// 3. add 函数
// 4. merge_tree 函数
// 5. 检查 query, push_up 里的初始值是否赋值正确
// 6. 一般来说 modify 函数绝对不会被修改 ！

const int INF = 0x3f3f3f3f;
struct Segment_Tree {

  struct Info {
    int maxn; 
    long long sum;
    Info () { sum = 0, maxn = -INF; } // 注意初始值
  };

  struct Tree {
    int ls, rs; // 左儿子，右儿子 编号
    int l, r; // 左边界，右边界
    Info dat;
    Tree(int a, int b, int c, int d) : ls(a), rs(b), l(c), r(d) {} 
  };

  vector<Tree> tree; 
  vector<int> root;

  // 初始化 n 棵线段树 1-indexed
  Segment_Tree (int n, int l, int r) {
    root.resize(n + 1);
    iota(root.begin(), root.end(), 0);
    tree.resize(n + 1, {-1, -1, l, r});  
  }

  int insert(int l, int r) {
    tree.push_back({-1, -1, l, r});
    return (int) tree.size() - 1;
  }

  // 无损合并：合并后原来的两个线段树完整保留, 但是要耗费很多空间(和重合点数正相关)。
  // 一般只有在强制在线题目用这个。
  //
  // 注意: 合并后不要进行修改，否则会破坏原来的线段树，变成有损，需要提前modify好。
  // 
  // n = 1e5, 256mb 大概率在不优化的时候空间会爆
  // 优化手段：(建议都加上)
  // 1. 关掉 #define int long long, 只把sum之类的加上 long long
  // 2. 强优化: tree.reserve(n * 50) (n = 1e5 256mb 刚好)
  // 3. 不要初始化成 (-1e9, 1e9)

  int merge_tree_lossless(int a, int b) {
    if (a == -1) return b;       
    if (b == -1) return a;
    int p = insert(tree[a].l, tree[a].r);
    // merge_tree 只会在叶子结点合并, 注意这里的合并不能使用两个儿子的 merge 函数! 要单独写
    if (tree[a].l == tree[a].r) {
      // 需要重写!!!
      tree[p].dat.maxn = tree[a].dat.maxn + tree[b].dat.maxn;
      tree[p].dat.sum = tree[a].l;
      return p;
    }
    tree[p].ls = merge_tree_lossless(tree[a].ls, tree[b].ls);
    tree[p].rs = merge_tree_lossless(tree[a].rs, tree[b].rs);
    push_up(p); 
    return p;
  }

  // 有损合并：合并后原来的两个线段树作废，耗费空间少一些。
  int merge_tree(int a, int b) {
    if (a == -1) return b;       
    if (b == -1) return a;
    // merge_tree 只会在叶子结点合并, 注意这里的合并不能使用两个儿子的 merge 函数! 要单独写
    if (tree[a].l == tree[a].r) {
      // 需要重写!!!
      tree[a].dat.maxn += tree[b].dat.maxn; 
      tree[a].dat.sum = tree[a].l;
      return a;
    }
    tree[a].ls = merge_tree(tree[a].ls, tree[b].ls);
    tree[a].rs = merge_tree(tree[a].rs, tree[b].rs);
    push_up(a); 
    return a;
  }

  // 同一棵树里两个儿子合并, 注意不能用在不同树的相同结点合并。
  Info merge(Info a, Info b) {
    Info res;
    // merge 函数每一道题都需要重写!!!
    if (a.maxn < b.maxn) swap(a, b);
    if (a.maxn == b.maxn) {
      res.sum = a.sum + b.sum;
      res.maxn = a.maxn;
    } else {
      res.sum = a.sum;
      res.maxn = a.maxn;
    }
    return res;
  }

  void add(int pos, int val) {
    tree[pos].dat.sum = tree[pos].l;   // 需要重写
    if (tree[pos].dat.maxn != -INF) 
      tree[pos].dat.maxn += val;
    else tree[pos].dat.maxn = val;
  }

  void push_up(int pos) {
    tree[pos].dat.sum = 0;            // 可能需要修改的地方
    tree[pos].dat.maxn = -INF; 
    if (tree[pos].ls != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].ls].dat);
    if (tree[pos].rs != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].rs].dat);
  }

  void modify(int pos, int goal, int val) {
    if (tree[pos].l == tree[pos].r) {
      add(pos, val);
      return;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    if (goal <= mid) {
      if (tree[pos].ls == -1) tree[pos].ls = insert(tree[pos].l, mid);
      modify(tree[pos].ls, goal, val);
    } else {
      if (tree[pos].rs == -1) tree[pos].rs = insert(mid + 1, tree[pos].r);
      modify(tree[pos].rs, goal, val);
    }
    push_up(pos);
  }

  Info query(int pos, int l, int r) {
    if (tree[pos].l >= l && tree[pos].r <= r) {
      return tree[pos].dat;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    Info res; res.maxn = -INF; //注意初始值 可能需要修改的地方
                               
    if (l <= mid && tree[pos].ls != -1) {
      res = merge(res, query(tree[pos].ls, l, r));
    }
    if (r >= mid + 1 && tree[pos].rs != -1) {
      res = merge(res, query(tree[pos].rs, l, r));
    }
    push_up(pos);
    return res;
  }
};


signed main() {
//  file();
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n; cin >> n;
  vector<int> a(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  vector<vector<int>> G(n + 1);
  for (int i = 1; i <= n - 1; i++) {
    int u, v; cin >> u >> v;
    G[u].push_back(v); 
    G[v].push_back(u);
  }

  Segment_Tree T(n, 1, n);
  T.tree.reserve(n * 50);
  for (int i = 1; i <= n; i++) {
    T.modify(T.root[i], a[i], 1);
  }

  vector<long long> ans(n + 1, 0);
  function<void(int, int)> dfs = [&] (int pos, int fa) {
    for (auto &v : G[pos]) {
      if (v == fa) continue;
      dfs(v, pos);
      T.root[pos] = T.merge_tree_lossless(T.root[pos], T.root[v]); 
    }
    ans[pos] = T.query(T.root[pos], 1, n).sum;
  };

  dfs(1, -1);
  for (int i = 1; i <= n; i++) {
    cout << ans[i] << " ";
  }
   
  return 0;
}
```

# 31. 回滚并查集

```cpp
//
//  main.cpp
//  2022-05-24 16:27
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 回滚并查集
// 1. 操作的两个数 >= 0
// 2. rollback 回滚到第 t 次操作，注意一次合并有两次操作
struct DSU {
  vector<int> f, size;   
  vector<array<int, 2>> ops;
  DSU(int n) : f(n + 1), size(n + 1, 1) { iota(f.begin(), f.end(), 0); }

  inline int find(int x) {
    while (x != f[x]) x = f[f[x]];
    return x;
  }

  inline bool merge(int x, int y) {
    x = find(x), y = find(y); 
    if (x == y) return false;
    if (size[x] > size[y]) swap(x, y);
    ops.push_back({x, f[x]});
    f[x] = y;    
    ops.push_back({~y, size[y]});
    size[y] += size[x];
    return true;
  }

  void rollback(int t) {
    while ((int) ops.size() > t) {
      assert(ops.size());
      auto [i, j] = ops.back();
      ops.pop_back();
      if (i >= 0) {
        f[i] = j;
      } else {
        size[~i] = j;
      }
    }
  }
};


int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  

  return 0;
}

```

# 32. 字符串哈希

```cpp
//
//  main.cpp
//  2022-04-25 00:04
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;

// 说明:
// 1. 纯 int 环境不会错，所以无需 #define int long long
// 2. 注意调用函数是 1 indexed，只有 string 是 0 indexed
// 3. 初始化O(n), 查询子串 hash O(1), 添加长度为 len 的字符串 O(len)
// 4. 不能初始化一个空串
// 5. 关于哈希错误率: 单哈希 1e6 次匹配错误率 0.001, 双哈希 1e6 次匹配错误率 0.000001

struct Hash {
  const array<int, 2> MOD = {127657753, 987654319};
  
  const array<int, 2> P = {137, 277};

  int n;
  string s; // 0 - indexed
  vector<array<int, 2>> hs; // 1 - indexed
  vector<array<int, 2>> pw; // 0 - indexed

  Hash(string _s) {
    assert(_s.length());
    n = _s.length(); 
    s = _s; 
    hs.resize(n + 1);
    pw.resize(n + 1);
    
    pw[0] = {1, 1};
    for (int i = 1; i <= n; i++) {
      for (int j = 0; j < 2; j++) {
        pw[i][j] = 1ll * pw[i - 1][j] * P[j] % MOD[j];
      }
    }
    hs.resize(n + 1, {0, 0}); 
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < 2; j++) {
        hs[i + 1][j] = (1ll * hs[i][j] * P[j] % MOD[j] + s[i]) % MOD[j];
      }
    }
  }

  void add(string _s) {
    assert(_s.length());
    int old_n = n;
    n += _s.length(); 
    s += _s;
    hs.resize(n + 1);
    pw.resize(n + 1);
    for (int i = old_n + 1; i <= n; i++) {
      for (int j = 0; j < 2; j++) {
        pw[i][j] = 1ll * pw[i - 1][j] * P[j] % MOD[j];
      }
    }
    hs.resize(n + 1, {0, 0}); 
    for (int i = old_n; i < n; i++) {
      for (int j = 0; j < 2; j++) {
        hs[i + 1][j] = (1ll * hs[i][j] * P[j] % MOD[j] + s[i]) % MOD[j];
      }
    }
  }

  array<int, 2> get_hash(int l, int r) { // 1 - indexed
    assert(1 <= l && l <= r && r <= n); 
    array<int, 2> ans;
    for (int i = 0; i < 2; i++) {
      ans[i] = (hs[r][i] - 1ll * hs[l - 1][i] * pw[r - l + 1][i] % MOD[i] + MOD[i]) % MOD[i];
    }
    return ans;
  }
};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n; cin >> n; 
  vector<int> a;
  
  for (int i = 1; i <= n; i++) {
    string s; 
    cin >> s;
    Hash hs(s);
    a.push_back(hs.get_hash(1, (int) s.length())[0]);
  }

  sort(a.begin(), a.end());
  a.erase(unique(a.begin(), a.end()), a.end());
  
  cout << a.size();
  return 0;
}

```

# 33. 并查集哈希

```cpp
//
//  main.cpp
//  2022-08-15 03:23
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 并查集哈希
// 1. 对并查集的连通状态 hash, hash
//    相同说明连通性相同。
// 2. 请务必 #define int long long (这个代码能跑，建议不要动了)
// 3. 并查集是回滚并查集, 只需要把 rollback 删掉
//    然后路径压缩就可以变成普通并查集
// 
// 4. hash 坑点:
//    a. MOD 很大，所以做幂的时候要用 __int128
//    b. 异或操作不支持 MOD, 所以只能对 global_hash 取 MOD
//    c. hash 策略是对连通块做异或求2次方然后相加, 可以加强(?)成求size次方
//      (如果复杂度可以接受, 注意快速幂要局部__int128)。
//    d. 请使用随机时间种子，不要用固定种子，否则容易冲突。
//    e. 模数用的 1ll << 59 (随便选的)。可以考虑其他模数。
//

#define int long long

const int MOD = 1ll << 59;

struct DSU {
  vector<int> f, size, hash;
  vector<array<int, 3>> ops;
  int global_hash = 0;

  DSU(int n) : f(n + 1), size(n + 1, 1), hash(n + 1) {
    iota(f.begin(), f.end(), 0);

    mt19937 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<long long> R(0, MOD - 1);

    for (int i = 1; i <= n; i++) {
      hash[i] = R(rng);
      (global_hash += hash[i]) %= MOD;
    }
  }

  inline int find(int x) {
    while (x != f[x]) x = f[f[x]];
    return x;
  }

  inline bool merge(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (size[x] > size[y]) swap(x, y);

    ops.push_back({global_hash, 0, 0});
    ops.push_back({x, f[x], 1});
    ops.push_back({y, size[y], 2});
    ops.push_back({y, hash[y], 3});

    f[x] = y;

    global_hash = (global_hash - (__int128) hash[y] * hash[y] % MOD + MOD) % MOD;

    size[y] += size[x];

    global_hash = (global_hash - (__int128) hash[x] * hash[x] % MOD + MOD) % MOD;

    hash[y] ^= hash[x];

    global_hash = (global_hash + (__int128) hash[y] * hash[y] % MOD) % MOD;

    return true;
  }

  void rollback(int t) {
    while ((int) ops.size() > t) {
      auto [i, j, k] = ops.back();
      ops.pop_back();
      if (k == 0) {
        global_hash = i;
      } else if (k == 1) {
        f[i] = j;
      } else if (k == 2) {
        size[i] = j;
      } else if (k == 3) {
        hash[i] = j;
      }
    }
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

```

# 34. 强连通分量

```cpp
//
//  main.cpp
//  2022-07-20 20:54
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 强连通分量 SCC
// 1. 关于强连通分量的数量:
//    a. 如果图的index是从 1 开始，数量 = cnt - 1
//    b. 如果图的index是从 0 开始，数量 = cnt
// 2. 注意不要混淆新图的点和原图的点的编号。

struct Tarjan {
  vector<int> dfn, low, belong, ins, stk;
  vector<vector<int>> g;
  int dfs_clock, cnt;

  Tarjan(int n, vector<vector<int>> &_g) : g(_g) {
    dfn.resize(n + 1, 0); 
    low.resize(n + 1, 0);
    ins.resize(n + 1, 0);
    stk.resize(n + 1, 0);
    belong.resize(n + 1, 0);
    dfs_clock = cnt = 0;
    for (int i = 0; i <= n; i++) {
      if (!dfn[i]) dfs(i);    
    }
  }

  void dfs(int u) {
    dfn[u] = low[u] = ++dfs_clock;       
    stk.push_back(u);
    ins[u] = 1;
    for (auto &v : g[u]) {
      if (dfn[v] == 0) {
        dfs(v);
        low[u] = min(low[v], low[u]);
      } else if (ins[v]) {
        low[u] = min(dfn[v], low[u]);
      }
    }
    if (dfn[u] == low[u]) {
      int last;
      do {
        last = stk.back(); stk.pop_back();
        belong[last] = cnt;
        ins[last] = 0;
      } while (last != u);
      cnt++;
    }
  }
};


// https://codeforces.com/contest/999/problem/E

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
  int n, m, s; cin >> n >> m >> s; 

  vector<vector<int>> g1(n + 1); 

  for (int i = 1; i <= m; i++) {
    int u, v; cin >> u >> v; 
    g1[u].push_back(v);    
  }

  Tarjan T(n, g1);

  vector<vector<int>> g2(n + 1); 
  vector<int> degree(n + 1, 0);
  
  for (int i = 1; i <= n; i++) {
    int u = T.belong[i];
    for (auto &j : g1[i]) {
      int v = T.belong[j];
      if (u != v) {
        g2[u].push_back(v);
        degree[v]++;
      }
    }
  }

  int ans = 0;
  for (int i = 1; i <= T.cnt - 1; i++) {
    if (degree[i] == 0 && i != T.belong[s]) ans++;
  }

  cout << ans << endl; 

  return 0;
}

```

# 35. 文艺平衡树

```cpp
#include <bits/stdc++.h>

using namespace std;

// 文艺平衡树 Splay 实现
// 原理: 利用 splay 操作把 l 和 r 移动到一个子树中。
//
// 注意: 
// 1. splay 前务必用 find 找到下标，find 兼具 传递/清理 lazy 标记的作用。
//    保证 splay/rotate 过程中无需再 push_down
//    建议加 assert 检查
// 2. splay 有向上 update 的作用, modify 和 query 后 需要考虑是否 splay 向上 update
//
// 建议: 
// 1. 如果涉及到复杂的区间操作，比如删除区间，移动区间。
//    可以初始化的时候用 n + 2, 把 0 和 n + 1 也算进区间，方便 splay 操作。

template<class T>
struct Splay {
  struct Node {
    array<int, 2> son;
    int fa, lazy, cnt, size;
    T val;
    Node() : son({0, 0}) { fa = lazy = 0; cnt = size = 1; }
  };

  vector<Node> tree;

  int root;

  Splay(int n, vector<T> &a) {
    tree.resize(n + 1);
    for (int i = 1; i <= n; i++) {
      tree[i].val = a[i];
    }
    root = build(1, n, 0);
  }

  // 刚 build 完后 tree[pos] 对应原数组的 pos
  int build(int l, int r, int fa) {
    if (l > r) return 0;
    int mid = (l + r) >> 1;
    tree[mid].fa = fa;
    tree[mid].son[0] = build(l, mid - 1, mid);
    tree[mid].son[1] = build(mid + 1, r, mid);
    update(mid);
    return mid;
  }

  void push_down(int pos) {
    if (tree[pos].lazy) {
      int l = tree[pos].son[0], r = tree[pos].son[1];
      tree[l].lazy ^= 1;
      tree[r].lazy ^= 1;
      swap(tree[pos].son[0], tree[pos].son[1]);
      tree[pos].lazy = 0;
    }
  }

  // 按照 size 定位原数组第 x 个数在 Splay 中的下标
  int find(int x) {
    assert(x <= tree[root].size && x >= 1);
    int pos = root;
    while (true) {
      push_down(pos);
      int l = tree[pos].son[0], r = tree[pos].son[1];
      if (l && x <= tree[l].size) {
        pos = l;
      } else {
        x -= (l ? tree[l].size : 0) + 1;
        if (x <= 0) {
          splay(pos, 0);
          return pos;
        }
        pos = r;
      }
    }
  }

  int get_o(int pos) {
    return pos == tree[tree[pos].fa].son[1];
  }

  void update(int pos) {
    int l = tree[pos].son[0];
    int r = tree[pos].son[1];
    tree[pos].size = tree[pos].cnt + (l ? tree[l].size : 0) + (r ? tree[r].size : 0);
  }

  // 注意，rotate 时需要保证路径上的所有位置的 lazy 标记已经清空。
  // 即 先 find 再 splay
  void rotate(int pos) {
    int y = tree[pos].fa, z = tree[y].fa, o = get_o(pos);

    tree[y].son[o] = tree[pos].son[o ^ 1];
    if (tree[pos].son[o ^ 1]) tree[tree[pos].son[o ^ 1]].fa = y;

    tree[pos].son[o ^ 1] = y;
    tree[y].fa = pos;

    tree[pos].fa = z;
    if (z) tree[z].son[y == tree[z].son[1]] = pos;

    update(y), update(pos);
  }

  // o != 0 时表示把 pos 移动成 root 的儿子
  // o = 0 时表示把 pos 移动成 root
  void splay(int pos, int o) {
    while (tree[pos].fa != (o ? root : 0)) {
      int fa = tree[pos].fa;
      if (tree[fa].fa != (o ? root : 0))
        rotate(get_o(pos) == get_o(fa) ? fa : pos);
      rotate(pos);
    }
    if (o == 0) root = pos;
  }

  // 把 l, r 放在一个子树里，返回子树根节点下标
  // 注意这项操作会改变树的形状，注意 splay 操作的使用时机。
  int get_range(int l, int r) {
    assert(l >= 1 && r <= tree[root].size && l <= r);
    if (l == 1 && r == tree[root].size) return root;
    else if (l == 1 && r != tree[root].size) {
      int y = find(r + 1);
      splay(y, 0);
      return tree[y].son[0];
    } else if (r == tree[root].size && l != 1) {
      int x = find(l - 1);
      splay(x, 0);
      return tree[x].son[1];
    } else {
      assert(r != tree[root].size && l != 1);
      int x = find(l - 1), y = find(r + 1);
      splay(x, 0), splay(y, 1);
      return tree[y].son[0];
    }
  }

  // 区间操作
  // 多数时候 modify 之后需要 splay 操作来向上 update
  void modify(int l, int r) {
    assert(l >= 1 && r <= tree[root].size && l <= r);
    int pos = get_range(l, r);
    tree[pos].lazy ^= 1;
    push_down(pos);
    splay(pos, 0);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n, m; cin >> n >> m;

  vector<int> a(n + 1);
  iota(a.begin(), a.end(), 0);

  Splay<int> T(n, a);

  for (int i = 1; i <= m; i++) {
    int l, r; cin >> l >> r;
    T.modify(l, r);
  }

  for (int i = 1; i <= n; i++) {
    cout << T.tree[T.find(i)].val << " ";
  }

  return 0;
}
```

# 36. 普通最短路

```cpp
#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m, s; cin >> n >> m >> s;
  vector<vector<array<long long, 2>>> g(n + 1);
  for (int i = 1; i <= m; i++) {
    int u, v, w; cin >> u >> v >> w;
    g[u].push_back({w, v});
  }

  // Dijkstra
  const long long INF = 1e18;
  auto dijkstra = [&]() {
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> que;
    vector<long long> d(n + 1, INF);
    que.emplace(0, s); d[s] = 0;
    while (!que.empty()) {
      auto [dis, u] = que.top(); que.pop();
      if (dis > d[u]) continue;
      for (auto [w, v] : g[u]) {
        if (w + dis < d[v]) {
          d[v] = w + dis;
          que.emplace(d[v], v);
        }
      }
    }
    return d;
  };
  // Dijkstra

  auto ans = dijkstra();
  for (int i = 1; i <= n; i++) {
    cout << ans[i] << " "[i == n];
  }

  return 0;
}
```

# 37. 最小瓶颈路

```cpp
//
//  main.cpp
//  2022-07-19 23:28
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 最小瓶颈路
// 1. 定义: 两点之间所有路径中最大边权最小的路径。
//    定理: 最小生成树上两点之间在树上的路径是一个最小瓶颈路。
//
// 2. 原则上只能处理正边权，但是可以修改后处理负边权,
//    更改 max_edge 初始化最小值( 默认是0 )
//
// 3. 使用方法：
//    a. 初始化为点数 n
//    b. add 加边 u, v, w, 自动无向
//    c. build 建树, 不需要传参数
//    d. query(a, b) 查询 a <-> b 最小瓶颈路上的最大边权。
struct Bottleneck {
  
  struct DSU {
    vector<int> f, dep, size;   
    DSU(int n) : f(n + 1), dep(n + 1, 0), size(n + 1, 1) { iota(f.begin(), f.end(), 0); }
  
    inline int find(int x) {
      while (x != f[x]) x = f[x] = f[f[x]];
      return x;
    }
  
    inline bool merge(int x, int y) {
      x = find(x), y = find(y); 
  
      if (x == y) return false;
      if (dep[x] > dep[y]) swap(x, y);
      if (dep[x] == dep[y]) dep[y]++;
  
      f[x] = y;    
      size[y] += size[x];
      return true;
    }
  };

  // LCA·改
  // 求出 lca 的同时求出两点路径上的边权最大值。
  // p[i][j][0] 是边权(最大值)
  // G[i][j][0] 是边权
  struct LCA {
    vector<vector<array<int, 2>>> G, p;
    vector<int> d;
    int n, s; 
  
    LCA(int _n, int _s) : G(_n + 1), d(_n + 1, 1), n(_n), s(_s) {
      p.resize(_n + 1);
      int level = 0;
      while ((1 << level) <= _n) level++;
      for (int i = 0; i <= _n; i++) {
        p[i].resize(level + 1);
      }
    }
  
    int flag = false;
  
    void add(int u, int v, int w) {
      G[u].push_back({w, v});
      G[v].push_back({w, u});
    }
    
    void dfs(int pos, int fa) {
      p[pos][0][1] = fa;
      for (auto &[w, v] : G[pos]) {
        if (v == fa) continue;
        d[v] = d[pos] + 1;
        p[v][0][0] = w;
        dfs(v, pos);
      }
    }
  
    void init() {
      flag = true; 
      dfs(s, s); 
      for (int level = 1; (1 << level) <= n; level++) {
        for (int i = 1; i <= n; i++) {
          p[i][level][1] = p[p[i][level - 1][1]][level - 1][1];
          p[i][level][0] = max(p[i][level - 1][0], p[p[i][level - 1][1]][level - 1][0]);
        }
      }
    }
    // 返回 [0] 最大边权 [1] lca ，如果最大边权不存在，返回0
    array<int, 2> lca(int a, int b) {
      assert(flag);
      if (d[a] < d[b]) swap(a, b); 
      int i, j;
      for (i = 0; (1 << i) <= d[a]; i++);  
      i--; 
      int max_edge = 0;
      
      for (j = i; j >= 0; j--) {
        if (d[a] - (1 << j) >= d[b]) {
          max_edge = max(max_edge, p[a][j][0]);
          a = p[a][j][1];
        }
      }
      if (a == b) return {max_edge, b}; 
      for (j = i; j >= 0; j--) {
        if (p[a][j][1] != p[b][j][1]) {
          max_edge = max(max_edge, p[a][j][0]);
          max_edge = max(max_edge, p[b][j][0]);
          a = p[a][j][1];
          b = p[b][j][1];
        }
      }
      max_edge = max(max_edge, p[a][0][0]);
      max_edge = max(max_edge, p[b][0][0]);
      return {max_edge, p[a][0][1]};
    }
  };

  vector<array<int, 3>> edges;
  DSU T1;
  LCA T2; 

  Bottleneck(int n): T1(n), T2(n, 1) {}
  
  void add(int u, int v, int w) {
    edges.push_back({w, u, v});
  }

  void build() {
    sort(edges.begin(), edges.end());
    for(auto &[w, u, v] : edges) {
      if (T1.merge(u, v)) {
        T2.add(u, v, w);
      }
    }
    T2.init(); 
  }

  int query(int a, int b) {
    if (T1.find(a) != T1.find(b)) return 0;
    return T2.lca(a, b)[0]; 
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  
  return 0;
}

```

# 38. 生成随机数

```cpp
//
//  main.cpp
//  2022-07-08 20:41
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

mt19937 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<long long> U(1, 1000000000ll);

int main() {
  cout << U(rng);
  return 0;
}
```

# 39. 类欧几里得

```cpp
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
```

# 40. 线性求逆元

```cpp
//
//  main.cpp
//
//  Created by LiZnB on 2020/12/1.
//

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, MOD; cin >> n >> MOD;

  vector<int> inv(n + 1);

  inv[1] = 1;
  for (int i = 2; i <= n; i++) {
    inv[i] = 1ll * (MOD - MOD / i) * inv[MOD % i] % MOD;
  }

  for (int i = 1; i <= n; i++) {
    cout << inv[i] << '\n';
  }

  return 0;
}
```

# 41. 零一字典树

```cpp
//
//  零一字典树.cpp
//  2022-03-28 15:38
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

/*
 * 常用功能:
 * 1. 插入一个零一字符串
 * 2. 删除一个零一字符串
 * 3. 对于零一字符串可重复集合 S，和零一字符串 x, 询问 S 中与 x 异或结果大于(等于) r 的字符串的个数。
 * 4. 零一Trie DP
 * 5. 和异或相关的题大概率可以使用。
 *
 * 本模板特性:
 * 1. 需要修改二进制位数上限，默认31。表示可以处理 [0, 2^B - 1] 的数
 * 2. 因为 cf 栈空间等于memory limit, 所以可以直接在 main 声明 Trie, 不用反复 init
 */

struct Trie {
  const int B = 31; 

  vector<array<int, 2>> ch;
  vector<int> cnt; 

  Trie(int n) { // 这里必须传入 n 是为了避免忘记初始化
    ch.push_back({0, 0});    
    cnt.push_back(0);
  }

  void insert(int x, int o) { // o = 1 插入，o = -1 删除
    int p = 0;
    for (int i = B - 1; i >= 0; i--) {
      int c = (x >> i) & 1;
      if (!ch[p][c]) {
        ch.push_back({0, 0});
        cnt.push_back(0);
        ch[p][c] = (int) ch.size() - 1;
      }
      p = ch[p][c];
      cnt[p] += o;
    }
  }
  // 查询 Trie 里异或 x 大于, 小于, 等于 goal 的数的个数 
  array<int, 3> find(int x, int goal) { 
    int val = x ^ goal, up = 0, down = 0, eq = 0;
    int p = 0;
    for (int i = B - 1; i >= 0; i--) {
      int cv = (val >> i) & 1, cg = (goal >> i) & 1; 
      if (cg == 0 && ch[p][cv ^ 1]) up += cnt[ch[p][cv ^ 1]];
      if (cg == 1 && ch[p][cv ^ 1]) down += cnt[ch[p][cv ^ 1]];
      p = ch[p][cv];
      if (!p) break;
    }
    if (p) eq += cnt[p];  
    return {up, down, eq};
  }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

```

# 42. 最小树形图(Tarjan)

```cpp
#include<bits/stdc++.h>
#define int long long
using namespace std;

const int N = 3e5 + 9;

const long long inf = 1e18;

template<typename T> struct PQ {
  long long sum = 0;
  priority_queue<T, vector<T>, greater<T>> Q;
  void push(T x) { x.w -= sum; Q.push(x); }
  T pop() { auto ans = Q.top(); Q.pop(); ans.w += sum; return ans; }
  int size() { return Q.size(); }
  void add(long long x) { sum += x; }
  void merge(PQ &&x) {
    if (size() < x.size()) swap(sum, x.sum), swap(Q, x.Q);
    while (x.size()) {
      auto tmp = x.pop();
      tmp.w -= sum;
      Q.push(tmp);
    }
  }
};
struct edge {
  int u, v; long long w;
  bool operator > (const edge &rhs) const { return w > rhs.w; }
};
struct DSU {
  vector<int> par;
  DSU (int n) : par(n, -1) {}
  int root(int i) { return par[i] < 0 ? i : par[i] = root(par[i]); }
  void set_par(int c, int p) { par[c] = p; }
};

// 复杂度 O(m + nlogn)
// 返回树中除root外的父节点
// DMST 接受从 0 开始编号的点!!! 且不能有重边，自环。答案必须存在。 
// Takes ~300ms for n = 2e5
vector<int> DMST(int n, int root, const vector<edge> &edges) {
  vector<int> u(2 * n - 1, -1), par(2 * n - 1, -1);
  edge par_edge[2 * n - 1];
  vector<int> child[2 * n - 1];

  PQ<edge> Q[2 * n - 1];
  for (auto e : edges) Q[e.v].push(e);
  for (int i = 0; i < n; i++) Q[(i + 1) % n].push({i, (i + 1) % n, inf});

  int super = n;
  DSU dsu(2 * n - 1);
  int head = 0;
  while (Q[head].size()) {
    auto x = Q[head].pop();
    int nxt_root = dsu.root(x.u);
    if (nxt_root == head) continue;
    u[head] = nxt_root;
    par_edge[head] = x;
    if (u[nxt_root] == -1) head = nxt_root;
    else {
      int j = nxt_root;
      do {
        Q[j].add(-par_edge[j].w);
        Q[super].merge(move(Q[j]));
        assert(u[j] != -1);
        child[super].push_back(j);
        j = dsu.root(u[j]);
      } while (j != nxt_root);
      for (auto u : child[super]) par[u] = super, dsu.set_par(u, super);
      head = super++;
    }
  }
  vector<int> res(2 * n - 1, -1);
  queue<int> q; q.push(root);
  while (q.size()) {
    int u = q.front();
    q.pop();
    while (par[u] != -1) {
      for (auto v : child[par[u]]) {
        if (v != u) {
          res[par_edge[v].v] = par_edge[v].u;
          q.push(par_edge[v].v);
          par[v] = -1;
        }
      }
      u = par[u];
    }
  }
  res[root] = root; res.resize(n);
  return res;
}

int pre(int n, int root, vector<edge> &edges) {
  vector<edge> res; 
  map<int, int> w[n], vis[n];
  for (auto &e : edges) {
    if (vis[e.u][e.v]) w[e.u][e.v] = min(w[e.u][e.v], e.w);
    else {
      vis[e.u][e.v] = 1;
      w[e.u][e.v] = e.w;
    }
  }
  for (int i = 0; i < n; i++) {
    for (auto &it : w[i]) {
      if (i != it.first) res.push_back({i, it.first, it.second});
    }
  }

  edges = res;

  queue<int> que; que.push(root);
  vector<int> used(n, 0); used[root] = 1; 

  while (!que.empty()) {
    int x = que.front(); que.pop();      
    for (auto &it : w[x]) {
      if (used[it.first]) continue;       
      used[it.first] = 1;
      que.push(it.first);
    }
  }
  for (int i = 0; i < n; i++) 
    if (!used[i]) return 0;
  return 1;
}

signed main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  int n, m, root; cin >> n >> m >> root;
  root--;

  vector<edge> edges(m); 
  for (auto &e : edges) {
    cin >> e.u >> e.v >> e.w;
    e.u--, e.v--;  // 注意DMST接受从0开始的点。
  }

  if (!pre(n, root, edges)) { //去除重边，自环，判断是否存在答案。
    cout << -1 << endl; 
    return 0;
  }
   
  auto res = DMST(n, root, edges); 

  map<int, int> w[n];   
  for (auto &e : edges) w[e.v][e.u] = e.w;

  long long ans = 0ll; 
  for (int i = 0; i < n; i++) {
    if (i == root) continue;
    ans += w[i][res[i]];
  }
  cout << ans << endl; 
  return 0;
}

// https://judge.yosupo.jp/problem/directedmst
// http://www.cs.tau.ac.il/~zwick/grad-algo-13/directed-mst.pdf
```

# 43. 权值线段树(动态开点)

```cpp
//
//  权值线段树(动态开点).cpp
//  2022-03-29 09:16
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

/*
 * 关于权值线段树:
 * 1. 经典问题：求数列第K大（区间第K大用可持久化线段树）
 * 关于动态开点：
 * 1. 能不用就不用，能离散化就离散化
 * 2. 已知用处：可持久化数据结构，线段树合并，强制在线不能离散化的题。
 * 模板特性：
 * 1. 只支持单点修改，区间查询。(反过来也行，不支持lazy标记)
 * 2. 如果问题能用普通线段树解决，就不要用这个。空间复杂度 Knlogn（K是大常数）
 * 3. 支持负数权值
 * 4. 不用 #define int long long 可以保证 long long 内结果正确
 * 5. !!!!! pos 是 0-indexed !!!!
 * 6. !!!!!! 单点覆盖时，注意这个点存不存在！，存在时才能用 -old_val + new_val 覆盖,否则直接 new_val
 * 优化：
 * 1. 模板内置了 sum，maxn，minn。可以根据题目要求删除一些，减少空间消耗。
 * 2. 还可以进一步优化空间：把 l 和 r 也删掉，直接在函数里传参
 * 3. 参考：逆序对问题：n = 5e5, 数值范围 1e9, 做了1，2优化后消耗 75mb 空间。
 */

const long long INF = 0x3f3f3f3f3f3f3f3f;
struct Segment_Tree {

  struct Info {
    long long sum, maxn, minn; 
    Info () { sum = 0, maxn = 0, minn = 0; }
  };

  struct Tree {
    int ls, rs; // 左儿子，右儿子 编号
    int l, r; // 左边界，右边界
    Info dat;
    Tree(int a, int b, int c, int d) : ls(a), rs(b), l(c), r(d) {} 
  };

  vector<Tree> tree; 

  Segment_Tree(int l, int r) {
    tree.push_back({-1, -1, l, r});
  }

  int insert(int l, int r) {
    tree.push_back({-1, -1, l, r});
    return (int) tree.size() - 1;
  }
 
  Info merge(Info a, Info b) {
    Info res;
    res.sum = a.sum + b.sum;
    res.maxn = max(a.maxn, b.maxn);
    res.minn = min(a.minn, b.minn);
    return res;
  }

  void add(int pos, long long val) {
    tree[pos].dat.sum += val;
    tree[pos].dat.maxn += val;
    tree[pos].dat.minn += val;
  }

  void push_up(int pos) {
    tree[pos].dat.sum = 0; 
    tree[pos].dat.maxn = -INF;
    tree[pos].dat.minn = INF;
    if (tree[pos].ls != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].ls].dat);
    if (tree[pos].rs != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].rs].dat);
  }

  void modify(int pos, int goal, long long val) {
    if (tree[pos].l == tree[pos].r) {
      add(pos, val);
      return;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    if (goal <= mid) {
      if (tree[pos].ls == -1) tree[pos].ls = insert(tree[pos].l, mid);
      modify(tree[pos].ls, goal, val);
    } else {
      if (tree[pos].rs == -1) tree[pos].rs = insert(mid + 1, tree[pos].r);
      modify(tree[pos].rs, goal, val);
    }
    push_up(pos);
  }

  Info query(int pos, int l, int r) {
    if (tree[pos].l >= l && tree[pos].r <= r) {
      return tree[pos].dat;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    Info res; res.maxn = -INF, res.minn = INF; //注意初始值

    if (l <= mid && tree[pos].ls != -1) {
      res = merge(res, query(tree[pos].ls, l, r));
    }
    if (r >= mid + 1 && tree[pos].rs != -1) {
      res = merge(res, query(tree[pos].rs, l, r));
    }
    push_up(pos);
    return res;
  }

  void modify(int goal, long long val) { modify(0, goal, val); }
  Info query(int l, int r) { return query(0, l, r); }
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  Segment_Tree T(1, 1e9); 

  
  return 0;
}
```

# 44. 中国剩余定理

```cpp
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
```

# 45. 快速数论变换

```cpp
#include <bits/stdc++.h>
using namespace std;

// NTT 快速数论变换
// 注意:
// 1. 模数 998244353 和 1004535809 的原根是 3
// 2. 998244353 能整除最大 2 ^ 23
// 3. 1004535809 能整除最大 2 ^ 21
// 4. MOD, g, ig 定义成全局 const 会在编译阶段优化取模运算
// 5. NTT 一般比 FFT 快

const long long MOD = 998244353;
const long long g = 3, ig = 332748118;

struct Poly {
  vector<long long> c;
  vector<int> rev;

  Poly(vector<long long> &_c) : c(_c) {
    assert((int) c.size());
  }

  long long power(long long a, long long b) {
    long long base = a, ans = 1ll;
    while (b) {
      if (b & 1) ans = 1ll * ans * base % MOD;
      base = 1ll * base * base % MOD;
      b >>= 1;
    }
    return ans;
  }

  void dft(int limit, int o) {
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
      long long step = power(o == 1 ? g : ig, (MOD - 1) / fa);
      for (int i = 0; i < limit; i += fa) {
        long long w = 1;
        for (int j = i; j < i + son; j++) {
          long long x = c[j], y = (1ll * c[j + son] * w) % MOD;
          c[j] = (x + y) % MOD;
          c[j + son] = (x - y + MOD) % MOD;
          (w *= step) %= MOD;
        }
      }
    }
    if (o == -1) {
      long long ilimit = power(limit, MOD - 2);
      for (int i = 0; i < limit; i++)
        c[i] = 1ll * c[i] * ilimit % MOD;
    }
  }

  friend Poly operator * (Poly a, Poly b) {
    int limit = 1;
    int len = (int) a.c.size() + (int) b.c.size() - 1;
    while (limit < len) limit <<= 1;

    a.dft(limit, 1); b.dft(limit, 1);

    vector<long long> res(limit);
    for (int i = 0; i < limit; i++) {
      res[i] = 1ll * a.c[i] * b.c[i] % MOD;
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
  ios::sync_with_stdio(false);
  cin.tie(0);

  int n, m; cin >> n >> m;
  vector<long long> a(n + 1), b(m + 1);
  for (int i = 0; i <= n; i++) cin >> a[i];
  for (int i = 0; i <= m; i++) cin >> b[i];

  Poly A(a), B(b);
  Poly C = A * B;

  for (auto &it : C.c) cout << it << " ";

  return 0;
}
```

# 46. 快速读入输出

```cpp
//
//  main.cpp
//  2022-06-07 22:19
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 快速读入输出
// 1. From tourist 
// 2. 唯一的修改, 增加 #define endl '\n'
// 3. 支持 __int128
// 4. 其他用法 cin cout 一样

#define endl '\n'
static struct FastInput {
  static constexpr int BUF_SIZE = 1 << 20;
  char buf[BUF_SIZE];
  size_t chars_read = 0;
  size_t buf_pos = 0;
  FILE *in = stdin;
  char cur = 0;

  inline char get_char() {
    if (buf_pos >= chars_read) {
      chars_read = fread(buf, 1, BUF_SIZE, in);
      buf_pos = 0;
      buf[0] = (chars_read == 0 ? -1 : buf[0]);
    }
    return cur = buf[buf_pos++];
  }
 
  inline void tie(int) {}

  inline explicit operator bool() {
    return cur != -1;
  }

  inline static bool is_blank(char c) {
    return c <= ' ';
  }

  inline bool skip_blanks() {
    while (is_blank(cur) && cur != -1) {
      get_char();
    }
    return cur != -1;
  }
 
  inline FastInput& operator>>(char& c) {
    skip_blanks();
    c = cur;
    return *this;
  }
  
  inline FastInput& operator>>(string& s) {
    if (skip_blanks()) {
      s.clear();
      do {
        s += cur;
      } while (!is_blank(get_char()));
    }
    return *this;
  }
 
  template <typename T>
  inline FastInput& read_integer(T& n) {
    // unsafe, doesn't check that characters are actually digits
    n = 0;
    if (skip_blanks()) {
      int sign = +1;
      if (cur == '-') {
        sign = -1;
        get_char();
      }
      do {
        n += n + (n << 3) + cur - '0';
      } while (!is_blank(get_char()));
      n *= sign;
    }
    return *this;
  }

  template <typename T>
  inline typename enable_if<is_integral<T>::value, FastInput&>::type operator>>(T& n) {
    return read_integer(n);
  }
  
  #if !defined(_WIN32) || defined(_WIN64)
  inline FastInput& operator>>(__int128& n) {
    return read_integer(n);
  }
  #endif

  template <typename T>
  inline typename enable_if<is_floating_point<T>::value, FastInput&>::type operator>>(T& n) {
    // not sure if really fast, for compatibility only
    n = 0;
    if (skip_blanks()) {
      string s;
      (*this) >> s;
      sscanf(s.c_str(), "%lf", &n);
    }
    return *this;
  }
} fast_input;

#define cin fast_input

static struct FastOutput {
  static constexpr int BUF_SIZE = 1 << 20;
  char buf[BUF_SIZE];
  size_t buf_pos = 0;
  static constexpr int TMP_SIZE = 1 << 20;
  char tmp[TMP_SIZE];
  FILE *out = stdout;

  inline void put_char(char c) {
    buf[buf_pos++] = c;
    if (buf_pos == BUF_SIZE) {
      fwrite(buf, 1, buf_pos, out);
      buf_pos = 0;
    }
  }

  ~FastOutput() {
    fwrite(buf, 1, buf_pos, out);
  }

  inline FastOutput& operator<<(char c) {
    put_char(c);
    return *this;
  }

  inline FastOutput& operator<<(const char* s) {
    while (*s) {
      put_char(*s++);
    }
    return *this;
  }
 
  inline FastOutput& operator<<(const string& s) {
    for (int i = 0; i < (int) s.size(); i++) {
      put_char(s[i]);
    }
    return *this;
  }
 
  template <typename T>
  inline char* integer_to_string(T n) {
    // beware of TMP_SIZE
    char* p = tmp + TMP_SIZE - 1;
    if (n == 0) {
      *--p = '0';
    } else {
      bool is_negative = false;
      if (n < 0) {
        is_negative = true;
        n = -n;
      }
      while (n > 0) {
        *--p = (char) ('0' + n % 10);
        n /= 10;
      }
      if (is_negative) {
        *--p = '-';
      }
    }
    return p;
  }

  template <typename T>
  inline typename enable_if<is_integral<T>::value, char*>::type stringify(T n) {
    return integer_to_string(n);
  }

  #if !defined(_WIN32) || defined(_WIN64)
  inline char* stringify(__int128 n) {
    return integer_to_string(n);
  }
  #endif

  template <typename T>
  inline typename enable_if<is_floating_point<T>::value, char*>::type stringify(T n) {
    sprintf(tmp, "%.17f", n);
    return tmp;
  }

  template <typename T>
  inline FastOutput& operator<<(const T& n) {
    auto p = stringify(n);
    for (; *p != 0; p++) {
      put_char(*p);
    }
    return *this;
  }
} fast_output;

#define cout fast_output

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  return 0;
}
```

# 47. 扩展欧几里得

```cpp
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

```

# 48. 拉格朗日插值

```cpp
//
//  main.cpp
//  2022-07-24 20:34
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

#define int long long

// 拉格朗日插值
// 1. 给 n 个点，确定一个最高次是 n - 1 的多项式, 
//    求出这个多项式在位置 k 的值: f(k)。
// 2. 如果 x 是连续的那么就可以 O(n) 求 f(k), 需要推式子。
// 3. 如果 x 是无规律的，可以 O(n ^ 2) 求。
// 4. 需要注意的是传入的点的 x 坐标必须两两不同。
// 5. 公式: f(k) = sum_{0}^{n} (yi \prod_{i != j} ((k - xj) / (xi - xj)))
// 6. a: 0-index

struct Lagrange {
  // 务必使用 #define int long long
  vector<array<int, 2>> a;
  int MOD;

  Lagrange(vector<array<int, 2>> &_a, int _MOD) : a(_a), MOD(_MOD) {}

  int power(int a, int b) {
    assert(b >= 0);
    int base = a, ans = 1;
    while (b) {
      if (b & 1) ans = 1ll * ans * base % MOD;
      base = 1ll * base * base % MOD;
      b >>= 1;
    }
    return ans;
  } 

  int inv(int x) {
    return power(x, MOD - 2);
  }

  // O(n ^ 2), x 无特殊规律。
  int f1(int k) {
    int ans = 0;
    for (int i = 0; i < (int) a.size(); i++) {
      int res = 1;
      for (int j = 0; j < (int) a.size(); j++) {
        if (i == j) continue;
        res = 1ll * res * (k - a[j][0] + MOD) % MOD * power((a[i][0] - a[j][0] + MOD) % MOD, MOD - 2) % MOD;
      }
      ans = (ans + 1ll * res * a[i][1]) % MOD;
    }
    return ans;
  }

  // O(n), X 连续且为 0 到 a.size() - 1
  // 注意是否从小到大排好序了！
  int f2(int k) {
    vector<int> fac(a.size()), pre(a.size()), suf(a.size()); 
    fac[0] = 1;
    pre[0] = k - a[0][0];
    suf.back() = k - (a.back()[0]);
    for (int i = 1; i < (int) fac.size(); i++) {
      fac[i] = fac[i - 1] * i % MOD;
      pre[i] = pre[i - 1] * ((k - a[i][0] + MOD) % MOD) % MOD;
    }
    for (int i = (int) a.size() - 2; i >= 0; i--) {
      suf[i] = suf[i + 1] * ((k - a[i][0] + MOD) % MOD) % MOD;
    }
    int ans = 0ll;
    for (int i = 0; i < (int) a.size(); i++) {
      int res = 1ll;
      if (i) res = res * pre[i - 1] % MOD;
      if (i != (int) a.size() - 1) res = res * suf[i + 1] % MOD;
      res = res * inv(fac[i]) % MOD * inv(fac[(int) a.size() - 1 - i]) % MOD;
      if (((int) a.size() - 1 - i) % 2) res = (res * -1 + MOD) % MOD;
      ans = (ans + a[i][1] * res % MOD) % MOD;
    }
    return ans;
  }

};


signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n, k; cin >> n >> k;
  vector<array<int, 2>> a(n); 

  for (int i = 0; i < n; i++) {
    cin >> a[i][0] >> a[i][1];
  }

  Lagrange T(a, 998244353);

  cout << T.f1(k) << " " << T.f2(k);
  
  return 0;
}

```

# 49. 无向图最小环

```cpp
//
//  main.cpp
//  2022-04-12 11:53
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

// 无向图最小环
// 思想:
// 枚举环中相邻两条边，i-k, k-j. 加上 i-j 不经过 k 的最短路，更新答案。

const int N = 200;
int G[N][N], f[N][N];

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n, m; cin >> n >> m;  

  const int INF = 0x3f3f3f3f;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      f[i][j] = G[i][j] = INF;
    }
  }

  for (int i = 1; i <= m; i++) {
    int u, v, w; cin >> u >> v >> w;
    f[u][v] = f[v][u] = w;
    G[u][v] = G[v][u] = w;
  }

  for (int i = 1; i <= n; i++) f[i][i] = 0;

  int ans = INF;

  for (int k = 1; k <= n; k++) {
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        if (i == j || i == k || k == j) continue;
        ans = min(ans, f[i][j] + G[i][k] + G[k][j]);
      }
    }
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        f[i][j] = min(f[i][k] + f[k][j], f[i][j]);
      }
    }
  }
  if (ans == INF) 
    cout << "No solution.";
  else 
    cout << ans;
  return 0;
}
```

# 50. 最近公共祖先

```cpp
//
//  main.cpp
//  2022-04-19 00:01
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;


// 特性：
// 1. add 会自动add双向边
// 2. add 必须 add 一颗标准的树，边数比点数少一的那种。
// 3. 如果是森林需要开多个 LCA 来存每一个树。 
// 4. 查询 lca 前要初始化
// 5. 编号请从1开始

struct LCA {
  vector<vector<int>> G, p;
  vector<int> d;
  int n, s; 

  LCA(int _n, int _s) : G(_n + 1), d(_n + 1, 1), n(_n), s(_s) {
    p.resize(_n + 1);
    int level = 0;
    while ((1 << level) <= _n) level++;
    for (int i = 0; i <= _n; i++) {
      p[i].resize(level + 1);
    }
  }

  int flag = false;

  void add(int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
  }
  
  void dfs(int pos, int fa) {
    p[pos][0] = fa;
    for (auto &v : G[pos]) {
      if (v == fa) continue;
      d[v] = d[pos] + 1;
      dfs(v, pos);
    }
  }

  void init() {
    flag = true; 
    dfs(s, s); 
    for (int level = 1; (1 << level) <= n; level++) {
      for (int i = 1; i <= n; i++) {
        p[i][level] = p[p[i][level - 1]][level - 1];
      }
    }
  }

  int lca(int a, int b) {
    assert(flag);
    if (d[a] < d[b]) swap(a, b); 
    int i, j;
    for (i = 0; (1 << i) <= d[a]; i++);  
    i--; 
    for (j = i; j >= 0; j--) {
      if (d[a] - (1 << j) >= d[b]) {
        a = p[a][j];
      }
    }
    if (a == b) return b; 
    for (j = i; j >= 0; j--) {
      if (p[a][j] != p[b][j]) {
        a = p[a][j];
        b = p[b][j];
      }
    }
    return p[a][0];
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  

  return 0;
}

```

# 51. 点双连通分量

```cpp
//
//  main.cpp
//  2022-07-27 21:30
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// Tarjan 求点双连通分量和割点
// 1. bccno : 非割点属于的点双连通分量编号(从0开始), 割点编号无意义(可以属于多个点双连通分量)。
// 2. bcc : 每个点双连通分量包含的点。(包含割点, 编号从0开始)
// 3. belong : 原图每个点属于 vbcc 缩点图哪个点
// 3. 注意: 点数小于等于2的连通图是bcc
// 4. 重边不影响答案，但是务必(在读入时，而不是模板内!)除掉自环
// 5. v-BCC 缩点方式: 割点保留，为每一个 bcc 建一个新点，连接割点和 bcc 新点。
// 6. vbcc 缩点图 1 indexed, 注意 vbcc 缩点图点数可能大于 n 例如 1-2-3-4-5 有 3 个割点 4 个 bcc
// 7. 不缩点可以快大约 0.4s  

struct Tarjan {
  
  vector<int> dfn, low, iscut, bccno, belong;
  vector<vector<int>> g, bcc, ng;
  stack<array<int, 2>> stk;
  int dfs_clock;

  Tarjan(int o, int n, vector<vector<int>> &_g) : g(_g) {
    dfn.resize(n + 1, 0);
    low.resize(n + 1, 0);
    iscut.resize(n + 1, 0);
    bccno.resize(n + 1, -1);  
    belong.resize(n + 1, 0);

    dfs_clock = 0; 

    for (int i = o; i <= n; i++) {
      if (!dfn[i]) dfs(i, -1);
    }
    vbcc(n);
  }

  int dfs(int u, int fa) {
    int lowu = dfn[u] = ++dfs_clock;
    int child = 0;  
    for (auto &v : g[u]) {
      if (!dfn[v]) {
        stk.push({u, v});    
        child++;
        int lowv = dfs(v, u);
        lowu = min(lowu, lowv);
        if (lowv >= dfn[u]) {
          iscut[u] = true;
          bcc.push_back({});
          while (true) {
            auto [xu, xv] = stk.top(); stk.pop();
            if (bccno[xu] != (int) bcc.size() - 1) {
              bcc.back().push_back(xu); 
              bccno[xu] = (int) bcc.size() - 1;
            }
            if (bccno[xv] != (int) bcc.size() - 1) {
              bcc.back().push_back(xv);
              bccno[xv] = (int) bcc.size() - 1;
            }
            if (xu == u && xv == v) break;
          }
        } 
      } else if (dfn[v] < dfn[u] && v != fa) {
        stk.push({u, v});
        lowu = min(lowu, dfn[v]);
      }
    }
    if (fa < 0 && child == 1) iscut[u] = 0; 
    if (fa < 0 && (int) g[u].size() == 0) bcc.push_back({u});
    return lowu;
  }

  void vbcc(int n) {
    int tot = 0;
    for (int i = 1; i <= n; i++) {
      if (iscut[i]) {
        belong[i] = ++tot;
      }
    }
    ng.resize(tot + bcc.size() + 1);
    for (int i = 0; i < (int) bcc.size(); i++) {
      tot++;
      for (auto &it : bcc[i]) {
        if (iscut[it]) {
          ng[belong[it]].push_back(tot);
          ng[tot].push_back(belong[it]);
        } else {
          belong[it] = tot;
        }
      }
    }
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n, m; cin >> n >> m;
  vector<vector<int>> g(n + 1);
  for (int i = 1; i <= m; i++) {
    int u, v; cin >> u >> v;
    if (u == v) continue;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  
  Tarjan T(1, n, g);

  cout << T.bcc.size() << endl;
  
  for (int i = 0; i < (int) T.bcc.size(); i++) {
    cout << T.bcc[i].size() << " ";
    for (auto &it : T.bcc[i]) cout << it << " "; 
    cout << endl;
  }

  return 0;
}

```

# 52. 自适应辛普森

```cpp
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

```

# 53. 莫比乌斯函数

```cpp
//
//  main.cpp
//  2022-06-05 13:53
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 说明:
// 1. is = 0 时是质数
// 2. 线性筛：利用莫比乌斯函数的积性
struct Mobius {
  vector<int> is, prinum, miu, sum;

  Mobius(int n) : is(n + 1, 0), miu(n + 1, 1), sum(n + 1, 0) {
    prinum.reserve(n + 1);
    for (int i = 2; i <= n; i++) {
      if (!is[i]) prinum.push_back(i), miu[i] = -1;  
      for (auto &it : prinum) {
        if (it * i > n) break;
        is[it * i] = 1;
        if (i % it == 0) {
          miu[it * i] = 0;
          break;
        } else {
          miu[it * i] = -1 * miu[i];
        }
      }
    }
    for (int i = 1; i <= n; i++) {
      sum[i] = sum[i - 1] + miu[i];
    }
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  Mobius T(100);
  for (int i = 1; i <= 100; i++) {
    cout << T.miu[i] << " ";  
  }

  return 0;
}

```

# 54. 莫比乌斯反演

```cpp
//
//  main.cpp
//  2022-06-05 13:51
//
//  Created by liznb
//  
#include <bits/stdc++.h>
#define int long long
using namespace std;

// 莫比乌斯反演:
// 注意: 一定打开 #define int long long
//
// 两个公式: 
// 1. f(n) = sigma{u(d) * F(n / d)} (d|n)
// 2. f(n) = sigma{u(d / n) * F(d)} (n|d)
//
// 莫比乌斯反演题特点：
// 1. 一般和 GCD，约数有关。 
// 2. 简单的莫比乌斯反演题可以直接套 gcd 计数, 基本不需要推式子。
// 3. 复杂的莫比乌斯反演需要把GCD计数写进多重和式里，然后推式子构造适合整除分块的式子，化简复杂度。
//
// 经典构造:
// 求 gcd(i, j) = k 的对数，其中 i <= n, j <= m
// 设 f(k) 是答案。F(k) 是 i <= n, j <= m 中 k | gcd(i, j) 的对数。
// 显然 F(k) = (n / k) * (m / k) = sigma{f(d)} (k | d);
// 所以 f(k) = sigma{u(d / k) * (n / d) * (m / d)} (k | d);
// 进一步转换，gcd(i, j) = k 与 gcd(i / k, j / k) = 1 等价。
// 所以只需要每次求 f(1), 但 n / k 和 m / k 不同的情况。用整除分块优化成 sqrt(min(n / k, m / k))
//
// 整除分块:
// i 从小到大, n / i 不相等的下一个 i 是 n / (n / i) + 1

struct Mobius {
  vector<int> is, prinum, miu, sum;

  Mobius(int n) : is(n + 1, 0), miu(n + 1, 1), sum(n + 1, 0) {
    prinum.reserve(n + 1);
    for (int i = 2; i <= n; i++) {
      if (!is[i]) prinum.push_back(i), miu[i] = -1;  
      for (auto &it : prinum) {
        if (it * i > n) break;
        is[it * i] = 1;
        if (i % it == 0) {
          miu[it * i] = 0;
          break;
        } else {
          miu[it * i] = -1 * miu[i];
        }
      }
    }
    for (int i = 1; i <= n; i++) {
      sum[i] = sum[i - 1] + miu[i];
    }
  }

  // gcd(i, j) = k (1 <= i <= n,  1 <= j <= m) 的对数
  // 复杂度 
  // 1. sqrt(min(n / k, m / k)) 单次
  // 2. 如果求 k 从 1 到 t, 复杂度低于 tlogt 
  //
  // 进一步优化策略:
  // 1. 如果 k 从 1 到 t: 外层循环可以再套一个整除分块，因为g函数里开头做了除法。
  // 2. 如果 k 是 质数: 用埃筛把每一个质数的倍数处的 miu 函数贡献累计一下(不再使用整除分块)。复杂度 O(n + nloglogn)
  // 3. 如果 n 和 m 始终一样，可以记忆化。
  
  int g(int n, int m, int k) {
    n /= k, m /= k; // 套用外层整除分块需要删除所有k。
    int limit = min(n, m);
    int ans = 0, nxt;
    for (int i = 1; i <= limit; i = nxt) {
      int nxtn = n / (n / i) + 1;
      int nxtm = m / (m / i) + 1;
      nxt = min({nxtn, nxtm, limit + 1});
      ans += (sum[nxt - 1] - sum[i - 1]) * (n / i) * (m / i);
    }
    return ans;
  }
  
};

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0); 
  
  return 0;
}
```

# 55. 边双连通分量

```cpp
//
//  main.cpp
//  2022-08-09 13:38
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 边双连通分量
// 1. 重边对边双连通分量有影响，模板可以处理。
// 2. 需要把双向边都存入 edges, 编号从 0 开始，保证双向边可以异或得到。
struct Tarjan {
  vector<int> dfn, low, belong, isbridge;
  vector<vector<int>> g, bcc;
  vector<array<int, 2>> edges;

  int dfs_clock;

  Tarjan(int o, int n, vector<vector<int>> &_g, vector<array<int, 2>> &_edges) : g(_g), edges(_edges) {
    dfn.resize(n + 1, 0);
    low.resize(n + 1, 0);
    isbridge.resize(edges.size(), 0);
    belong.resize(n + 1, -1);
    dfs_clock = 0;

    for (int i = o; i <= n; i++) {
      if (!dfn[i]) dfs(i, -1);
    }

    for (int i = o; i <= n; i++) {
      if (belong[i] == -1) {
        bcc.push_back({});
        ebcc(i);
      }
    }
  }

  int dfs(int u, int from) {
    int lowu = dfn[u] = ++dfs_clock;
    for (auto &it : g[u]) {
      auto [x, v] = edges[it];
      if (!dfn[v]) {
        int lowv = dfs(v, it);
        lowu = min(lowu, lowv);
        if (dfn[u] < low[v]) {
          isbridge[it] = 1;
          isbridge[it ^ 1] = 1;
        }
      } else if (dfn[v] < dfn[u] && (it ^ 1) != from) {
        lowu = min(lowu, dfn[v]);
      }
    }
    low[u] = lowu;
    return lowu;
  }

  void ebcc(int u) {
    belong[u] = (int) bcc.size() - 1;
    bcc.back().push_back(u);
    for (auto &it : g[u]) {
      if (isbridge[it]) continue;
      auto [x, v] = edges[it];
      if (belong[v] != -1) continue;
      ebcc(v);
    }
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int n, m; cin >> n >> m;

  vector<array<int, 2>> edges;
  vector<vector<int>> g(n + 1);

  auto add = [&] (int u, int v) {
    edges.push_back({u, v}); 
    g[u].push_back((int) edges.size() - 1);
  };

  for (int i = 1; i <= m; i++) {
    int u, v; cin >> u >> v;
    add(u, v); add(v, u);
  }
  
  Tarjan T(1, n, g, edges);

  cout << T.bcc.size() << endl;
  for (int i = 0; i < (int) T.bcc.size(); i++) {
    cout << T.bcc[i].size() << " ";
    for (auto &it : T.bcc[i]) cout << it << " ";
    cout << endl;
  }

  return 0;
}

```

# 56. 大质因数分解(Pollard-Rho算法)

```cpp
//
//  main.cpp
//  2022-07-07 20:28
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// Pollard_Rho 算法
// 期望复杂度 O(n ^ 0.25)
// 来自 Rockdu

namespace NT {
  long long gcd(long long a, long long b) {
    return b ? gcd(b, a % b) : a;
  }
  long long mul(long long a, long long b, long long m) {
    long long s = a * b - (long long)((long double)a * b / m + 0.5) * m;
    return s < 0 ? s + m : s;
  }
  long long fpow(long long x, long long a, long long m) {
    long long ans = 1;
    while(a) {
      if(a & 1) ans = mul(ans, x, m);
      x = mul(x, x, m), a >>= 1;
    }
    return ans;
  }
}

namespace Miller_Rabin {
  using namespace NT;
  long long p[15] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  int detect(long n, long long a) {
    if(n == a) return 1;
    if(a % n == 0) return 1;
    long long now = n - 1, lst = 1;
    if(fpow(a, now, n) != 1) 
      return 0;
    while(!(now & 1)) {
      now /= 2;
      long long p = fpow(a, now, n);
      if(lst == 1 && p != 1 && p != n - 1)
        return 0;
      lst = p;
    }
    return 1;
  }
  
  long long MR(long long n) {
    if(n < 2) return 0;
    for(int i = 0; i < 7; ++i) {
      if(!detect(n, p[i])) 
        return 0;
    }
    return 1;
  }
}

namespace Pollard_Rho {
  using namespace NT;
  using namespace Miller_Rabin;
  long long f(long long x, long long C, long long p) {
    return (mul(x, x, p) + C) % p;
  }
  long long Rand() {return (rand() << 15) + rand();}
  long long Randll() {return (Rand() << 31) + Rand();}
  
  long long PR(long long n) {
    if(n == 4) return 2;
    if(MR(n)) return n;
    while(1) {
      long long C = Randll() % (n - 1) + 1;
      long long s = 0, t = 0;
      long long acc = 1;
      do {
        for(int i = 1; i <= 128; ++i) {
          t = f(f(t, C, n), C, n);
          s = f(s, C, n);
          long long tmp = mul(acc, abs(t - s), n);
          if(s == t || tmp == 0)
            break;
          acc = tmp;
        }
        long long d = gcd(n, acc);
        if(d > 1) return d;
      } while(s != t);
    }
  }
  
  // 返回的 first 是质因子, second 是质因子个数, 从小到大排序。
  typedef pair<long long, int > pii;
  vector<pii> DCOMP(long long n) {
    vector<pii> ret;
    while(n != 1) {
      long long p = PR(n);
      while(!MR(p)) 
        p = PR(p);
      int c = 0;
      while(n % p == 0) n /= p, ++c;
      ret.push_back(make_pair(p, c));
    }
    sort(ret.begin(), ret.end());
    return ret;
  }
}

signed main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  int z; cin >> z; 
  
  while (z--) {
    long long n; cin >> n;
    auto res = Pollard_Rho::DCOMP(n); 

    int cnt = 0;
    for (auto &it : res) cnt += it.second;
    // cnt 等于 1 说明 n 是质数。
    if (cnt == 1) cout << "Prime" << endl;
    else cout << res.back().first << endl;
  }

  return 0;
}

```

# 57. 无向图最小割(Stoer-Wagner)

```cpp
//
//  main.cpp
//  2022-04-06 21:33
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
#define LOCAL
using namespace std;

// Stoer-Wagner 算法
// 来自 YouKn0wWho
// 求无向图的最小割
// 特性说明：
// 1. 点从1开始，初始化时直接传点数n
// 2. add_edge 自动添加无向图

const int N = 1000; 
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct StoerWagner {
  int n;
  long long G[N][N], dis[N];
  int idx[N];
  bool vis[N];
  const long long inf = 1e18;
  StoerWagner() {}
  StoerWagner(int _n) {
    n = _n;
    memset(G, 0, sizeof G);
  }
  void add_edge(int u, int v, long long w) { //undirected edge, multiple edges are merged into one edge
    // 无向图
    if (u != v) {
      G[u][v] += w;
      G[v][u] += w;
    }
  }
  long long solve() {
    long long ans = inf;
    for (int i = 0; i < n; ++ i) idx[i] = i + 1;
    shuffle(idx, idx + n, rnd);
    while (n > 1) {
      int t = 1, s = 0;
      for (int i = 1; i < n; ++ i) {
        dis[idx[i]] = G[idx[0]][idx[i]];
        if (dis[idx[i]] > dis[idx[t]]) t = i;
      }
      memset(vis, 0, sizeof vis);
      vis[idx[0]] = true;
      for (int i = 1; i < n; ++ i) {
        if (i == n - 1) {
          if (ans > dis[idx[t]]) ans = dis[idx[t]]; //idx[s] - idx[t] is in two halves of the  mincut
          if (ans == 0) return 0;
          for (int j = 0; j < n; ++ j) {
            G[idx[s]][idx[j]] += G[idx[j]][idx[t]];
            G[idx[j]][idx[s]] += G[idx[j]][idx[t]];
          }
          idx[t] = idx[-- n];
        }
        vis[idx[t]] = true;
        s = t;
        t = -1;
        for (int j = 1; j < n; ++ j) {
          if (!vis[idx[j]]) {
            dis[idx[j]] += G[idx[s]][idx[j]];
            if (t == -1 || dis[idx[t]] < dis[idx[j]]) t = j;
          }
        }
      }
    }
    return ans;
  }
};

signed main() {
  //file();
  ios::sync_with_stdio(false); 
  cin.tie(0);
   
  return 0;
}
```

# 58. 动态图连通性(离线)

```cpp
//
//  main.cpp
//  2022-08-14 17:32
//
//  Created by liznb
//  

#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL
#include "/Users/liznb/algo/liznb.h"
#endif

// https://loj.ac/s/1554369
//
// 动态图连通性(离线)
// 做法：
//    线段树分治，下标是时间，用 lazy 标记覆盖一条边出现的时间段。
//    回滚并查集在线段树里 dfs 维护连通性。
// 需要注意的地方:
//   1. 需要单独处理到最后也没消失的边。

struct DSU {
  vector<int> f, size;   
  vector<array<int, 2>> ops;
  DSU(int n) : f(n + 1), size(n + 1, 1) { iota(f.begin(), f.end(), 0); }

  inline int find(int x) {
    while (x != f[x]) x = f[f[x]];
    return x;
  }

  inline bool merge(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (size[x] > size[y]) swap(x, y);
    ops.push_back({x, f[x]});
    f[x] = y;    
    ops.push_back({~y, size[y]});
    size[y] += size[x];
    return true;
  }

  void rollback(int t) {
    while ((int) ops.size() > t) {
      assert(ops.size());
      auto [i, j] = ops.back();
      ops.pop_back();
      if (i >= 0) {
        f[i] = j;
      } else {
        size[~i] = j;
      }
    }
  }
};

struct Segment_Tree {
  struct Tree {
    int l, r; 
  };

  vector<Tree> tree;
  vector<vector<array<int, 2>>> lazy;

  Segment_Tree(int n) {
    tree.resize(n * 4 + 10);  
    lazy.resize(n * 4 + 10);
  }

  void build(int pos, int l, int r) {
    tree[pos].l = l; tree[pos].r = r;
    int m = l + (r - l) / 2;
    if (l == r) return;
    build(pos << 1, l, m);
    build(pos << 1 | 1, m + 1, r);
  }

  void add(int pos, array<int, 2> e) {
    lazy[pos].push_back(e);
  }

  void modify(int pos, int l, int r, array<int, 2> e) {
    assert(l <= r);
    if (tree[pos].l >= l && tree[pos].r <= r) {
      add(pos, e);
      return;
    }  
    int m = tree[pos].l + (tree[pos].r - tree[pos].l) / 2;
    if (l <= m) modify(pos << 1, l, r, e);
    if (r > m) modify(pos << 1 | 1, l, r, e);
  } 
};

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n, m; cin >> n >> m;

  vector<map<int, int>> has(n + 1);
  vector<array<int, 2>> ask(m + 1, {-1, -1});

  Segment_Tree T(m);
  T.build(1, 1, m);

  DSU dsu(n);

  for (int i = 1; i <= m; i++) {
    int op, x, y; cin >> op >> x >> y;
    if (op == 0) {
      has[x][y] = i;
      has[y][x] = i;
    } else if (op == 1) {
      // 注意 l <= r
      if (has[x][y] + 1 <= i - 1)
        T.modify(1, has[x][y] + 1, i - 1, {x, y});
      has[x].erase(y);
      has[y].erase(x);
    } else {
      ask[i] = {x, y};
    }
  }

  // 单独处理到最后也没消失的边
  for (int i = 1; i <= n; i++) {
    vector<array<int, 2>> erase;
    for (auto &it : has[i]) {
      if (it.second + 1 <= m)
        T.modify(1, it.second + 1, m, {i, it.first});
      erase.push_back({i, it.first});
    }
    for (auto &it : erase) {
      has[it[0]].erase(it[1]);
      has[it[1]].erase(it[0]);
    }
  }

  vector<int> ans;

  // 对线段树 dfs
  function<void(int)> dfs = [&] (int pos) {
    int backup = dsu.ops.size();

    for (auto &it : T.lazy[pos]) {
      dsu.merge(it[0], it[1]);
    }

    if (T.tree[pos].l == T.tree[pos].r) {
      int p = T.tree[pos].l;
      if (ask[p] != (array<int, 2>) {-1, -1}) {
        if (dsu.find(ask[p][0]) == dsu.find(ask[p][1])) {
          ans.emplace_back(1);
        } else {
          ans.emplace_back(0);
        }
      }
      dsu.rollback(backup);
      return;
    }

    dfs(pos << 1);
    dfs(pos << 1 | 1);

    dsu.rollback(backup);
  };

  dfs(1);

  for (auto &it : ans) {
    if (it) cout << "Y" << endl;
    else cout << "N" << endl;
  }

  return 0;
}
```

# 59. 二维树状数组(动态开点)

```cpp
//
//  main.cpp
//  2022-09-06 19:41
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 动态开点树状数组
// 说明：
// 1. 单点修改 区间查询
// 2. pbds 或者 unordered_map 会快一点
// 3. CF C++ 20 使用 pbds 会 CE
// 4. 范围大时，速度很慢!, 比动态开点线段树慢, 如果需要更快，
//    可以考虑把第一维换成普通树状数组。
//
// 注意! 
// 1. 如果区间范围不在 int 内，最好 define int long long
// 2. 一维动态开点，首选动态开点线段树, 或者离散化。
//
// 附 pbds:
// #include <bits/extc++.h>
// using namespace __gnu_pbds;
// gp_hash_table<int, T> tree;

template<class T>
struct BIT {

  unordered_map<int, T> tree;
  int n;

  BIT() {}
  BIT(int _n) : n(_n) {}

  void modify(int x, T val) {
    assert(x >= 1);
    while (x <= n) {
      tree[x] += val;
      x += x & -x;
    }
  }

  T query(int x) {
    assert(x >= 0);
    T ans = 0;
    while (x) {
      ans += tree[x];
      x -= x & -x;
    }
    return ans;
  }
};

// 二维动态开点树状数组
// 单点修改，区间查询
template<class T>
struct BIT2d {

  unordered_map<int, BIT<T>> tree;

  int n, m;

  BIT2d(int _n, int _m) : n(_n), m(_m) {}
  
  void modify(int x, int y, T val) {
    assert(x >= 1 && y >= 1);
    while (x <= n) {
      if (tree.find(x) == tree.end()) tree[x].n = m;
      tree[x].modify(y, val);
      x += x & -x;
    }
  }

  T query(int x, int y) {
    assert(x >= 0 && y >= 0);
    T ans = 0;
    while (x) {
      if (tree.find(x) == tree.end()) tree[x].n = m;
      ans += tree[x].query(y);
      x -= x & -x;
    }
    return ans;
  }

  T get_range(int x1, int y1, int x2, int y2) {
    return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  BIT2d<long long> T(1e9, 1e9);

  T.modify((int) 1e9 - 1, (int) 1e9 - 1, 100);

  cout << T.get_range(1, 1, (int) 1e9, (int) 1e9);

  return 0;
}

```

# 60. 二维树状数组(区间操作)

```cpp
//
//  main.cpp
//  2022-07-16 20:07
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

struct BIT2d{
  vector<vector<array<long long, 4>>> tree;
  int maxn, maxm;

  BIT2d(int n, int m) : tree(n + 1, vector<array<long long, 4>> (m + 1, {0ll, 0ll, 0ll, 0ll})), maxn(n), maxm(m){}

  inline int lowbit(int x) {
    return x & (-x);
  }

  void modify(int x, int y, long long val) {
    assert(x > 0 && y > 0);
    for (int k = x; k <= maxn; k += lowbit(k)) {
      for (int l = y; l <= maxm; l += lowbit(l)) {
        tree[k][l][0] += val;
        tree[k][l][1] += x * val;
        tree[k][l][2] += y * val;
        tree[k][l][3] += x * y * val;
      }
    }
  }

  long long query(int x, int y) {
    assert(x >= 0 && y >= 0);
    long long ans = 0ll;
    for (int i = x; i > 0; i -= lowbit(i)) {
      for (int j = y; j > 0; j -= lowbit(j)) {
        ans += 1ll * (x + 1) * (y + 1) * tree[i][j][0];
        ans -= 1ll * (y + 1) * tree[i][j][1];
        ans -= 1ll * (x + 1) * tree[i][j][2];
        ans += 1ll * tree[i][j][3];
      }
    }
    return ans;
  }
  // 左上 右下
  void add(int x1, int y1, int x2, int y2, long long val) {
    modify(x1, y1, val);  
    modify(x2 + 1, y1, -val);
    modify(x1, y2 + 1, -val);
    modify(x2 + 1, y2 + 1, val);
  }

  // 左上 右下
  long long get_sum(int x1, int y1, int x2, int y2) {
    return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

```

# 61. 可持久化线段树

```cpp
//
//  main.cpp
//  2022-05-22 15:38
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

// 可持久化线段树
// 1. vector<int> ver 保存版本, add_ver 会添加一个版本.
// 2. 注意：访问版本一定用 ver[i], 访问版本一定用 ver[i], 访问版本一定用 ver[i]
// 3. 如果操作范围是连续的 1-n, 且 n 较小，可以直接 O(n) build.
// 4. modify 不增加版本修改(主要用于不使用 build 的初始化), pmodify 是可持久化修改
// 5. pmodify 一次必然增加一个版本，不能多次 pmodify 只增加一个版本，所以根据需求选择是否多点 pmodify
//
// 优化空间的方法:
// 1. 关闭 long long
// 2. Info 里面把无关的变量删了
// 3. T.tree.reserve() , 一般大小是 log2(n) * m , 可以减少很多空间
// 
// 前缀和性质:
// 不同的版本按照时间顺序(可能)具有前缀和性质，可以相减
//

const int INF = 0x3f3f3f3f; // 注意 INF 大小， 默认在 int 范围
struct Segment_Tree {
  
  struct Info {
    int sum;  // 一定考虑初始值在 insert 后且不 modify 时是否不影响 query
              // 例如 sum = 0, minn = INF, maxn = -INF
              // 也就是说可持续化线段树里会有废点，不能让这些点影响答案，(权值线段树里不会有, insert 后必 modify)
    Info () { sum = 0; }
  };

  struct Tree {
    int ls, rs; // 左儿子，右儿子 编号
    int l, r; // 左边界，右边界
    Info dat;
    Tree(int a, int b, int c, int d) : ls(a), rs(b), l(c), r(d) {} 
  };

  vector<Tree> tree; 
  vector<int> ver;

  Segment_Tree(int l, int r) {
    tree.push_back({-1, -1, l, r});
    ver.push_back((int) tree.size() - 1);
  }
 
  int add_ver(int l, int r) {
    tree.push_back({-1, -1, l, r}); 
    ver.push_back((int) tree.size() - 1);
    return (int) tree.size() - 1;
  }

  int insert(int l, int r) {
    tree.push_back({-1, -1, l, r});
    return (int) tree.size() - 1;
  }
 
  Info merge(const Info &a, const Info &b) {
    Info res;
    res.sum = a.sum + b.sum;
    return res;
  }

  void add(int pos, int val) {
    tree[pos].dat.sum += val;
    // 注意初始值是否可以直接相加，例如 sum = 0 可以，minn = INF 不可以
  }

  void push_up(int pos) {
    tree[pos].dat.sum = 0; 
    // 注意初始化！！！！！！！！！！！！！！！！
    if (tree[pos].ls != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].ls].dat);
    if (tree[pos].rs != -1) tree[pos].dat = merge(tree[pos].dat, tree[tree[pos].rs].dat);
  }

  void build(int pos, vector<int> &arr) {
    if (tree[pos].l == tree[pos].r) {
      tree[pos].dat.sum = arr[tree[pos].l];
      return;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    if (tree[pos].ls == -1) tree[pos].ls = insert(tree[pos].l, mid);
    if (tree[pos].rs == -1) tree[pos].rs = insert(mid + 1, tree[pos].r);
    build(tree[pos].ls, arr); 
    build(tree[pos].rs, arr);
    push_up(pos);
  }

  // 普通修改，不增加版本  
  void modify(int pos, int goal, int val) {
    if (tree[pos].l == tree[pos].r) {
      add(pos, val);
      return;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    if (goal <= mid) {
      if (tree[pos].ls == -1) tree[pos].ls = insert(tree[pos].l, mid);
      modify(tree[pos].ls, goal, val);
    } else {
      if (tree[pos].rs == -1) tree[pos].rs = insert(mid + 1, tree[pos].r);
      modify(tree[pos].rs, goal, val);
    }
    push_up(pos);
  }

  // 可持续化修改，插入单点  
  void pmodify(int base, int pos, int goal, int val) {
    if (tree[pos].l == tree[pos].r) {
      tree[pos].dat = tree[base].dat;  
      add(pos, val);
      return;        
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    if (goal <= mid) {
      tree[pos].rs = tree[base].rs;
      tree[pos].ls = insert(tree[pos].l, mid);
      if (tree[base].ls == -1) tree[base].ls = insert(tree[base].l, mid); 
      pmodify(tree[base].ls, tree[pos].ls, goal, val);
    } else {
      tree[pos].ls = tree[base].ls;
      tree[pos].rs = insert(mid + 1, tree[pos].r);
      if (tree[base].rs == -1) tree[base].rs = insert(mid + 1, tree[base].r);
      pmodify(tree[base].rs, tree[pos].rs, goal, val);
    }
    push_up(pos);
  }

  int calc_cnt(int l, int r, vector<array<int, 2>> &arr) {
    int pr = upper_bound(arr.begin(), arr.end(), (array<int, 2>) {r, INF}) - arr.begin(); 
    int pl = lower_bound(arr.begin(), arr.end(), (array<int, 2>) {l, -INF}) - arr.begin();  
    return pr - pl;
  }

  // 可持续化修改，插入许多单点(不是区间修改)
  void pmodify(int base, int pos, vector<array<int, 2>> &arr) {
    if (tree[pos].l == tree[pos].r) {
      tree[pos].dat = tree[base].dat; 
      auto it = lower_bound(arr.begin(), arr.end(), (array<int, 2>){tree[pos].l, -INF});
      add(pos, (*it)[1]);
      return;        
    }

    int mid = (tree[pos].l + tree[pos].r) >> 1;

    if (calc_cnt(tree[pos].l, mid, arr)) {
      tree[pos].ls = insert(tree[pos].l, mid);
      if (tree[base].ls == -1) tree[base].ls = insert(tree[base].l, mid);
      pmodify(tree[base].ls, tree[pos].ls, arr);
    } else tree[pos].ls = tree[base].ls;

    if (calc_cnt(mid + 1, tree[pos].r, arr)) {
      tree[pos].rs = insert(mid + 1, tree[pos].r);
      if (tree[base].rs == -1) tree[base].rs = insert(mid + 1, tree[base].r);
      pmodify(tree[base].rs, tree[pos].rs, arr);
    } else tree[pos].rs = tree[base].rs;

    push_up(pos);
  }

  Info query(int pos, int l, int r) {
    if (tree[pos].l >= l && tree[pos].r <= r) {
      return tree[pos].dat;
    }
    int mid = (tree[pos].l + tree[pos].r) >> 1;
    Info res;  //注意初始值

    if (l <= mid && tree[pos].ls != -1) {
      res = merge(res, query(tree[pos].ls, l, r));
    }
    if (r >= mid + 1 && tree[pos].rs != -1) {
      res = merge(res, query(tree[pos].rs, l, r));
    }
    push_up(pos);
    return res;
  }
};

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);
  int n, m; cin >> n >> m;
  vector<int> a(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];      
  }
  vector<int> b = a;
  sort(b.begin() + 1, b.end());
  b.erase(unique(b.begin() + 1, b.end()), b.end());
  vector<int> ord(n + 1); 
  for (int i = 1; i <= n; i++) {
    ord[i] = lower_bound(b.begin() + 1, b.end(), a[i]) - b.begin();
  }
  Segment_Tree T(1, (int) b.size() - 1);   

  for (int i = 1; i <= n; i++) {
    T.add_ver(1, (int) b.size() - 1);
    vector<array<int, 2>> arr;
    arr.push_back({ord[i], 1});
    T.pmodify(T.ver[i - 1], T.ver[i], arr);
  }

  for (int i = 1; i <= m; i++) {
    int l, r, k; cin >> l >> r >> k;
    // 区间第 k 小
    function<int(int, int, int)> kth = [&] (int posl, int posr, int k) {
      int cntl = 0; 
      if (T.tree[posr].l == T.tree[posr].r) return T.tree[posr].l;
      if (T.tree[posr].ls != -1) cntl += T.tree[T.tree[posr].ls].dat.sum;
      if (posl != -1 && T.tree[posl].ls != -1) cntl -= T.tree[T.tree[posl].ls].dat.sum;
      if (cntl >= k) {
        if (posl == -1) return kth(-1, T.tree[posr].ls, k);
        else return kth(T.tree[posl].ls, T.tree[posr].ls, k);
      } else {
        if (posl == -1) return kth(-1, T.tree[posr].rs, k - cntl);
        else return kth(T.tree[posl].rs, T.tree[posr].rs, k - cntl);
      }
    };
    cout << b[kth(T.ver[l - 1], T.ver[r], k)] << endl; 
  }
   
  return 0;
}
```

# 62. 快速傅里叶变换

```cpp
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

  Poly &operator *= (Poly b) {
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
```

# 63. 最小费用最大流(zkw)

```cpp
//
//  main.cpp
//  2022-04-14 11:53
//
//  Created by liznb
//  

#include <bits/stdc++.h>
#define int long long
#define endl '\n'
using namespace std;

// 特性说明：
// 1. 自环会忽略，重边不影响。
// 2. edges 参数分别是 u, v, residual, cost
// 3. 可以在费用负环图跑, SPFA 已加入 SLF + swap 优化

struct ZKW_MCMF {
  vector<array<int, 4>> edges;    
  vector<vector<int>> G;
  vector<int> vis, d;
  const long long INF = 0x3f3f3f3f3f3f3f3f;
  
  ZKW_MCMF(int n) : G(n + 1), vis(n + 1), d(n + 1) {}

  int s, t; long long cost;
  
  void add(int u, int v, int cap, int cost) {
    edges.push_back({u, v, cap, cost});
    edges.push_back({v, u, 0, -cost});
    G[u].push_back((int) edges.size() - 2);
    G[v].push_back((int) edges.size() - 1);
  }

  bool SPFA() {
    fill(d.begin(), d.end(), INF);
    fill(vis.begin(), vis.end(), 0);

    deque<int> que; que.push_front(t);
    vis[t] = 1; d[t] = 0;
    
    while (!que.empty()) {
      int x = que.front(); que.pop_front();
      vis[x] = false;
      for (auto &it : G[x]) {
        auto &[v, u, r, c] = edges[it ^ 1];
        if (u != v && r && d[u] + c < d[v]) {
          d[v] = d[u] + c;
          if (!vis[v]) {
            vis[v] = true;
            que.push_back(v);
            if (que.front() > que.back()) {
              swap(que.front(), que.back());
            }
          }
        }
      }
    }
    return d[s] != INF;
  }

  int DFS(int x, int low) {
    vis[x] = true; 
    if (x == t) return low;
    int flow = 0, f;
    for (auto &it : G[x]) {
      auto &[u, v, r, c] = edges[it]; 
      if (d[v] == d[x] - c && r && !vis[v]) {
        f = DFS(v, min(low, r));
        flow += f; 
        low -= f; 
        r -= f;
        edges[it ^ 1][2] += f;
        cost += 1ll * f * c;
        if (!low) break;
      }
    }
    return flow;
  }
  long long Mincost(int s, int t, int & flow) {
    this->s = s, this->t = t;
    flow = 0, cost = 0ll;
    while (SPFA()) {
      do {
        fill(vis.begin(), vis.end(), 0);
        flow += DFS(s, INF);
      } while (vis[t]);
    }
    return cost;
  }

};

signed main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  int n, m, s, t;
  cin >> n >> m >> s >> t;

  ZKW_MCMF mcmf(n);

  for (int i = 1; i <= m; i++) {
    int u, v, w, c;
    cin >> u >> v >> w >> c;
    mcmf.add(u, v, w, c);
  }

  int flow = 0; 
  int cost = mcmf.Mincost(s, t, flow);
  cout << flow << " " <<  cost << endl;
  return 0;
}
```

