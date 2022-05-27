echo "输入执行带测程序的命令"
read cmd

num=1

WAlist=()
watot=0
actot=0

RED=$(tput setaf 1) #红
GREEN=$(tput setaf 2) # 绿
YELLOW=$(tput setaf 3) # 黄
BLUE=$(tput setaf 4) # 蓝
PINK=$(tput setaf 5) # 粉红
RES=$(tput sgr0) # 清除颜色

while([[ -f "./test/"$num".in" ]]) do
  echo "第"$num"组数据。"
  $cmd < "./test/"$num".in" > "out.txt"
  res=$?
  if [ $res == 127 ]; then break; fi  # cmd 错了直接退出

  if [ $res != 0 ] 
  then 
    echo "${YELLOW}RE${RES}"
  else
    diff -b -B "out.txt" "./test/"$num".out" > /dev/null 2>&1
    if [ $? != 0 ]; then
      echo "${RED}WA${RES}"
      WAlist[$watot]=$num
      let "watot++"      
    else 
      echo "${GREEN}AC${RES}"
      let "actot++"
    fi
  fi
  let "num++"
done

rm out.txt

echo "${BLUE}AC "$actot"/"$((num - 1))"${RES}"

if [ ${#WAlist[*]} != 0 ]; then
  echo "${RED}WA 的数据${RES}"
  echo "${YELLOW}${WAlist[*]}${RES}"
fi
