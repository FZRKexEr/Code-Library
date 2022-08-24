#include <bits/stdc++.h>

using namespace std;

// 文艺平衡树 Splay 实现
// 原理: 利用 splay 操作把 l 和 r 移动到一个子树中。
//
// 注意: 
// 1. splay 前务必用 find 找到下标，find 兼具 传递/清理 lazy 标记的作用。
//    保证 splay 过程中无需再 push_down
// 2. 在文艺平衡树题目中只需要在 rotate 的时候 update, 如果需要修改 update
//    要考虑在其他位置增加 update。

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
    root = build(1, n, 0);
    for (int i = 1; i <= n; i++) {
      tree[i].val = a[i];
    }
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
  void modify(int l, int r) {
    assert(l >= 1 && r <= tree[root].size && l <= r);
    int pos = get_range(l, r);

    tree[pos].lazy ^= 1;
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
