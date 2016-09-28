#!/bin/bash
#
cd $(dirname $0)
echo "make project ..."
make -C ..
echo "make project done"

echo "run unit test ..."
for file in bin/Test*.bin
do
        echo "run unit test case $file ..."
        ./$file
	echo "run unit test case $file done"
done
echo "run unit test done"

echo "run logic test ..."
for dir in $(ls logic-test)
do
	echo "run logic-test/$dir ..."
	./bin/main.bin logic-test/$dir/$dir.xyz logic-test/$dir/$dir.in | diff - logic-test/$dir/$dir.out
	echo "run logic-test/$dir done"
done
echo "run logic test done"


