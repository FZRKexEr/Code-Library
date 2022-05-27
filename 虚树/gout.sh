limit=1

echo "输入执行 std 程序的命令"
read cmd

while([[ -f "./test/"$limit".in" ]]) do
  echo "正在生成第"$limit"组数据的答案。"
  (cat "./test/"$limit".in" | $cmd) > "./test/"$limit".out"

  if [ $? != 0 ]; then 
    break 
  fi

  let "limit++"
done
