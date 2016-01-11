#!/bin/bash
#
cd $(dirname $0)
make -C ..

for file in bin/Test*.bin
do
        echo $file
        ./$file
done

for dir in $(ls logic-test)
do
	echo "logic-test/$dir"
	./bin/main.bin logic-test/$dir/$dir.xyz logic-test/$dir/$dir.in | diff - logic-test/$dir/$dir.out
done


