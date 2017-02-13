cd $(dirname $0)
../../bin/wll shell.xyz shell.in | diff - shell.out
