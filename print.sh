#!/bin/bash

# 把所有代码合并成一个 md 文件用于打印

if [ -f 'print.md' ]; then
  rm 'print.md'
fi

tot=1

# 生成一个目录
# echo '# 目录' >> 'print.md'
# echo '' >> 'print.md'
# 
# for file in `ls`
# do
#   if [ -d "$file" ]; then
#     echo $tot". "$file >> 'print.md'
#     let tot+=1
#   fi
# done
# 
# echo '' >> 'print.md'

# 打印代码

tot=1

for file in `ls`
do
  if [ -d "$file" ]; then
    echo '# '$tot'. '$file >> 'print.md'
    echo '' >> 'print.md'
    echo "\`\`\`cpp" >> 'print.md'
    cat $file"/main.cpp" >> 'print.md'
    echo "\`\`\`" >> 'print.md'
    echo '' >> 'print.md'
    let tot+=1
  fi
done
