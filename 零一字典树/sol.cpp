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
 * 4. 和异或相关的题大概率可以使用。
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

