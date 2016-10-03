cd $(dirname $0)

../../bin/wll c.xyz c.in | diff - c.out
../../bin/wll b.xyz c.in | diff - c.out
../../bin/wll a.xyz c.in | diff - c.out

