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
    int fa, lazy, cnt, size;
    T val;
    Node() : son({0, 0}) { fa = lazy = 0; cnt = size = 1; }
  };
  

};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  LCT<int> T;

  return 0;
}
