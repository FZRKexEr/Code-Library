set nu et cin wrap hls hid acd
set ts=2 sts=2 sw=2 cb=unnamed
syntax on
nnoremap <space>r :call Run() <CR>
fu! Run()
  silent exe 'w'
  exe '!g++ % -std=c++17 -Wall -Wextra -Wshadow -O2 && ./a.out < in.txt'
  silent exe '!rm a.out'
endf
