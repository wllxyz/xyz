cd $(dirname $0)

../../bin/wll v2.xyz v2.in | diff - v2.out
../../bin/wll v3.xyz v3.in | diff - v3.out
../../bin/wll v4.xyz v4.in | diff - v4.out
../../bin/wll v5.xyz v5.in | diff - v5.out
../../bin/wll v6.xyz v6.in | diff - v6.out
../../bin/wll v7b.xyz v7b.in | diff - v7b.out
../../bin/wll v7.xyz v7.in | diff - v7.out
../../bin/wll v.xyz v.in | diff - v.out
