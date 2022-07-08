//
//  main.cpp
//  2022-07-08 20:30
//
//  Created by liznb
//  
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
int power(int a, int b) {
  assert(b >= 0);
  int base = a, ans = 1;
  while (b) {
    if (b & 1) ans = ans * base % MOD;
    base = base * base % MOD;
    b >>= 1;
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false); 
  cin.tie(0);

  return 0;
}

