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

