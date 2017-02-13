cd $(dirname $0)

../../bin/wll l3.xyz l3.in | diff - l3.out
