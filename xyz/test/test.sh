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
	if [ -e logic-test/$dir/test.sh ]
	then
		logic-test/$dir/test.sh
	else
		./bin/main.bin logic-test/$dir/$dir.xyz logic-test/$dir/$dir.in | diff - logic-test/$dir/$dir.out
	fi
	echo "run logic-test/$dir done"
done
echo "run logic test done"


