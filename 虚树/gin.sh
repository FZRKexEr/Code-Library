# 说明:
# 1. 编写好 generate.py 就可以用这个程序生成数据了
# 2. 数据存放在 ./test/

echo "输入执行数据生成器的命令"
read cmd

if [[ ! -d "./test" ]]; then
  mkdir test
fi

st=1
while([[ -f "./test/"$st".in" ]]) do
  let "st++"
done
let "st--"

echo "test 目录中已经有"$st"组数据"

echo "输入要生成多少组数据: "
read n 

tot=1
while(($tot <= n))
do
  number=$((tot + st))
  echo "正在生成第"$number"组"
  file="./test/"$number".in"
  $cmd > $file
  let "tot++"
done
echo "结束!"
