cd $(dirname $0)

../../bin/wll expression.xyz expression.in | diff - expression.out
