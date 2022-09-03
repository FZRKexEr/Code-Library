//
//  main.cpp
//  2022-08-19 17:51
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

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

