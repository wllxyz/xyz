#!/bin/bash
cd $(dirname $0)
../../bin/wll step1.xyz nAnBnC.in | ../../bin/wll step2.xyz - | diff - nAnBnC.out

