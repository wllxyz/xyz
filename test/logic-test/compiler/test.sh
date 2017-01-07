cd $(dirname $0)

../../../bin/wll pl0.xyz pl0.in | diff - pl0.out
../../../bin/wll pl1.xyz pl1.in | diff - pl1.out

