#include <bits/stdc++.h>

using namespace std;

// Splay 伸展树
// 说明：
// 1. 能做常规平衡树能做的事情。
// 2. 如果要 合并平衡树/可持久化，请用 Treap
// 3. 速度比 Treap 快。
// 4. 可以自定义数据类型。
// 5. 空结点指向 0，如果要修改，请注意 val 的初始值。

template<class T>
struct Splay {
  struct Node {
    array<int, 2> son;
    int fa, size, cnt;
    T val;

    Node() : son({0, 0}) {
      fa = size = cnt = 0;
    }

    Node(T _val) : val(_val) {
      size = cnt = 1;
      fa = 0;
    }
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

  // 查询比 val 小的数的个数 + 1，如果 val 不存在，assert(0);
  int rank(T val) {
    int res = 0, pos = root;
    while (true) {
      if (pos == 0) assert(0);
      if (val < tree[pos].val) {
        pos = tree[pos].son[0];
      } else {
        res += tree[tree[pos].son[0]].size;
        if (val == tree[pos].val) {
          splay(pos);
          return res + 1;
        }
        res += tree[pos].cnt;
        pos = tree[pos].son[1];
      }
    }
  }

  // 查询第 k 小的数，请务必保证 k 不超过总的元素个数
  T kth(int k) {
    int pos = root;
    assert(k >= 1 && k <= tree[root].size);

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

    tree[y].son[o] = tree[pos].son[o ^ 1];
    if (tree[pos].son[o ^ 1]) tree[tree[pos].son[o ^ 1]].fa = y;

    tree[pos].son[o ^ 1] = y;
    tree[y].fa = pos;

    tree[pos].fa = z;
    if (z) tree[z].son[y == tree[z].son[1]] = pos;

    update(y), update(pos);
  }

  void splay(int pos) {
    for (int fa = tree[pos].fa; fa = tree[pos].fa, fa; rotate(pos))
      if (tree[fa].fa) rotate(get_o(pos) == get_o(fa) ? fa : pos);
    root = pos;
  }
};

int main() {

  Splay<int> T;

  int n; cin >> n;

  for (int i = 1; i <= n; i++) {
    int opt, x; cin >> opt >> x;
    if (opt == 1) T.insert(x);
    if (opt == 2) T.erase(x);
    if (opt == 3) cout << T.rank(x) << endl;
    if (opt == 4) cout << T.kth(x) << endl;
    if (opt == 5) cout << T.pre(x) << endl;
    if (opt == 6) cout << T.nxt(x) << endl;
  }

  return 0;
}
