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

