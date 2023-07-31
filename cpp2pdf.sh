#!/bin/bash
# 
# cpp -> md -> pdf

now_time=$(date "+%Y-%m-%d")

cat > print.md << EOF
---
title: "Code-Library"
author: "LiZnB"
date: "$now_time"
numbersections: true
toc: true
toc-title: "目录"
header-includes: |
  \\usepackage[top=2cm, bottom=1.5cm, left=2cm, right=2cm]{geometry}
  \\usepackage{sectsty}
  \\sectionfont{\\clearpage}
---

EOF

for dir in `ls`
do
  if [ -d "$dir" ]; then
    echo $dir
    for file in `ls $dir`
    do
      if [[ ${file##*.} == 'cpp' || ${file##*.} == 'sh' ]]; then
        echo '# '$dir >> 'print.md'
        echo '' >> 'print.md'
        echo "\`\`\`${file##*.}" >> 'print.md'
        cat $dir"/"$file >> 'print.md'
        echo "\`\`\`" >> 'print.md'
        echo '' >> 'print.md'
      fi
    done
  fi
done

pandoc --pdf-engine=xelatex --highlight-style kate -V CJKmainfont="Source Han Serif SC" "print.md" -o "print.pdf"

rm "print.md"
