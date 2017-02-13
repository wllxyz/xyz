#!/bin/bash

cd $(dirname $0)
dir=$(pwd)
cd ../../
./bin/main.bin $dir/counter.xyz $dir/counter.in

cd $dir
echo "8+9" | ../../../script/wll add.xyz - | ../../../script/wll counter.xyz -
