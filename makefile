#!make
###############################################################################
# author: Wangliliang
# date: 2011/3/12
# email: postwangliliang@163.com
# any bug or issue please contact me
###############################################################################
#--------------------------------------
#DEBUG flag to control "-g -pg" option
#0-- to disable, 1-- to enable
DEBUG=1
#--------------------------------------
#to set the default compiler, default use c++
GCC=clang++-3.5 -std=c++11
#--------------------------------------
#here you can set your customer macros defined for compiler
DEFINE_OPTION=-DPARSER=2 -DINTERPRETER=3 #-DDEBUG
#DEFINE_OPTION=-DWLL1LOADER -DPARSER=0 -DSEARCH_ALL_SOLUTIONS #-DDEBUG
STATIC_LINK=1
#--------------------------------------
#you can add the third part lib's include directories here, lib's developed by yourself should linked in "sub" directory
INCLUDE_OPTION=
#--------------------------------------
#you can add the third part lib's lib directories here, lib's developed by yourself should linked in "sub" directory
LIB_OPTION=-lpthread
#--------------------------------------
install_dir=$(INSTALL_HOME)/xyz/xyz
#--------------------------------------
include makefile.linux
