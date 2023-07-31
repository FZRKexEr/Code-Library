#!/bin/bash
#
# 对拍
# sj: 数据生成器, 不要 freopen
# sol: 待测程序
# std: 暴力程序
# 
# 如果没有std 可以在 sol 中设置一些 assert, 用 RE 判断是否有错。

g++ sol.cpp -o sol
g++ sj.cpp -o sj
g++ std.cpp -o std

for i in {1..1000}
do
  ./sj > in.txt
  ./sol < in.txt > out.txt
  ./std < in.txt > out.txt

  diff -b -B out.txt std.txt

  if [ $? -eq 0 ]; then
    echo "AC " $i
  elif [ $? -eq 1]; then
    echo "WA " $i
    break
  else
    echo "RE " $i
    break
  fi
done
