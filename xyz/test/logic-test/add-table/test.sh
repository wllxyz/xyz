#!/bin/bash

cd $(dirname $0)
../../bin/wll counter.xyz counter.in | diff - counter.out
../../bin/wll add.xyz add.in | ../../bin/wll counter.xyz - | diff - add.out
