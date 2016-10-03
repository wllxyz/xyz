cd $(dirname $0)
../../bin/wll grammar.wll2 add.xyz add.in | diff - add.out

