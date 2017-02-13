cd $(dirname $0)

../../bin/wll c.xyz c.in | diff - c.out
../../bin/wll b.xyz c.in | diff - c.out
../../bin/wll a.xyz c.in | diff - c.out
../../bin/wll t2.xyz t2.in | diff - t2.out
../../bin/wll test_id.xyz test_id.in | diff - test_id.out
../../bin/wll l4.xyz l4.in | diff - l4.out
